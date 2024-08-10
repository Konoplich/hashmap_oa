#include <stdlib.h>
#include <stdio.h>
#include "libhashmap.h"
#include <string.h>

static const float LOAD_FACTOR = 0.5f;
static const size_t INITIAL_SIZE = 100;
static const float MULTIPLIER = 2.0f;
 
void hashmap_free_cell(cell_t **e) {
    free((*e)->key);
    free((*e)->value);
    free(*e);
    *e = NULL;
}

//Сравнение ключей
#define CMP_EQ(a, b) (strcmp((a), (b)) == 0)
 
//Нахождение хэш-кода строки
unsigned long long hashcode(unsigned char *str) {
    unsigned long long hash = 5381;
    int c;
 
    while (c = *str++) {
        hash = ((hash << 5) + hash) ^ c;
    }
    return hash;
} 

hashmap_t* hashmap_create(size_t limit, float loadFactor, float multiplier) {
    hashmap_t *tmp = (hashmap_t*)malloc(sizeof(hashmap_t));
    tmp->arr_size = (limit >= INITIAL_SIZE) ? limit : INITIAL_SIZE;
    tmp->loadFactor = (loadFactor >= LOAD_FACTOR &&
        loadFactor <= 1.0f) ? loadFactor : LOAD_FACTOR;
    tmp->limit = (int)(tmp->loadFactor * tmp->arr_size);
    tmp->size = 0;
    tmp->multiplier = (multiplier >= MULTIPLIER) ? multiplier : MULTIPLIER;
 
    tmp->data = (cell_t**)calloc(tmp->arr_size, sizeof(cell_t*));
    return tmp;
}
void hashmap_raw_put(hashmap_t **hmap, cell_t *e) {
    //Находим хэш и положение в массиве
    unsigned long long hash = hashcode(e->key);
    size_t index = (hash % (*hmap)->arr_size);
 
    if ((*hmap)->size <= (*hmap)->limit) {
        if ((*hmap)->data[index] == NULL) {
            (*hmap)->data[index] = e;
        }
        else {
            while ((*hmap)->data[index]) {
                index++;
                if (index >= (*hmap)->arr_size) {
                    index = 0;
                }
            }
            (*hmap)->data[index] = e;
        }
    }
    else {
        *hmap = hashmap_rehash_up(hmap, e);
    }
    (*hmap)->size++;
}
 
void hashmap_put(hashmap_t **hmap, KEY key, VAL value) {
    cell_t *e = (cell_t*)malloc(sizeof(cell_t));
    e->key = key;
    e->value = value;
    hashmap_raw_put(hmap, e);
}

hashmap_t* hashmap_rehash_up(hashmap_t **hmap, cell_t* e) {
    hashmap_t *newMap = hashmap_create((size_t)(*hmap)->arr_size * (*hmap)->multiplier,
                                    (*hmap)->loadFactor,
                                    (*hmap)->multiplier);
 
    size_t i, size;
    size = (*hmap)->arr_size;
    for (i = 0; i < size; i++) {
        //не создаём новых пар, вставляем прежние
        if ((*hmap)->data[i]) {
            hashmap_raw_put(&newMap, (*hmap)->data[i]);
        }
    }
    free((*hmap)->data);
    free(*hmap);
    hashmap_raw_put(&newMap, e);
    *hmap = newMap;
    return newMap;
}

void hashmap_destroy(hashmap_t **hmap) {
    size_t i, size;
     
    size = (*hmap)->arr_size;
    for (i = 0; i < size; i++) {
        if ((*hmap)->data[i]) {
            hashmap_free_cell(&((*hmap)->data[i]));
        }
    }
 
    free((*hmap)->data);
    free(*hmap);
    *hmap = NULL;
}

VAL hashmap_get(hashmap_t **hmap, KEY key) {
    unsigned long long hash = hashcode(key);
    size_t index = (hash % (*hmap)->arr_size);
    VAL retVal = NULL;
    if ((*hmap)->data[index] != NULL) {
        if (CMP_EQ((*hmap)->data[index]->key, key)) {
            retVal = (*hmap)->data[index]->value;
        } else {
            //Если элемент не NULL, только тогда сравниваем
            while ((*hmap)->data[index] == NULL || 
                   !CMP_EQ((*hmap)->data[index]->key, key)) {
                index++;
                if (index >= (*hmap)->arr_size) {
                    index = 0;
                }
            }
            retVal = (*hmap)->data[index]->value;
        }
    }
    return retVal;
}

cell_t* hashmap_xremove(hashmap_t **hmap, KEY key) {
    unsigned long long hash = hashcode(key);
    size_t index = (hash % (*hmap)->arr_size);
    cell_t *retVal = NULL;
 
    if ((*hmap)->data[index] != NULL) {
        if (CMP_EQ((*hmap)->data[index]->key, key)) {
            retVal = (*hmap)->data[index];
            (*hmap)->data[index] = NULL;
        } else {
            while (!CMP_EQ((*hmap)->data[index]->key, key)) {
                index++;
                if (index >= (*hmap)->arr_size) {
                    index = 0;
                }
            }
            retVal = (*hmap)->data[index];
            (*hmap)->data[index] = NULL;
        }
    }
 
    (*hmap)->size--;
    return retVal;
}

void hashmap_iterate(hashmap_t **hmap, void(*f)(cell_t*, void*), void* data) {
    size_t size, i;
    size = (*hmap)->arr_size;
    for (i = 0; i < size; i++) {
        if ((*hmap)->data[i]) {
            f((*hmap)->data[i], data);
        }
    }
}

void hashmap_print_cell(cell_t *e, void* data) {
    printf("%s->%s\n",e->key, e->value);
}


