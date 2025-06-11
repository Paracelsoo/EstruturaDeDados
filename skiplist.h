// skiplist.h
#ifndef SKIPLIST_H
#define SKIPLIST_H

#define MAX_FEATURES 20

typedef struct SkipNode {
    int index;                       // número do registro
    double features[MAX_FEATURES];  // vetor com as features do dado
    int feature_count;              // número real de features usadas
    struct SkipNode **forward;      // ponteiros para níveis
} SkipNode;

typedef struct SkipList {
    int level;
    SkipNode *header;
} SkipList;

SkipList *sl_create(int max_level);
void sl_free(SkipList *list);
int sl_insert(SkipList *list, int index, double *features, int feature_count);
SkipNode *sl_search(SkipList *list, int index);
int sl_remove(SkipList *list, int index);
SkipNode *sl_search_by_index(SkipList *list, int index);
void print_node(SkipNode *node);
#endif

