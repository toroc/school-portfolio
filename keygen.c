/******************************************************
#   CS 344 - Winter 2016 - Program 4
#   By: Carol Toro
#   File Created: 03/09/2016
#   Last Modified: 03/09/2016
#   Filename: keygen.c
#	Excecution: keygen keylength
#   Description:  
#       
#       
#       
#       
#       
#       
#
#
******************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define OPS = 27 /*Operations*/

void checkCommandLine(int argcount, char *args[]);
void generateKey(int keyLen);
int main(int argc, char*argv)
{

	checkCommandLine(argc, argv);
	/*Store length*/
	int keyLen = atoi(argv[1]);
	/*DEBUG*/
	print("Debug: keyLen %d \n", keyLen);

	/*Seed for rand*/
	srand(time(NULL));

	/*Generate key*/
	generateKey(keyLen);

	return 0;




}
/******************************************************
#   checkCommandLine
#   @desc: ensure program is executed with correct
#       command else, program ends
#   @param: int of count of command line arguments used
#		array of strings with values of arguments
#   @return: n/a
******************************************************/
void checkCommandLine(int argcount, char *args[])
{
	if(argcount<2){
		fprintf(stderr, "Usage: %s keylength\n",args[0]);
		exit(1);
	}
}

void generateKey(int keyLen){

	char alphabet[OPS]="ABCDEFGH IJKLMNOPQRSTUVWXYZ";
	/*String to store generated key*/
	char keyString[keyLen+1];
	int i=0;

	/*Loop through*/

	while(i < keyLen){

		int randIndex = rand() % OPS;
		char curChar = alphabet[randIndex];
		keyString[i++]=cur;
	}
	/*Append null char*/
	keyString[i]="\0";
	/*Print to console */
	printf("%s\n", keyString);
}