/******************************************************
#   CS 344 - Winter 2016 - Program 4
#   By: Carol Toro
#   File Created: 10/17/2016
#   Last Modified: 10/17/2016
#   Filename: helpers.h
#   Description: misc. helper functions used
#
#
******************************************************/
#ifndef HELPERS_H
#define HELPERS_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>


/*Signal, Error, and Debug functions*/
void sigintHandle(int sigNum);
void debugTrace(const char *msg, int line);
void error(const char *msg);
#endif
