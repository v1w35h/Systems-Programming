#!/bin/bash

#compiles page allocator lab

gcc -o pa main.c -L/home/smarz1/courses/cosc360/stud -lpagealloc && ./pa
gcc -Wall -Wextra -g -o  mypa main.c pagealloc.c -fsanitize=address &&  ./mypa

