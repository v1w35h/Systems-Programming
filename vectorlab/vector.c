/*
Viwesh Gupta
cs360
9/11/2024
vector lab- making a vector opaque structure to implement into a main.c project
*/
#include "vector.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct Vector {
    int size;
    int capacity;
    int64_t *values;
} Vector;

Vector *vector_new(void)
{
    // allocate memory for a new vector
    Vector *v = (Vector *)malloc(sizeof(Vector));
    // set the size & capacity to 0
    v->size = 0;
    v->capacity = 0;
    // values are assigned to NULL 
    v->values = NULL;
    return v;
}

Vector *vector_new_with_capacity(int capacity)
{
    Vector *v = (Vector *)malloc(sizeof(Vector));
    v->size = 0;
    v->capacity = capacity;
    v->values = (int64_t *)calloc(v->capacity, sizeof(int64_t)); // clears memory to 0s
    return v;
}

// 
//frees all memory associated with the vector including it's values
//
void vector_free(Vector *vec){
    free((int64_t*)vec->values);
    free(vec);
}

//
// Pushes new values to the back of the vector
//
void vector_push(Vector *vec, int64_t value){
    vec->size += 1;
    if(vec->size > vec->capacity){
        vector_resize(vec,vec->size);
    }
    vec->values[vec->size-1] = value;
}

//
// insert a value at a  specified index, and if the index is too large, just push new value to the back
//
void vector_insert(Vector *vec, int index, int64_t value){
    if(index > vec->size){
        //if the index is bigger than the size just push it to the back
        vector_push(vec, value);
    }
    else{
        // otherwise increase size and shift the values over to make room for it
        vec->size+=1;
        for(int i = vector_size(vec)-1; i > index; i--){
            int64_t tmp = vec->values[i];
            vec->values[i] = vec->values[i-1];
            vec->values[i-1] = tmp;
        }
        vec->values[index] = value;
    }

}

//
// remove the value at the index
//
bool vector_remove(Vector *vec, int index){
    // make sure that the index is within bounds
    if(index >0 && index <= vector_size(vec)){
        // set the value to NULL and decrease the size
        vec->values[index] = (int64_t)NULL;
        vec->size -= 1;
        return true;
    }
    else{
        return false;
    }
}

//
// set value into index, returns true or false based on if index is in bounds
//
bool vector_get(Vector *vec, int index, int64_t *value){
    // returns false if index is not in bounds
    if((index < 0 || index > vec->size)){
        return false;
    } 
    else{
        // returns true if there is a value at the index
        if(vec->values[index] == (int64_t)NULL){
            return true;
        }
        // false otherwise
        else if(!vec->values[index]){
            return false;
        }
        else{
            *value = vec->values[index];
            return true;
        }
    }
}

//
// set value into index, also returns true or false based on if index is in bounds
//
bool vector_set(Vector *vec, int index, int64_t value){
    // first check if the index is within bounds
    if(!(index >=0 && index <= vector_capacity(vec))){
        return false;
    } else{
        // if it is set it
        vec->values[index] = value;
        return true;
    }
}

//
// linear search through vector
//
int vector_find(Vector *vec, int64_t value){
    // search through the vector for the value, return the index
    for(int i = 0; i< vector_size(vec); i++){
        if(vec->values[i] = value){
            return i;
        }
    }
    return -1;
}

static bool comp_ascending(int64_t left, int64_t right)
{
    // Consider left <= right to mean left is in place with right. Meaning
    // we swap if left > right.
    return left <= right;
}

void vector_sort_by(Vector *vec, bool (*comp)(int64_t left, int64_t right))
{
    int i;
    int j;
    int min;
    int64_t tmp;

    for (i = 0; i < vec->size - 1; i++) {
        min = i;
        for (j = i + 1; j < vec->size; j++) {
            if (!comp(vec->values[min], vec->values[j])) {
                min = j;
            }
        }
        if (min != i) {
            tmp = vec->values[min];
            vec->values[min] = vec->values[i];
            vec->values[i]   = tmp;
        }
    }
}

void vector_sort(Vector *vec)
{
    vector_sort_by(vec, comp_ascending);
}

/* binary search, returns index of where value is or -1 
if not found(or not sorted beforehand)
*/
int vector_bsearch(Vector *vec, int64_t value){

    int64_t left = 0;
    int64_t right = vec->size-1;

    while(left <= right){
        int64_t middle = left + (right - left)/2;


        if(vec->values[middle] < value){
            left = middle + 1;
        }
        else if(vec->values[middle] < value){
            right = middle - 1;
        }
        else{
            right =  middle;
        }
    } 
    return -1;
}

//
// manipulate the size of the vector
//
void vector_resize(Vector *vec, int new_size){
    // resizing the vector
    vec->size = new_size;

    // if the new size is greater than the capacity, the values have to be reallocated
    if(new_size > vec->capacity){
        // allocate memory for a new array of values and copy the values from the original vector
        int64_t *new_vals = (int64_t*)calloc(new_size, sizeof(int64_t));
        memcpy(new_vals,vec->values, vec->size * sizeof(int64_t));

        // printf("Copied vector contains: ");
        // for(int i = 0; i < vector_size(vec); i++){
        //     printf("%ld ", new_vals[i]);
        // }
        // printf("\n");

        // free the previous values
        free(vec->values);

        // set the new values into the vector
        vec->values = new_vals;
        vec->capacity = new_size;

        // printf("Copied vector contains: ");
        // for(int i = 0; i < vector_size(vec); i++){
        //     printf("%ld ", new_vals[i]);
        // }

    }

}

//
// changes the capacity of the vector
//
void vector_reserve(Vector *vec, int new_capacity){ 
    // reset capacity, do not change size if new is smaller than size
    if(new_capacity < vec->size){
        vec->capacity = vec->size;
        return;
    }
    else if(new_capacity > vec->capacity || new_capacity > vec->size){
        int64_t *new_vals = (int64_t*)calloc(new_capacity, sizeof(int64_t));
        
        memcpy(new_vals,vec->values, vec->size * sizeof(int64_t));

        // free the previous values
        free(vec->values);

        // set the new values into the vector
        vec->capacity = new_capacity;
        vec->values = new_vals;

    }

}

//
// sets the size to zero
//
void vector_clear(Vector *vec){
    vec->size = 0;
}

//
// used to get the capacity as an int
//
int vector_capacity(Vector *vec){
    return vec->capacity;
}

//
// used to get the size as an int
//
int vector_size(Vector *vec){
    return vec->size;
}