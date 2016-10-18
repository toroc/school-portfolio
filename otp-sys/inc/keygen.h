/******************************************************
#   CS 344 - Winter 2016 - Program 4
#   By: Carol Toro
#   File Created: 03/09/2016
#   Last Modified: 10/17/2016
#   Filename: keygen.h
#	Usage: keygen <keylength>
#   Description: Key generator function definitons.
#
#
******************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define OP_NUM 27


void checkCommandLine(int argcount, char *args[]);
void generateKey(int keyLen);

