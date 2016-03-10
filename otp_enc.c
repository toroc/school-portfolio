/******************************************************
#   CS 344 - Winter 2016 - Program 4
#   By: Carol Toro
#   File Created: 03/09/2016
#   Last Modified: 03/09/2016
#   Filename: opt_enc.c
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
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 


#define MAX_NAME 50
/*Session data structure*/
typedef struct session session;

struct session{
	char *plainFile; /*string to store file name*/
	int plainLen;/*length of plain file text*/
	char *keyFile; /*string to store key file name*/
	int keyLen; /*length of keymfile text*/
	int serverPort; /*int to store server port*/
	char *cipherTxt;/*string to store cipher text*/
	int cipherLen; /*length of cipher text*/

};

/*Data Structure Functions*/
session* createSession();
void freeSession(struct session *thisSession);

void checkCommandLine(int argcount, char *args[]);

void eror(const char *msg);
void validateFileKey(struct session *thisSession);
int fileCharCount(const char *fileName);
int main(int argc, char *argv[])
{
	/*Check command line arguments are correct*/
	checkCommandLine(argc, argv);

	struct session *curSession = createSession();

	/*save commands*/
	curSession->plainFile=argv[1];
	curSession->keyFile=argv[2];
	curSession->serverPort=atoi(argv[3]);

	/**/
	validateFileKey(curSession);


	return 0;





}



void error(const char *msg)
{
    perror(msg);
    exit(0);
}
/******************************************************
#   Session Data Structure Functions
******************************************************/
/******************************************************
#   createSession
#   @desc: allocate memory for  data structure
#       and return pointer to session object
#   @param: n/a
#   @return: pointer to session *theSession object
******************************************************/
session* createSession(){

	/*Allocate memory for the session variables*/
	session *theSession = (session*)malloc(sizeof(session));
	theSession->plainFile=(char*)malloc(sizeof(char)*MAX_NAME);
	theSession->plainLen=0;
	theSession->keyFile=(char*)malloc(sizeof(char)*MAX_NAME);
	theSession->keyLen=0;
	
	return theSession;
}

/******************************************************
#   freeSession
#   @desc: Deallocate the memory used up by user
#   @param: pointer to session datat structure object
#   @return: void
******************************************************/
void freeSession(struct session *thisSession)
{
	free(thisSession);

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
	if(argcount<=3){
		printf("Usage: %s <plainFile> <keyFile> <portNumber>\n",args[0]);
		exit(1);
	}
}


int fileCharCount(const char *fileName)
{
	/*File descriptor*/
	FILE *fileP;
	int charCount=0;

	/*open for read*/
	fileP=fopen(fileName,"r");

	if(fileP==0){
		printf("File failed to open");
	}

	char c;
	while(1){
		c = fgetc(fileP);
		if(c == EOF){
			/*reached end of file*/
			break;
		}
		/*increment count*/
		++charCount;
	}

	/*close file descriptor*/
	fclose(fileP);

	printf("%d\n",charCount);

	return charCount;

}
void validateFileKey(struct session *thisSession)
{

	printf("About to get char count in files \n");
	/*Get count of plain file and key file*/
	thisSession->plainLen=fileCharCount(thisSession->plainFile);
	thisSession->keyLen=fileCharCount(thisSession->keyFile);

	/*exit with error: if keyfile is shorter than plain file*/
	if(thisSession->keyLen < thisSession->plainLen){
		printf("Error: key \'%s\' is too short\n", thisSession->keyFile);
		exit(1);
	}


}