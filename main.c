#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "libhashmap.h"

char* init_and_copy(const char *str) {
    char *word = (char*)malloc(strlen(str) + 1);
    strcpy(word, str);
    return word;
}

void main() {
    hashmap_t *map = hashmap_create(2, 0.5f, 2.0f);
    size_t i;
    cell_t *tmp;
    char *words[][10] = {
        { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten" },
        { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" }
    };
 
    for (i = 0; i < 10; i++) {
        hashmap_put(&map, init_and_copy(words[0][i])
				,init_and_copy(words[1][i]));
    }
	printf("test hashmap_iterate\n"); 
    hashmap_iterate(&map, hashmap_print_cell, NULL);
    printf("\n");
	printf("test get: four,five,six\n");
    printf("%s\n", hashmap_get(&map, "four"));
    printf("%s\n", hashmap_get(&map, "five"));
    printf("%s\n", hashmap_get(&map, "six"));
    printf("\n");
	printf("test remove: six\n");
    tmp = hashmap_xremove(&map, "six");
	printf("test free\n");
    hashmap_free_cell(&tmp);
	printf("test iterate\n");
    hashmap_iterate(&map, hashmap_print_cell, NULL);
	printf("test destroy\n");
    hashmap_destroy(&map);
	printf("test rehash\n");
    for (i = 0; i < 100; i++) {
		printf("i%d create ",i);
        map = hashmap_create(2, 0.5f, 2.0f);
        for (int j = 0; j < 10; j++) {
            hashmap_put(&map, init_and_copy(words[0][j])
					, init_and_copy(words[1][j]));
        }
		printf("destroy ");
        hashmap_destroy(&map);
		printf("\n");
    }
	printf("ok\n");
    getchar();
}

