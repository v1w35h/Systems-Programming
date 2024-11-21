/*
Viwesh Gupta
cs360
10/16/2024
Page Allocator lab- creating a pool of memory that can  be used to allocate pages and free them, the information about them being allocated or free is held in  bookkeeping pages
*/

// #include "pagealloc.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//global variables
size_t num_pages = (size_t)NULL;
void *mem_start_addr = NULL;
// global variable for bk pages to save trouble for accessing memory addresses 
size_t bookkeeping_pages = (size_t)NULL;
// this global variable keeps track of the bk bytes 
char *BK = NULL;

// These bitwise functions were given by Jacob during office hours
bool test_avail(size_t pn){
    BK = (char*) mem_start_addr;
    char *byte = &BK[pn/4];
    int index = (pn%4) * 2 + 0;
    return ((*byte) >> index) & 1;
}

bool test_last(size_t pn){
    BK = (char*) mem_start_addr;
    char *byte = &BK[pn/4];
    int index = (pn%4) * 2 + 1;
    return ((*byte) >> index) & 1;
}

void set_avail(size_t pn){
    BK = (char*) mem_start_addr;
    char *byte = &BK[pn/4];
    int index = (pn%4) * 2 + 0;
    (*byte) |= (1 << index);
}


void set_last(size_t pn){
    BK = (char*) mem_start_addr;
    char *byte = &BK[pn/4];
    int index = (pn%4) * 2 + 1;
    (*byte) |= (1 << index);
}

void clear_avail(size_t pn){
    BK = (char*) mem_start_addr;
    char *byte = &BK[pn/4];
    int index = (pn%4) * 2 + 0;
    (*byte) &= ~(1 << index);
}

void clear_last(size_t pn){
    BK = (char*) mem_start_addr;
    char *byte = &BK[pn/4];
    int index = (pn%4) * 2 + 1;
    (*byte) &= ~(1 << index);
}

bool page_init(size_t pages)
{
    // bounds of pages allowed
    if(pages < 2 || pages > 262144){
        return false;
    }

    // calculate number of bookkeeping bytes
    size_t bookkeeping_bytes;
    // attempt to get the cieling 
    if((pages % 4) == 0){
        bookkeeping_bytes = (pages)/4;
    }
    else{
        bookkeeping_bytes = (pages+4)/4;
    }

    // do the same thing for bookkeeping pages
    if((bookkeeping_bytes % 4096) == 0){
        bookkeeping_pages = (bookkeeping_bytes)/4096;
    }
    else{
        bookkeeping_pages = (bookkeeping_bytes+4096)/4096;
    }

    // mmap enough memory for bookkeeping pages and number of pages
    mem_start_addr = mmap(NULL , 4096 * (pages + bookkeeping_pages), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if(mem_start_addr  == MAP_FAILED){
        printf("mmap failed\n");
        return  false;
    }

    num_pages = pages;

    return true;
}

void page_deinit(void)
{
    // munmap has to be called since mmap was called
    munmap(mem_start_addr, 4096 * (num_pages + bookkeeping_pages));
    num_pages = 0;
}

// addr + bookkeeping_pages * 4096 + index *4096
void *page_alloc(size_t pages)
{
    if(pages  <= 0){
        return NULL;
    }

    size_t index = 0;
    // find starting index && check if there is a block of contiguous pages that can be marked as taken
    size_t block_counter = 0;
    for(size_t i =0; i < num_pages; i++){
        // look for a block of pages that are not taken
        if(!test_avail(i)){
            // set the index to the first page found to be free
            if(block_counter == 0){
                index = i;
            }
            // increment the block_counter until either the loop is over or 
            // until it equals the passed in number of pages
            block_counter++;

            if(block_counter == pages){
                break;
            }
        }
        // if a page is found to be taken, then reset the block counter
        else{
            block_counter = 0;
        }
    }

    // if it got out of the for loop, but block_counter != pages, return NULL
    if(block_counter < pages){
        return NULL;
    }

    // set each page as taken 
    for(size_t i = index; i < (pages+ index); i++){
        set_avail(i);
    }
    // set the last page
    set_last(pages + index-1);

    // return pointer to the start of the first page allocatted
    return mem_start_addr + bookkeeping_pages * 4096 + index *4096;
}

void page_free(void *addr){
    if(addr == NULL){
        return;
    }
    // find the start index using pointer arithmetic
    size_t index = (addr-mem_start_addr)/4096 - bookkeeping_pages;

    // loop from starting index until a page that was set to last is found
    size_t last;
    for(size_t i = index;i < num_pages; i++){
        if(test_last(i)){
            last = i;
            break;
        }
    }
    
    // now starting from the same index up until the last that was previously found clear all taken bits
    for(size_t i = index; i <= last; i++){
        clear_avail(i);
    }
    // finally clear the last page
    clear_last(last);

}

// count and return how many pages are taken
size_t pages_taken(void){
    size_t taken= 0;
    for(size_t i = 0; i< num_pages; i++){
        if(test_avail(i)){
            taken++;
        }
    }
    return taken;
}

// count and return how many pages are free
size_t pages_free(void){
    size_t free= 0;
    for(size_t i = 0; i< num_pages; i++){
        if(!test_avail(i)){
            free++;
        }
    }
    return free;
}