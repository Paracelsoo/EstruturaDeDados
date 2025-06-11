
// skiplist.c
#include "skiplist.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <string.h>

#define SKIPLIST_MAX_LEVEL 16
#define P 0.5

static int random_level() {
    int lvl = 1;
    while ((rand() / (double)RAND_MAX) < P && lvl < SKIPLIST_MAX_LEVEL)
        lvl++;
    return lvl;
}

SkipList *sl_create(int max_level) {
    SkipList *list = (SkipList *)malloc(sizeof(SkipList));
    list->level = 1;
    list->header = (SkipNode *)malloc(sizeof(SkipNode));
    list->header->forward = (SkipNode **)malloc(sizeof(SkipNode *) * max_level);
    for (int i = 0; i < max_level; i++)
        list->header->forward[i] = NULL;
    return list;
}

void sl_free(SkipList *list) {
    SkipNode *current = list->header->forward[0];
    while (current) {
        SkipNode *next = current->forward[0];
        free(current->forward);
        free(current);
        current = next;
    }
    free(list->header->forward);
    free(list->header);
    free(list);
}

int sl_insert(SkipList *list, int index, double *features, int feature_count) {
    SkipNode *update[SKIPLIST_MAX_LEVEL];
    SkipNode *x = list->header;
    for (int i = list->level - 1; i >= 0; i--) {
        while (x->forward[i] && x->forward[i]->index < index)
            x = x->forward[i];
        update[i] = x;
    }
    x = x->forward[0];

    if (x && x->index == index) return 0; // evita duplicatas

    int lvl = random_level();
    if (lvl > list->level) {
        for (int i = list->level; i < lvl; i++)
            update[i] = list->header;
        list->level = lvl;
    }

    x = (SkipNode *)malloc(sizeof(SkipNode));
    x->index = index;
    x->feature_count = feature_count;
    memcpy(x->features, features, sizeof(double) * feature_count);
    x->forward = (SkipNode **)malloc(sizeof(SkipNode *) * lvl);

    for (int i = 0; i < lvl; i++) {
        x->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = x;
    }

    return 1;
}

SkipNode *sl_search(SkipList *list, int index) {
    SkipNode *x = list->header;
    for (int i = list->level - 1; i >= 0; i--) {
        while (x->forward[i] && x->forward[i]->index < index)
            x = x->forward[i];
    }
    x = x->forward[0];
    if (x && x->index == index)
        return x;
    return NULL;
}

int sl_remove(SkipList *list, int index) {
    SkipNode *update[SKIPLIST_MAX_LEVEL];
    SkipNode *x = list->header;
    for (int i = list->level - 1; i >= 0; i--) {
        while (x->forward[i] && x->forward[i]->index < index)
            x = x->forward[i];
        update[i] = x;
    }
    x = x->forward[0];
    if (!x || x->index != index) return 0;

    for (int i = 0; i < list->level; i++) {
        if (update[i]->forward[i] != x) break;
        update[i]->forward[i] = x->forward[i];
    }

    free(x->forward);
    free(x);

    while (list->level > 1 && list->header->forward[list->level - 1] == NULL)
        list->level--;

    return 1;
}

SkipNode *sl_search_by_index(SkipList *list, int index) {
    return sl_search(list, index);
}


