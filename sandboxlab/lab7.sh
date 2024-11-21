#!/bin/bash

#compiles sandbox lab

gcc -Wall -Wextra -g -o sandbox sandbox.c -I/home/smarz1/courses/cosc360/stud -L/home/smarz1/courses/cosc360/stud -lvector -fsanitize=address
