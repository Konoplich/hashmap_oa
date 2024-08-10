#include <stdlib.h>

typedef unsigned char uchar;
typedef uchar *VAL;
typedef uchar *KEY;

typedef struct _cell
{
	KEY key;           
	VAL value;        
} cell_t;

typedef struct _hashmap {
    cell_t **data;        //массив корзин
    size_t size;        //количество элементов в карте 
    size_t arr_size;    //размер массива корзин
    size_t limit;       //целочисленное значение количества элементов карты
    //при превышении которого происходит пересборка
    float loadFactor;   //процентное заполнение карты, при котором
    //происходит пересборка карты
    float multiplier;   //во столько раз увеличится размер карты при пересборке
} hashmap_t;
 
hashmap_t* hashmap_create(size_t limit, float loadFactor, float multiplier);
void hashmap_raw_put(hashmap_t **hmap, cell_t *e);
void hashmap_put(hashmap_t **hmap, KEY key, VAL value);
hashmap_t* hashmap_rehash_up(hashmap_t **hmap, cell_t* e);
void hashmap_destroy(hashmap_t **hmap);
VAL hashmap_get(hashmap_t **hmap, KEY key);
cell_t* hashmap_xremove(hashmap_t **hmap, KEY key);
void hashmap_iterate(hashmap_t **hmap, void(*f)(cell_t*, void*), void* data);
void hashmap_print_cell(cell_t *e, void* data);
void hashmap_free_cell(cell_t **e);


