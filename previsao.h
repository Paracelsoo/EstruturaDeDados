#ifndef RANDOM_FOREST_H
#define RANDOM_FOREST_H

#include "hash.h"

typedef struct TreeNode {
    int feature_index;      // Índice da feature usada para corte
    double threshold;       // Valor do corte
    double prediction;      // Valor predito (média dos y no nó)
    struct TreeNode* left;  // Subárvore esquerda (valores <= threshold)
    struct TreeNode* right; // Subárvore direita (valores > threshold)
    int is_leaf;            // Flag para folha
} TreeNode;

typedef struct {
    TreeNode** trees;    // Vetor de ponteiros para árvores
    int n_trees;
    int max_depth;
    int min_samples_split;
    unsigned int seed;
} RandomForest;

// Extrai dados da hash para arrays (X: double**, Y: double*)
void converter_lista_para_arrays(HashTable* table, double*** X_out, double** Y_out, int* n_out, int* num_features_out);

// Treina Random Forest com dados da hash
RandomForest* train_random_forest(HashTable* table, int n_trees, int max_depth, int min_samples_split, unsigned int seed);

// Prediz dados X usando modelo treinado
void predict_random_forest(RandomForest* rf, double** X, int n_samples, int num_features, double* y_pred_out);

// Métricas
double mean_absolute_error(double* y_true, double* y_pred, int n);
double r_squared(double* y_true, double* y_pred, int n);

// Libera memória
void free_random_forest(RandomForest* rf);
int executar_previsao(HashTable* table);

#endif
