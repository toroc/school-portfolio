/******************************************************
#   CS 344 - Winter 2016 - Program 4
#   By: Carol Toro
#   File Created: 03/09/2016
#   Last Modified: 03/09/2016
#   Filename: keygen.c
#	Excecution: keygen keylength
#   Description: Prints to standard output a random key
#		of length received from command
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
int main(int argc, char*argv[])
{

	checkCommandLine(argc, argv);
	/*Store length*/
	int keyLen = atoi(argv[1]);
	/*DEBUG*/
	printf("Debug: keyLen %d \n", keyLen);

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

/******************************************************
#   generateKey
#   @desc: generate random key based on length received
#       from execution command
#   @param: int length key string should be
#   @return: n/a
******************************************************/
void generateKey(int keyLen){

	char alphabet[OP_NUM]="ABCDEFGH IJKLMNOPQRSTUVWXYZ";
	/*String to store generated key*/
	char keyString[keyLen+1];
	char curChar;
	int randIndex;
	int i=0;

	/*Loop through*/
	while(i < keyLen){
		/*Get a random index from 0-27*/
		randIndex = rand() % 27;
		/*Set the curChar to char at randIndex*/
		curChar = alphabet[randIndex];
		/*Set the curChar to index of keyString and increment i*/
		keyString[i++]=curChar;
	}

	/*Append null char*/
	keyString[i]='\0';

	/*Print to console */
	fprintf(stdout, "%s\n", keyString);
}
