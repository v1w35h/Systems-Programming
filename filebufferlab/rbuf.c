// /*
// Viwesh Gupta
// cs360
// 9/17/2024
// Ring Buffer lab- making a ring buffer opaque structure. Able to push, pop,ignore bytes, and read/write onto buffer.
// */

// #include <stdio.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <stdio.h>
// #include <string.h>
// #include <errno.h>
// #include <sys/stat.h>

// #include "rbuf.h"


// struct RingBuffer
// {
//     size_t at;
//     size_t size;
//     size_t capacity;
//     char *buffer;
// };

// struct RingBuffer *rb_new(size_t capacity)
// {
//     struct RingBuffer *rb;
//     if (!(rb = (struct RingBuffer *)malloc(sizeof(*rb)))) {
//         // Out of memory.
//         return NULL;
//     }
//     // If we get here, all is well with the memory.
//     rb->at = 0;
//     rb->size = 0;
//     rb->capacity = capacity;
//     rb->buffer = (char *)malloc(rb->capacity * sizeof(char));
//     if (!rb->buffer) {
//         // We were able to create the structure in memory,
//         // but not the buffer. We need both, so free the
//         // structure and return NULL (error).
//         free(rb);
//         return NULL;
//     }
//     return rb;
// }

// // free must be called anytime malloc is called
// void rb_free(struct RingBuffer *rb)
// {
//     free(rb->buffer);
//     free(rb);
// }

// size_t rb_at(const struct RingBuffer *rb)
// {
//     return rb->at;
// }

// size_t rb_size(const struct RingBuffer *rb)
// {
//     return rb->size;
// }

// size_t rb_capacity(const struct RingBuffer *rb)
// {
//     return rb->capacity;
// }

// /*
// Fills in ring buffer until the capacity is filled in and updates size.
// Returns true only if data was pushed.
// */
// bool rb_push(struct RingBuffer *rb, char data)
// {
//     // push will only work until the capacity is filled 
//     if(rb->size < rb->capacity){
//         // increase size by one
//         rb->size+=1;
//         // add data to the buffer
//         rb->buffer[rb->size-1] = data;
//         return true;
//     }
//     else{
//         return false;
//     }
// }

// /*
//  pops the element at at, returns false if nothing can be popped
// */
// bool rb_pop(struct RingBuffer *rb, char *data)
// {
//     if( rb->size == 0){
//         return false;
//     }
//     else if(data){
//         *data = rb->buffer[rb->at];
        
//         // handle wrapping around by incrementing at by one and then
//         // using the mod operation to loop back
//         rb->at = (rb->at+1) % rb->capacity;
//         rb->size-=1;
    
//         return true;
//     }
//     // If data is NULL, the at index is still moved, but the data is essentially ignored
//     else if(data == NULL){
//         rb->at = (rb->at+1) % rb->capacity;
//     }
// }

// // similar to pop
// // return value at rb_at, but without changing any values
// char rb_peek(const struct RingBuffer *rb)
// {
//     if(rb->size == 0){
//         return 0;
//     }
//     return rb->buffer[rb->at];

// }

// // skip over a number of bytes, like pop over and over again(but not actually using pop)
// void rb_ignore(struct RingBuffer *rb, size_t num)
// {
//     char *data;
//     // clear if you are going to ignore more than the size
//     if(rb->size < num || rb->size < rb->at + num){
//         rb_clear(rb);
//     }
//     // arithmetically update at and size
//     else{
//         rb->at += num;
//         rb->size -= num;
//     }
// }

// // read number of bytes coming in from buf into ring buffer
// // may need two seperate memcpys 
// size_t rb_read(struct RingBuffer *rb, char *buf, size_t max_bytes)
// {
//     size_t bytes;
//     // reads the lesser between max_bytes & size 
//     if(max_bytes > rb->size){
//         bytes = max_bytes;
//     }
//     else{
//         bytes = rb->size;
//     }

//     // Reading in less bytes than capacity can hold so it doesn't need to wrap around the buffer
//     if(rb->at + bytes <= rb->capacity){
//         // copy # of bytes being read from address of rb->buffer at at index into buf
//         memcpy(buf, &rb->buffer[rb->at], bytes);
//     }
//     // more bytes are being read in than the capacity could hold, so it is required to call memcpy 2X
//     else{
//         // copying from at until the end of the buffer
//         memcpy(buf, &rb->buffer[rb->at], rb->capacity - rb->at);
//         // for next memcpy call, the amount of bytes is decremented 
//         size_t leftoverBytes = bytes - (rb->capacity - rb->at);
//         // copying from at until the end of the buffer
//         memcpy(buf + (rb->capacity - rb->at), &rb->buffer[0], leftoverBytes);
//     }
//     // update at and size 
//     rb->at = (rb->at + bytes) % rb->capacity;
//     rb->size -= bytes;

//     return bytes;
// }



// // similar to read, but you are copying bytes from buf into rb
// // capacity - size is the max amount that can be written
// // 
// size_t rb_write(struct RingBuffer *rb, const char *buf, size_t bytes)
// {
//     // can only read at most capacity - size bytes
//     if(bytes > (rb->capacity - rb->size)){
//         bytes = rb->capacity - rb->size;
//     }

//     // Don't need to wrap around to copy memory over
//     if(rb->at + bytes <= rb->capacity){
//         // same idea as read with destination flipped with source
//         memcpy(&rb->buffer[rb->at], buf, bytes);
//     }
//     // needs to wrap around, and call memcpy 2x
//     else{
//         memcpy(&rb->buffer[rb->at], buf, rb->capacity - rb->at);
//         size_t leftoverBytes = bytes - (rb->capacity - rb->at);
//         memcpy(&rb->buffer[0], buf + (rb->capacity - rb->at), rb->capacity - rb->at);
//     }
//     // only size is updated (by # of bytes written)
//     rb->size += bytes;

//     return bytes;
// }

// // Logically empty the string
// void rb_clear(struct RingBuffer *rb)
// {
//     rb->at = 0;
//     rb->size = 0;
// }
