#include "previsao.h"
#include "hash.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define NUM_FEATURES 5  // season, hr, atemp, hum, windspeed
#define FEATURES_TO_SAMPLE 3 // Nº de features sorteadas por nó para corte


// Random int com seed
static int random_int(unsigned int* seed, int max) {
    *seed = (*seed * 1103515245 + 12345) & 0x7fffffff;
    return (int)(*seed % max);
}

// Função para calcular média de array
static double mean(double* arr, int n) {
    double s = 0.0;
    for (int i = 0; i < n; i++) s += arr[i];
    return s / n;
}

// Calcula MSE do vetor y
static double mse(double* y, int start, int end, double val) {
    double sum = 0.0;
    for (int i = start; i < end; i++) {
        double diff = y[i] - val;
        sum += diff * diff;
    }
    return sum / (end - start);
}

// Compara para qsort índice pelo valor de X na feature f
typedef struct {
    double val;
    int idx;
} ValIdx;

static int cmp_validx(const void* a, const void* b) {
    double diff = ((ValIdx*)a)->val - ((ValIdx*)b)->val;
    if (diff < 0) return -1;
    else if (diff > 0) return 1;
    return 0;
}

// Função para construir árvore recursivamente
static TreeNode* build_tree(double** X, double* Y, int* sample_indices, int n_samples, int num_features,
                          int depth, int max_depth, int min_samples_split,
                          unsigned int* seed) {
    TreeNode* node = malloc(sizeof(TreeNode));
    if (!node) return NULL;

    // Calcular predição média do nó
    double node_pred = 0.0;
    for (int i = 0; i < n_samples; i++) node_pred += Y[sample_indices[i]];
    node_pred /= n_samples;

    node->prediction = node_pred;
    node->left = NULL;
    node->right = NULL;
    node->feature_index = -1;
    node->threshold = 0.0;
    node->is_leaf = 0;

    // Critérios de parada
    if (depth >= max_depth || n_samples < min_samples_split) {
        node->is_leaf = 1;
        return node;
    }

    // Selecionar FEATURES_TO_SAMPLE features aleatórias para corte
    int features[NUM_FEATURES];
    for (int i = 0; i < NUM_FEATURES; i++) features[i] = i;
    // Embaralhar features
    for (int i = NUM_FEATURES - 1; i > 0; i--) {
        int j = random_int(seed, i + 1);
        int tmp = features[i];
        features[i] = features[j];
        features[j] = tmp;
    }

    int n_features_to_try = FEATURES_TO_SAMPLE;
    if (n_features_to_try > NUM_FEATURES) n_features_to_try = NUM_FEATURES;

    double best_mse = INFINITY;
    int best_feature = -1;
    double best_threshold = 0.0;
    int best_left_count = 0;

    // Para cada feature sorteada
    for (int fi = 0; fi < n_features_to_try; fi++) {
        int f = features[fi];

        // Extrair valores e índices para ordenar
        ValIdx* sorted_vals = malloc(n_samples * sizeof(ValIdx));
        if (!sorted_vals) continue;
        for (int i = 0; i < n_samples; i++) {
            sorted_vals[i].val = X[sample_indices[i]][f];
            sorted_vals[i].idx = sample_indices[i];
        }
        qsort(sorted_vals, n_samples, sizeof(ValIdx), cmp_validx);

        // Tentar cortes entre valores distintos
        for (int i = 1; i < n_samples; i++) {
            if (sorted_vals[i].val == sorted_vals[i-1].val) continue;
            double threshold = (sorted_vals[i].val + sorted_vals[i-1].val) / 2.0;

            // Particionar
            double left_sum = 0.0, right_sum = 0.0;
            int left_count = 0, right_count = 0;
            for (int j = 0; j < n_samples; j++) {
                double v = X[sample_indices[j]][f];
                if (v <= threshold) {
                    left_sum += Y[sample_indices[j]];
                    left_count++;
                } else {
                    right_sum += Y[sample_indices[j]];
                    right_count++;
                }
            }
            if (left_count < min_samples_split || right_count < min_samples_split) continue;

            double left_mean = left_sum / left_count;
            double right_mean = right_sum / right_count;

            // Calcular mse total
            double mse_left = 0.0, mse_right = 0.0;
            for (int j = 0; j < n_samples; j++) {
                double val = Y[sample_indices[j]];
                double pred = (X[sample_indices[j]][f] <= threshold) ? left_mean : right_mean;
                double diff = val - pred;
                mse_left += diff * diff;
            }
            mse_left /= n_samples;

            if (mse_left < best_mse) {
                best_mse = mse_left;
                best_feature = f;
                best_threshold = threshold;
                best_left_count = left_count;
            }
        }
        free(sorted_vals);
    }

    if (best_feature == -1) {
        node->is_leaf = 1;
        return node;
    }

    // Criar índices para left e right
    int* left_indices = malloc(best_left_count * sizeof(int));
    int* right_indices = malloc((n_samples - best_left_count) * sizeof(int));
    if (!left_indices || !right_indices) {
        free(left_indices);
        free(right_indices);
        node->is_leaf = 1;
        return node;
    }
    int li = 0, ri = 0;
    for (int i = 0; i < n_samples; i++) {
        double v = X[sample_indices[i]][best_feature];
        if (v <= best_threshold) left_indices[li++] = sample_indices[i];
        else right_indices[ri++] = sample_indices[i];
    }

    node->feature_index = best_feature;
    node->threshold = best_threshold;

    node->left = build_tree(X, Y, left_indices, best_left_count, num_features,
                          depth + 1, max_depth, min_samples_split, seed);
    node->right = build_tree(X, Y, right_indices, n_samples - best_left_count, num_features,
                           depth + 1, max_depth, min_samples_split, seed);

    free(left_indices);
    free(right_indices);

    return node;
}

// Predição para uma única amostra (recursivo)
static double predict_tree(TreeNode* node, double* x) {
    if (!node) return 0.0;
    if (node->is_leaf) return node->prediction;
    if (x[node->feature_index] <= node->threshold)
        return predict_tree(node->left, x);
    else
        return predict_tree(node->right, x);
}

// Libera memória árvore recursivamente
static void free_tree(TreeNode* node) {
    if (!node) return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}

// Converte hash para arrays X e Y
void converter_hash_para_arrays(HashTable* table, double*** X_out, double** Y_out, int* n_out, int* num_features_out) {
    int capacity = 1000;
    double** X = malloc(capacity * sizeof(double*));
    double* Y = malloc(capacity * sizeof(double));
    int count = 0;
    if (!X || !Y) {
        *X_out = NULL; *Y_out = NULL; *n_out = 0; *num_features_out = 0;
        return;
    }
    
    for (int i = 0; i < table->size; i++) {
        HashNode* node = table->buckets[i];
        while (node) {
            if (count >= capacity) {
                capacity *= 2;
                double** X_new = realloc(X, capacity * sizeof(double*));
                double* Y_new = realloc(Y, capacity * sizeof(double));
                if (!X_new || !Y_new) {
                    for (int j = 0; j < count; j++) free(X[j]);
                    free(X); free(Y);
                    *X_out = NULL; *Y_out = NULL; *n_out = 0; *num_features_out = 0;
                    return;
                }
                X = X_new; Y = Y_new;
            }
            
            double* features_array = malloc(NUM_FEATURES * sizeof(double));
            if (!features_array) {
                for (int j = 0; j < count; j++) free(X[j]);
                free(X); free(Y);
                *X_out = NULL; *Y_out = NULL; *n_out = 0; *num_features_out = 0;
                return;
            }
            
            BikeData* d = &node->data;
            features_array[0] = (double)d->season;
            features_array[1] = (double)d->hr;
            features_array[2] = d->atemp;
            features_array[3] = d->hum;
            features_array[4] = d->windspeed;
            
            X[count] = features_array;
            Y[count] = (double)d->cnt;
            count++;
            node = node->next;
        }
    }
    
    *X_out = X;
    *Y_out = Y;
    *n_out = count;
    *num_features_out = NUM_FEATURES;
}

// Treina Random Forest completo
RandomForest* train_random_forest(HashTable* table, int n_trees, int max_depth, int min_samples_split, unsigned int seed) {
    double **X; double *Y;
    int n_samples, num_features;
    converter_hash_para_arrays(table, &X, &Y, &n_samples, &num_features);
    if (n_samples == 0) return NULL;

    RandomForest* rf = malloc(sizeof(RandomForest));
    if (!rf) {
        for (int i = 0; i < n_samples; i++) free(X[i]);
        free(X); free(Y);
        return NULL;
    }

    rf->n_trees = n_trees;
    rf->max_depth = max_depth;
    rf->min_samples_split = min_samples_split;
    rf->seed = seed;
    rf->trees = malloc(n_trees * sizeof(TreeNode*));
    if (!rf->trees) {
        free(rf);
        for (int i = 0; i < n_samples; i++) free(X[i]);
        free(X); free(Y);
        return NULL;
    }

    // Preparar índices para amostragem bootstrap
    int* indices = malloc(n_samples * sizeof(int));
    if (!indices) {
        free(rf->trees);
        free(rf);
        for (int i = 0; i < n_samples; i++) free(X[i]);
        free(X); free(Y);
        return NULL;
    }
    for (int i = 0; i < n_samples; i++) indices[i] = i;

    unsigned int local_seed = seed;

    for (int t = 0; t < n_trees; t++) {
        // Amostragem bootstrap
        int* sample_indices = malloc(n_samples * sizeof(int));
        if (!sample_indices) break;
        for (int i = 0; i < n_samples; i++) {
            int idx = random_int(&local_seed, n_samples);
            sample_indices[i] = indices[idx];
        }
        rf->trees[t] = build_tree(X, Y, sample_indices, n_samples, num_features,
                                0, max_depth, min_samples_split, &local_seed);
        free(sample_indices);
    }

    free(indices);

    // Liberar arrays X e Y pois agora já tem a floresta
    for (int i = 0; i < n_samples; i++) free(X[i]);
    free(X);
    free(Y);

    return rf;
}

// Prediz vetor y_pred para amostras X
void predict_random_forest(RandomForest* rf, double** X, int n_samples, int num_features, double* y_pred_out) {
    for (int i = 0; i < n_samples; i++) {
        double sum = 0.0;
        for (int t = 0; t < rf->n_trees; t++) {
            sum += predict_tree(rf->trees[t], X[i]);
        }
        y_pred_out[i] = sum / rf->n_trees;
    }
}

double mean_absolute_error(double* y_true, double* y_pred, int n) {
    double s = 0.0;
    for (int i = 0; i < n; i++) s += fabs(y_true[i] - y_pred[i]);
    return s / n;
}

double r_squared(double* y_true, double* y_pred, int n) {
    double mean_y = mean(y_true, n);
    double ss_tot = 0.0, ss_res = 0.0;
    for (int i = 0; i < n; i++) {
        ss_tot += (y_true[i] - mean_y) * (y_true[i] - mean_y);
        ss_res += (y_true[i] - y_pred[i]) * (y_true[i] - y_pred[i]);
    }
    if (ss_tot == 0) return 1.0;
    return 1.0 - ss_res / ss_tot;
}

void free_random_forest(RandomForest* rf) {
    if (!rf) return;
    for (int i = 0; i < rf->n_trees; i++) {
        free_tree(rf->trees[i]);
    }
    free(rf->trees);
    free(rf);
}

int executar_previsao(HashTable* table) {
    if (!table || table->count < 10) {
        printf("[ERRO] HashTable com poucos dados para Random Forest (mínimo 10).\n");
        return 1;
    }

    // Converter dados da hash para arrays X e Y
    double** X = NULL;
    double* Y = NULL;
    int n_samples = 0, num_features = 0;

    converter_hash_para_arrays(table, &X, &Y, &n_samples, &num_features);

    if (n_samples < 10) {
        printf("[ERRO] Poucos dados para treinamento (mínimo 10).\n");
        for (int i = 0; i < n_samples; i++) free(X[i]);
        free(X); free(Y);
        return 1;
    }

    int n_trees = 120;
    int max_depth = 20;
    int min_samples_split = 5;
    unsigned int seed = (unsigned int)time(NULL);

    RandomForest* rf = train_random_forest(table, n_trees, max_depth, min_samples_split, seed);
    if (!rf) {
        printf("[ERRO] Falha ao treinar Random Forest.\n");
        for (int i = 0; i < n_samples; i++) free(X[i]);
        free(X); free(Y);
        return 1;
    }

    double* y_pred = malloc(n_samples * sizeof(double));
    if (!y_pred) {
        printf("[ERRO] Falha na alocação de memória para y_pred.\n");
        free_random_forest(rf);
        for (int i = 0; i < n_samples; i++) free(X[i]);
        free(X); free(Y);
        return 1;
    }

    predict_random_forest(rf, X, n_samples, num_features, y_pred);

    double mae = mean_absolute_error(Y, y_pred, n_samples);
    double r2 = r_squared(Y, y_pred, n_samples);

    printf("\n[RESULTADOS RANDOM FOREST]\n");
    printf("Número de amostras: %d\n", n_samples);
    printf("MAE: %.3f\n", mae);
    printf("R²: %.3f\n", r2);
    printf("\n");

    // Limpar memória
    for (int i = 0; i < n_samples; i++) free(X[i]);
    free(X);
    free(Y);
    free(y_pred);
    free_random_forest(rf);

    return 0;
}