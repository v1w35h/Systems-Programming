#include "vector.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct Vector {
    int size;
    int capacity;
    int64_t *values;
} Vector;

/*
 * Creates a new vector on the heap, without setting a size or capacity
*/
Vector *vector_new(void){
    // allocate a new Vector struct on the heap
    Vector *vec = (Vector*)malloc(sizeof(Vector));
    // size and capacity are set to 0
    vec->size = 0;
    vec->capacity = 0;
    // assign a value to values pointer
    vec->values = NULL;

    return vec;
}

/*
 * Creates a new vector on the heap with a capacity, but not a size
*/
Vector* vector_new_with_capacity(int capacity){
    Vector *vec = (Vector *)malloc(sizeof(Vector));
    vec->size = 0;
    vec->capacity = capacity;
    vec->values = (int64_t *)calloc(vec->capacity, sizeof(int64_t));

    return vec;
}

//
// free the vector pointer as well as the values
//
void vector_free(Vector *vec){
    free(vec);
    free(vec->values);
}

//
// manipulate the size of the vector
//
void vector_resize(Vector *vec, int new_size){
    vec->size = new_size;
    if(new_size > vec->capacity){
        // reallocate the values, first allocate space for a new pointer to copy memory over to
        int64_t *new_values = (int64_t *)calloc(new_size, sizeof(int64_t));
        memcpy(new_values, vec->values, new_size*(sizeof(int64_t)));
        free(vec->values);
        vec->values = new_values;
        vec->capacity = vec->size;
        free(new_values);
    }
}

void vector_reserve(Vector *vec, int new_capacity){
    // changes the capacity of the vector
    vec->capacity = new_capacity;
    if(new_capacity < vec->size){
        // set the capacity to the value of the size if the size is bigger than the new_capacity
        vec->capacity = vec->size;
    } else{
        // reallocate the values, first allocate space for a new pointer to copy memory over to
        int64_t *new_values = (int64_t *)calloc(new_capacity, sizeof(int64_t));
        memcpy(new_values, vec->values, new_capacity*(sizeof(int64_t)));
        free(vec->values);
        vec->values = new_values;
        vec->capacity = vec->size;
        free(new_values);
    }
}


void vector_push(Vector *vec, int64_t value){
    // if there is not enough capacity the vector needs to be resized
    // if(vec->size > vec->capacity){
    //     vector_resize(vec, vec->size+1);
    // }
    // // pushes value to the end of the vector
    // vec->values[vec->size-1] = value;
        vec->size += 1;
    if(vec->size > vec->capacity){
        vector_resize(vec,vec->size);
    }
    vec->values[vec->size-1] = value;
}


void vector_insert(Vector *vec, int index, int64_t value){
    vector_resize(vec,vec->size+1);

}

bool vector_get(Vector *vec, int index, int64_t *value){
    if((index < 0 || index > vec->size)){
        return false;
    } 
    else{
        if(vec->values[index] == (int64_t)NULL){
            return true;
        }
        else{
            *value = vec->values[index];
            return true;
        }
    }
}

void vector_clear(Vector *vec){
    // sets the size to zero
    vec->size = 0;
}

int vector_capacity(Vector *vec){
    // used to get the capacity as an int
    return vec->capacity;
}

int vector_size(Vector *vec){
    // used to get the size as an int
    return vec->size;
}