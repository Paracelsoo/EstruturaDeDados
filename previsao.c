#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "previsao.h"

typedef struct TreeNode {
    int feature_index;
    double threshold;
    double value; // valor predito para leaf
    struct TreeNode* left;
    struct TreeNode* right;
    int is_leaf;
} TreeNode;

typedef struct {
    TreeNode** trees;
    int n_trees;
    int max_depth;
    int num_features;
} RandomForest;

// Função para extrair dados da lista para arrays
void converter_lista_para_arrays(listaBicicletas* lista, double*** X_out, double** Y_out, int* n_out, int* num_features_out) {
    int n = lista->tamanho;
    *num_features_out = 4; // hr, atemp, hum, workingday
    double** X = malloc(n * sizeof(double*));
    double* Y = malloc(n * sizeof(double));
    No* atual = lista->inicio;

    for (int i=0; i<n; i++) {
        X[i] = malloc((*num_features_out) * sizeof(double));
        X[i][0] = (double)atual->dados.hr;
        X[i][1] = atual->dados.atemp;
        X[i][2] = atual->dados.hum;
        X[i][3] = (double)atual->dados.workingday;
        Y[i] = (double)atual->dados.cnt;
        atual = atual->proximo;
    }
    *X_out = X;
    *Y_out = Y;
    *n_out = n;
}

// --- Funções auxiliares para Random Forest ---

static double mean(double* vals, int n) {
    double soma = 0.0;
    for (int i=0; i<n; i++) soma += vals[i];
    return soma / n;
}

static double mse(double* vals, int n, double media) {
    double soma = 0.0;
    for (int i=0; i<n; i++) {
        double diff = vals[i] - media;
        soma += diff * diff;
    }
    return soma / n;
}

static void bootstrap_sample(int* sample_indices, int n, int total_n) {
    for (int i=0; i<n; i++) {
        sample_indices[i] = rand() % total_n;
    }
}

// Para extrair um vetor de features para um subconjunto de indices
static void extract_feature(double* feature_vals, double** X, int* indices, int n, int feature_index) {
    for (int i=0; i<n; i++) {
        feature_vals[i] = X[indices[i]][feature_index];
    }
}

// Para extrair targets para um subconjunto
static void extract_targets(double* targets, double* Y, int* indices, int n) {
    for (int i=0; i<n; i++) {
        targets[i] = Y[indices[i]];
    }
}

// Encontrar melhor split para uma árvore de decisão
static void find_best_split(double** X, double* Y, int* indices, int n, int num_features, int* features_consider, int k_features, int* best_feature, double* best_threshold) {
    double best_mse = 1e20;
    *best_feature = -1;
    *best_threshold = 0.0;

    double* feature_vals = malloc(n * sizeof(double));
    double* left_targets = malloc(n * sizeof(double));
    double* right_targets = malloc(n * sizeof(double));

    for (int f=0; f < k_features; f++) {
        int feat = features_consider[f];
        extract_feature(feature_vals, X, indices, n, feat);

        int num_thresholds = 10;
        for (int t=0; t<num_thresholds; t++) {
            double thr = feature_vals[rand() % n];

            int left_count = 0, right_count = 0;
            for (int i=0; i<n; i++) {
                if (X[indices[i]][feat] <= thr) left_count++;
                else right_count++;
            }
            if (left_count == 0 || right_count == 0) continue;

            int li=0, ri=0;
            for (int i=0; i<n; i++) {
                if (X[indices[i]][feat] <= thr) left_targets[li++] = Y[indices[i]];
                else right_targets[ri++] = Y[indices[i]];
            }

            double left_mean = mean(left_targets, left_count);
            double right_mean = mean(right_targets, right_count);
            double left_mse = mse(left_targets, left_count, left_mean);
            double right_mse = mse(right_targets, right_count, right_mean);

            double weighted_mse = (left_count * left_mse + right_count * right_mse) / n;

            if (weighted_mse < best_mse) {
                best_mse = weighted_mse;
                *best_feature = feat;
                *best_threshold = thr;
            }
        }
    }

    free(feature_vals);
    free(left_targets);
    free(right_targets);
}

static TreeNode* create_leaf(double* Y, int* indices, int n) {
    TreeNode* node = malloc(sizeof(TreeNode));
    node->is_leaf = 1;
    double soma = 0.0;
    for (int i=0; i<n; i++) soma += Y[indices[i]];
    node->value = soma / n;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static TreeNode* train_tree(double** X, double* Y, int* indices, int n, int depth, int max_depth, int num_features) {
    if (n <= 5 || depth >= max_depth) {
        return create_leaf(Y, indices, n);
    }

    int k_features = (int)sqrt(num_features);
    if (k_features < 1) k_features = 1;

    int* features_consider = malloc(num_features * sizeof(int));
    for (int i=0; i<num_features; i++) features_consider[i] = i;

    // Embaralha features e pega as k_features primeiras
    for (int i=num_features-1; i>0; i--) {
        int j = rand() % (i+1);
        int tmp = features_consider[i];
        features_consider[i] = features_consider[j];
        features_consider[j] = tmp;
    }

    // Considera só as k_features
    // (pode-se realloc, mas não é obrigatório)
    // features_consider = realloc(features_consider, k_features * sizeof(int));

    int best_feature;
    double best_threshold;
    find_best_split(X, Y, indices, n, num_features, features_consider, k_features, &best_feature, &best_threshold);

    free(features_consider);

    if (best_feature == -1) {
        return create_leaf(Y, indices, n);
    }

    int* left_indices = malloc(n * sizeof(int));
    int* right_indices = malloc(n * sizeof(int));
    int left_count = 0, right_count = 0;

    for (int i=0; i<n; i++) {
        if (X[indices[i]][best_feature] <= best_threshold)
            left_indices[left_count++] = indices[i];
        else
            right_indices[right_count++] = indices[i];
    }

    if (left_count == 0 || right_count == 0) {
        free(left_indices);
        free(right_indices);
        return create_leaf(Y, indices, n);
    }

    TreeNode* node = malloc(sizeof(TreeNode));
    node->feature_index = best_feature;
    node->threshold = best_threshold;
    node->is_leaf = 0;

    node->left = train_tree(X, Y, left_indices, left_count, depth+1, max_depth, num_features);
    node->right = train_tree(X, Y, right_indices, right_count, depth+1, max_depth, num_features);

    free(left_indices);
    free(right_indices);

    return node;
}

static double predict_tree(TreeNode* node, double* x) {
    if (node->is_leaf) return node->value;
    if (x[node->feature_index] <= node->threshold)
        return predict_tree(node->left, x);
    else
        return predict_tree(node->right, x);
}

static void free_tree(TreeNode* node) {
    if (node == NULL) return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}

RandomForest* train_random_forest(double** X, double* Y, int n_samples, int num_features, int n_trees, int max_depth) {
    RandomForest* rf = malloc(sizeof(RandomForest));
    rf->n_trees = n_trees;
    rf->max_depth = max_depth;
    rf->num_features = num_features;
    rf->trees = malloc(n_trees * sizeof(TreeNode*));

    int* sample_indices = malloc(n_samples * sizeof(int));

    for (int i=0; i<n_trees; i++) {
        bootstrap_sample(sample_indices, n_samples, n_samples);
        rf->trees[i] = train_tree(X, Y, sample_indices, n_samples, 0, max_depth, num_features);
    }

    free(sample_indices);
    return rf;
}

double predict_random_forest(RandomForest* rf, double* x) {
    double soma = 0.0;
    for (int i=0; i<rf->n_trees; i++) {
        soma += predict_tree(rf->trees[i], x);
    }
    return soma / rf->n_trees;
}

void free_random_forest(RandomForest* rf) {
    for (int i=0; i<rf->n_trees; i++) {
        free_tree(rf->trees[i]);
    }
    free(rf->trees);
    free(rf);
}

// Função principal que junta tudo: treina e mostra métricas
void prever_random_forest(listaBicicletas* lista) {
    if (lista->tamanho < 10) {
        printf("[ERRO] Lista muito pequena para Random Forest (mínimo 10).\n");
        return;
    }
    srand((unsigned)time(NULL));

    double** X;
    double* Y;
    int n, num_features;

    converter_lista_para_arrays(lista, &X, &Y, &n, &num_features);

    int n_trees = 120;
    int max_depth = 20;

    RandomForest* rf = train_random_forest(X, Y, n, num_features, n_trees, max_depth);

    double mae=0, mse=0, rss=0, tss=0;
    double y_mean = mean(Y, n);

    for (int i=0; i<n; i++) {
        double pred = predict_random_forest(rf, X[i]);

        // Limitar previsão a faixa válida
        if (pred > 500) pred = 500;
        if (pred < 0) pred = 0;

        double erro = pred - Y[i];
        mae += fabs(erro);
        mse += erro*erro;
        rss += erro*erro;
        tss += (Y[i] - y_mean)*(Y[i] - y_mean);
    }
    mae /= n;
    mse /= n;
    double rmse = sqrt(mse);
    double r2 = 1 - rss/tss;

    printf("\n[MODELO RANDOM FOREST]\n");
    printf("Árvores: %d | Profundidade máxima: %d\n", n_trees, max_depth);
    printf("MAE: %.2f | MSE: %.2f | RMSE: %.2f | R²: %.4f\n", mae, mse, rmse, r2);
    printf("Número de observações: %d\n\n", n);

    // Liberar memória
    for (int i=0; i<n; i++) free(X[i]);
    free(X);
    free(Y);
    free_random_forest(rf);
}


