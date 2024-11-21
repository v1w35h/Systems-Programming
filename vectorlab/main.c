#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "vector.h"

int main() {
    // Create a new vector with an initial capacity of 5
    Vector *vec = vector_new_with_capacity(5);
    // Vector *vec = vector_new();
    int64_t value;
    // Vector *vec = vector_new();
    if (vec == NULL) {
        printf("Failed to create vector.\n");
        return -1;
    }
    else{
        printf("Vector created with capacity\n");
    }

    printf("Pushing into vector\n");
    vector_push(vec, 9);
    vector_push(vec, 2);
    vector_push(vec, 6);
    vector_push(vec, 4);
    vector_push(vec, 4);

    // printf("Inserting 8 into vector.\n");
    // vector_insert(vec,10,8);
    // printf("Successfully inserted.\n");

        // Print values
    for (int i = 0; i < vector_size(vec);i++) {
        vector_get(vec, i, &value);
        printf("%ld ", value);
    }
    printf("\n");

    // vector_reserve(vec, 10);
    // printf("Vector reserved new capacity: %d, Size: %d\n", vector_capacity(vec), vector_size(vec));
    // vector_reserve(vec, 1);
    // printf("Vector reserved new capacity: %d, Size: %d\n", vector_capacity(vec), vector_size(vec));

    // printf("Attempting to resize vector\n");
    // vector_resize(vec, 8);
    // printf("Vector resized!\n");

    // printf("This is the index where 9 is: %d\n",vector_find(vec, 9));
    // printf("This is the index where 6 is: %d\n",vector_bsearch(vec, 9));



    // Print values
    for (int i = 0; i < vector_size(vec);i++) {
        vector_get(vec, i, &value);
        printf("%ld ", value);
    }
    printf("\n");
    vector_get(vec, 100, &value);



    vector_remove(vec, 4);
    printf("Removed index #4\n");
    vector_set(vec, 4, 1);

        // Sort the vector
    printf("Sorting the vector\n");
    vector_sort(vec);
    for (int i = 0; i < vector_size(vec);i++) {
        vector_get(vec, i, &value);
        printf("%ld ", value);
    }
    printf("\n");

    // vector_set(vec, 10, 1);
    for (int i = 0; i < vector_size(vec);i++) {
        vector_get(vec, i, &value);
        printf("%ld ", value);
    }
    printf("\n");


    // // Clear the vector
    vector_clear(vec);
    printf("Cleared the vector. Size after clear: %d\n", vector_size(vec));
    for (int i = 0; i < vector_size(vec);i++) {
        vector_get(vec, i, &value);
        printf("%ld ", value);
    }
    printf("\n");

    // Free the vector
    printf("Attempting to free the vector.\n");
    vector_free(vec);
    printf("Freed the vector.\n");

    return 0;
}
