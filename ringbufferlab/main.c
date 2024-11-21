#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rbuf.h"

#define CAP 5

// int main() {
//     struct RingBuffer *rb = rb_new(CAP);
//     char bytes[CAP];
//     size_t n;
//     char c;

//     rb_push(rb, 'A');
//     rb_push(rb, 'l');
//     rb_push(rb, 'p');
//     rb_push(rb, 'h');
//     rb_push(rb, 'a');
//     rb_push(rb, 'D');
//     rb_push(rb, 'o');
//     rb_push(rb, 'g');

//     printf("Peek: %c\n", rb_peek(rb));

//     (rb_pop(rb, &c));
//     printf("rb_pop: popped '%c'\n", c);

//     printf("%d %d \n", rb_size(rb), rb_at(rb));

//     // rb_ignore(rb, 2);
//     // rb_clear(rb);

//     printf("Peek: %c\n", rb_peek(rb));
//     printf("%d %d \n", rb_size(rb), rb_at(rb));


//     n = rb_read(rb, bytes, CAP + 5);
//     printf("%d %d \n", rb_size(rb), rb_at(rb));

//     printf("Peek after rb_read: %c\n", rb_peek(rb));

//     // n = rb_read(rb, bytes, 7);
//     // printf("Peek: %c\n", rb_peek(rb));



//     printf("rb_read: returned %lu bytes '%.*s'\n", n, n, bytes);

//     n = rb_write(rb, "Hello World", strlen("Hello World"));

//     printf("%d %d \n", rb_size(rb), rb_at(rb));

//     printf("rb_write: wrote %lu bytes.\n", n);

//     n = rb_read(rb, bytes, CAP);

//     printf("rb_read: returned %lu bytes '%.*s'\n", n, n, bytes);

//     rb_free(rb);

//     return 0;
 
// }

int main(){

    struct RingBuffer *rb = rb_new(CAP);
    char bytes[CAP];
    size_t n;
    char ch;
    printf("Stress Test:\n\n");


    if (rb != NULL) {
        printf("Ring buffer initialized correctly.\n");
    } else {
        printf("Failed to initialize ring buffer.\n");
    }

    // Test empty buffer
    printf("Initial size: %zu\n", rb_size(rb));
    printf("Buffer capacity: %zu\n", rb_capacity(rb));
    printf("Peek from empty buffer: %d\n", rb_peek(rb));

    // Try popping from empty buffer (should return false)
    if (!rb_pop(rb, &ch)) {
        printf("Pop from empty buffer: correct\n");
    } else {
        printf("Pop from empty buffer: incorrect\n");
    }

    // Fill the buffer to its capacity
    rb_push(rb, 'A');
    rb_push(rb, 'B');
    rb_push(rb, 'C');
    rb_push(rb, 'D');
    rb_push(rb, 'E');

    // Test buffer overflow: pushing into a full buffer should return false
    if (!rb_push(rb, 'F')) {
        printf("Push into full buffer: correct\n");
    } else {
        printf("Push into full buffer: incorrect\n");
    }

    // Peek the front element (should be 'A')
    printf("Peek front element: %c\n", rb_peek(rb));

    // Pop elements and check if order is preserved
    rb_pop(rb, &ch);
    printf("Popped element: %c\n", ch);
    rb_pop(rb, &ch);
    printf("Popped element: %c\n", ch);
    rb_pop(rb, &ch);
    printf("Popped element: %c\n", ch);
    rb_pop(rb, &ch);
    printf("Popped element: %c\n", ch);
    rb_pop(rb, &ch);
    printf("Popped element: %c\n", ch);
    printf("%d %d \n", rb_size(rb), rb_at(rb));


    // Buffer should now be empty again
    printf("Buffer size after popping all elements: %zu\n", rb_size(rb));

    // Try popping from empty buffer again
    if (!rb_pop(rb, &ch)) {
        printf("Pop from empty buffer (again): correct\n");
    } else {
        printf("Pop from empty buffer (again): incorrect\n");
    }

    // Push elements again to test circular nature
    printf("%d %d \n", rb_size(rb), rb_at(rb));

    rb_push(rb, 'F');
    rb_push(rb, 'G');
    rb_push(rb, 'H');
    rb_push(rb, 'I');
    rb_push(rb, 'J');

    printf("%d %d \n", rb_size(rb), rb_at(rb));


    // Pop half of the buffer to test wrap-around
    rb_pop(rb, &ch);
    printf("Popped element: %c\n", ch);
    rb_pop(rb, &ch);
    printf("Popped element: %c\n", ch);

    printf("%d %d \n", rb_size(rb), rb_at(rb));

    // Push more data and check circular behavior
    rb_push(rb, 'K');
    rb_push(rb, 'L');

    // Buffer should now be full again
    if (!rb_push(rb, 'M')) {
        printf("Push into full buffer (after wrap-around): correct\n");
    } else {
        printf("Push into full buffer (after wrap-around): incorrect\n");
    }

    // Test rb_read and rb_write
    char buffer[5];

    printf("%d %d \n", rb_size(rb), rb_at(rb));

    // size_t bytes_read = rb_read(rb, buffer,5);

    // printf("Bytes read: %zu\n", bytes_read);  // Should read 'H', 'I', 'J'
    // printf("Data read: %c%c%c\n", buffer[0], buffer[1], buffer[2]);
    n = rb_write(rb, "Hello World", strlen("Hello World"));

    printf("%d %d \n", rb_size(rb), rb_at(rb));

    printf("rb_write: wrote %lu bytes.\n", n);
    printf("%d %d \n", rb_size(rb), rb_at(rb));

        n = rb_read(rb, bytes, CAP);

    printf("rb_read: returned %lu bytes '%.*s'\n", n, n, bytes);


    // Test ignoring elements
    rb_push(rb, 'X');
    rb_push(rb, 'Y');
    rb_ignore(rb, 1);  // Should ignore one element ('K')
    printf("Peek after ignoring one element: %c\n", rb_peek(rb));
    printf("%d %d \n", rb_size(rb), rb_at(rb));


    // Test rb_clear
    rb_clear(rb);
    printf("Buffer size after clear: %zu\n", rb_size(rb));
    printf("Peek after clear: %d\n", rb_peek(rb));

    // Cleanup
    rb_free(rb);
    printf("Ring buffer freed.\n");
}