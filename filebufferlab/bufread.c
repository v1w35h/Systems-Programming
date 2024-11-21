/* Viwesh Gupta cs360
9/26
File Buffer lab
Using the previously created ring buffer to emulate a file streams buffer
recieved help from Jacob and Andrew
*/
#include "rbuf.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>


struct BufReader {
    struct RingBuffer *rb;
    size_t fill;  
    int fd; // file descriptor
};

// fill needs to always be greater than size, if not call rb_read and rb_write to essentially fill in buffer correctly. Jacob told me to write this function during office hours
static void filltrigger(struct BufReader *br){
    if(rb_size(br->rb) <= br->fill){
        // You are going to write this much into the rb: capacity - size, when it's first created this will write into the entire ring buffer
        size_t size_left = rb_capacity(br->rb) - rb_size(br->rb);
        // allocate memory for a temporary buffer of the size we can hold
        char* tempbuf = (char*)malloc(size_left);
        
        // rb_write how many bytes were read from the file into the temporary buffer
        ssize_t read_bytes = read(br->fd,tempbuf, size_left);
        // error check
        if(read_bytes < 0){
            br_close(br);
            free(tempbuf);
            return;
        }
        ssize_t bytes = rb_write(br->rb,tempbuf, read_bytes);
        
        // free temporary buffer
        free(tempbuf);
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

    // create the ring buffer
    br->rb = rb_new(capacity);

    // error check the ring buffer
    if(br->rb == NULL){
        close(fd);
        free(br);
        return 0;
    }

    // set the bufreader variables
    br->fd = fd;
    br->fill = fill_trigger;

    // call my filltrigger function to 
    filltrigger(br);

    return br;
}

void br_close(struct BufReader *br)
{
    // close the opened file
    close(br->fd);
    // free the buffer
    rb_free(br->rb);
    // free the buffer reader
    free(br);
}

// I genuienly have no idea how this is supposed to work
void br_seek(struct BufReader *br, ssize_t offset, int whence)
{
    // call lseek at whence
    if(whence == SEEK_SET && offset >= 0){
        lseek(br->rb, offset, whence);
    }
    if(whence == SEEK_END && offset <= 0){
        lseek(br->rb, offset, whence);
    }
    // location buffered into ring buffer
    if(whence == SEEK_CUR && offset >= 0 && offset < rb_size(br->rb)) {
        // ignore the bytes that were sought past
        rb_ignore(br->rb, offset);
    } else {
        // Otherwise, we need to flush the buffer
        rb_clear(br->rb);

    }
    // check the fill trigger
    if(rb_size(br->rb) <= br->fill) {
        filltrigger(br);
    }
}

size_t br_tell(const struct BufReader *br)
{
    // return error if the file is closed
    if(br == NULL){
        return -1;
    }

    // subtract the size to get to 0 (the beginning of the file)
    size_t tell = lseek(br->fd, 0,  SEEK_CUR) - rb_size(br->rb);

    // error if br_tell returns a negative number
    if(tell < 0){
        return -1;
    }
    return tell;
    
}

int br_getchar(struct BufReader *br)
{
    char ch;
    // read a singular char from the ring buffer
    rb_read(br->rb, &ch, 1);
    if(ch == EOF){
        return -1;
    }
    // if the fill trigger is reached, we need to read in more bytes
    if(rb_size(br->rb) <= br->fill) {
        filltrigger(br);
    }
    //return the value of the char (ascii)
    return (unsigned char)ch;
}

bool br_getline(struct BufReader *br, char s[], size_t size)
{
    // reading in byte by byte, starting with a zero index
    size_t byte = 0;
    // loop until end of line/file or size is reached
    while(true){
        // same idea as getchar, but now the chars are getting input into a string
        char ch;
        if(rb_read(br->rb, &ch, 1)){
            s[byte] = ch;
        } 
        // if no bytes were read return false
        else{
            return false;
        }

        // once end of line or file is reached or the number of bytes read =size
        // set last char to  '\0'
        if(ch == '\n' || ch == EOF || byte == size) {
            s[byte] = '\0';
            return true;
        }
        
        // if the fill trigger is reached, we need to read in more bytes
        if(rb_size(br->rb) <= br->fill) {
            filltrigger(br);
        }
        //  increment to next byte
        byte++;

    }
}


size_t br_read(struct BufReader *br, void *dest, size_t max_bytes)
{
    // reading in byte by byte, starting with a zero index
    size_t bytes_read = 0;
    // make a temp string that will hold the same  number of bytes as dest 
    char *str = (char*)dest;
    // return 0  if you're reading 0 or less bytes
    if(max_bytes <= 0){
        return 0;
    }
    while(true){
        // read a single char from the rb into the sttring
        rb_read(br->rb, &str[bytes_read], 1);
        bytes_read++;
        // refresh the buffer
        if(rb_size(br->rb) <= br->fill) {
            filltrigger(br);
        }
        // when max_bytes have been read break out the loop
        if(bytes_read == max_bytes){
            break;
        }
    }
    // return the amount of bytes read
    return bytes_read;
}
