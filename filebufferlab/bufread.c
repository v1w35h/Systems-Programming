/* Viwesh Gupta cs360
9/26
Buffer file lab

*/
#include "rbuf.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>


struct BufReader {
    struct RingBuffer *rb;
    size_t fill;  // fill needs to always be greater than size, if not call rb_read and rb_write to essentially fill in buffer correctly
    int fd; // fill descriptor
};

size_t filltrigger(struct RingBuffer *rb, size_t capacity, size_t fill){
    if(rb_size(rb) <= fill){
        
    }
}

struct BufReader *br_open(const char path[], size_t capacity, size_t fill_trigger)
{
    int fd = open(path, O_RDONLY);
    if(fd < 0){
        return NULL;
    }

    // Allocate memory for BufReader structure
    struct BufReader *br;
    if (!(br = (struct BufReader *)malloc(sizeof(*br)))) {
        // Out of memory.
        return NULL;
    }

    br->rb = rb_new(capacity);

    if(br->rb == NULL){
        close(fd);
        free(br);
        return 0;
    }

    br->fd = fd;
    br->fill = fill_trigger;

    char readbuf[br->fill];
    char rbbuf[br->fill];

    // read as many bytes that will fit into the 
    // ssize_t bytes = rb_read(br->rb,rbbuf,read(fd, readbuf, br->fill));
    ssize_t bytes = rb_write(br->rb,rbbuf,read(fd, readbuf, br->fill));

    printf("br_open worked bytes: %lu\n", bytes);

    return br;
}

void br_close(struct BufReader *br)
{
    close(br->fd);
    rb_free(br->rb);
    free(br);
}

// void br_seek(struct BufReader *br, ssize_t offset, int whence)
// {

// }

// size_t br_tell(const struct BufReader *br)
// {

// }

// bool br_getline(struct BufReader *br, char s[], size_t size)
// {

// }

// int br_getchar(struct BufReader *br)
// {

// }

// size_t br_read(struct BufReader *br, void *dest, size_t max_bytes)
// {
    
// }