#include <stdlib.h> // heap functions
#include <string.h> // memset
#include <stdio.h>
#include <stdbool.h> // for bool

static void *mycalloc(unsigned long nmemb, unsigned int size);
static void sort(int *values, int size);

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: %s <numbers...>\n", argv[0]);
        return -1;
    }

    // All heap memory is managed through pointers
    int *numbers;
    int i;

    // calloc stands for "contiguous allocation". 
    // The first parameter is the number of elements to create.
    // The second parameter is the size of each element.
    // Essentially, calloc multiplies first * second to come up with the number
    // of bytes.
    // calloc will CLEAR the memory to 0.
    numbers = mycalloc(argc - 1, sizeof(int));
    for (i = 1;i < argc;i += 1) {
        if (1 != sscanf(argv[i], "%d", numbers + i - 1)) {
            printf("Error reading integer at position %d ('%s').\n", i, argv[i]);
            break;
        }
    }
    sort(numbers, argc - 1);
    for (i = 0;i < argc - 1;i+=1) {
        printf("%-2d: %d\n", i, numbers[i]);
    }

    free(numbers);
}

static void *mycalloc(unsigned long nmemb, unsigned int size)
{
    return memset(malloc(nmemb * size), 0, nmemb * size);
}

static void sort(int *values, int size)
{
    bool swapped;
    int i;
    int tmp;
    int n = size;
    do {
        swapped = false;
        for (i = 1;i < n;i += 1) {
            if (values[i-1] < values[i]) {
                tmp = values[i - 1];
                values[i - 1] = values[i];
                values[i] = tmp;
                swapped = true;
            }
        }
        n -= 1;
    } while (swapped);
}
