#!/bin/bash

#compiles page allocator lab

gcc -Wall -Wextra -g -o marzlab mymain.c -L/home/smarz1/courses/cosc360/stud -ltpool -lpthread -fsanitize=address 
gcc -Wall -Wextra -g -o mylab tpool.c mymain.c -lpthread -fsanitize=address 
# vimdiff marz_sol my_sol
gcc -Wall -Wextra -g -o ic prod_cons.c -lpthread -fsanitize=address

