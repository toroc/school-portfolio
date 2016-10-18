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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>


/*Signal, Error, and Debug functions*/
void sigintHandle(int sigNum);
void debugTrace(const char *msg, int line);
void error(const char *msg);