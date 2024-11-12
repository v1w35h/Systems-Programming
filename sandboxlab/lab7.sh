#!/bin/bash

#compiles sandbox lab

gcc -g -o sandbox sandbox.c -fsanitize=address
