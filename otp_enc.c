/******************************************************
#   CS 344 - Winter 2016 - Program 4
#   By: Carol Toro
#   File Created: 03/09/2016
#   Last Modified: 03/09/2016
#   Filename: otp_enc.c
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

#define MAX_PACKET 1000
#define MAX_NAME 50

typedef enum{INVALID=0, VALID}CTYPE;
/*Session data structure*/
typedef struct session session;

typedef struct fileStruct fileStruct;

struct fileStruct{
	char *fileName;
	int charCount;
	CTYPE validChars;
};

struct session{
	struct fileStruct *plainFile;
	struct fileStruct *keyFile;
	char *plainFileStr; /*string to store file name*/
	int plainLen;/*length of plain file text*/
	CTYPE plainChars; /*valid chars*/
	char *keyFileStr; /*string to store key file name*/
	int keyLen; /*length of keymfile text*/
	CTYPE keyChars; /*valid chars*/
	int serverPort; /*int to store server port*/
	char *cipherTxt;/*string to store cipher text*/
	int cipherLen; /*length of cipher text*/
	int dataSocket;
};

/*Data Structure Functions*/
fileStruct* createFileStruct();
void freeFileStruct(struct fileStruct *thisFile);
session* createSession();
void freeSession(struct session *thisSession);

void checkCommandLine(int argcount, char *args[]);

void error(const char *msg);
void validateFileKey(struct session *thisSession);

/*File and Key validation helper functions*/
void fileCharValidation(struct fileStruct *thisFile);
int validChar(char curChar);


void sendFile(struct session *thisSession);
void sendKey(struct session *thisSession);
void receiveCipher(struct session *thisSession);

int main(int argc, char *argv[])
{
	/*Check command line arguments are correct*/
	checkCommandLine(argc, argv);

	struct session *curSession = createSession();

	/*save commands*/
	curSession->plainFile->fileName=argv[1];
	curSession->plainFileStr=argv[1];
	curSession->keyFile->fileName=argv[2];
	curSession->keyFileStr=argv[2];
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

fileStruct* createFileStruct()
{
	/*Allocate memory for file struct vars*/
	fileStruct *thisFileStruct = (fileStruct*)malloc(sizeof(fileStruct));
	thisFileStruct->fileName=(char*)malloc(sizeof(char)*MAX_NAME);
	thisFileStruct->charCount=0;

	return thisFileStruct;
}
void freeFileStruct(struct fileStruct *thisFile){
	free(thisFile);
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
	theSession->plainFile=createFileStruct();
	theSession->keyFile=createFileStruct();

	theSession->plainFileStr=(char*)malloc(sizeof(char)*MAX_NAME);
	theSession->plainLen=0;
	theSession->keyFileStr=(char*)malloc(sizeof(char)*MAX_NAME);
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


void fileCharValidation(struct fileStruct *thisFile)
{
	/*File descriptor*/
	FILE *fileP;
	int charCount=0;

	/*open for read*/
	fileP=fopen(thisFile->fileName,"r");

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
		/*Invalid char*/
		if(validChar(c)==0){
			thisFile->validChars=INVALID;
			printf("Invalid char\n");
			break;
		}

		/*increment count*/
		++charCount;
	}

	/*Set char count*/
	thisFile->charCount=charCount;
	thisFile->validChars=VALID;

	/*close file descriptor*/
	fclose(fileP);

	printf("%s: count %d\n", thisFile->fileName, charCount);



}

int validChar(char curChar)
{

	/*Return 1 when valid*/
	int charVal=curChar;

	if(charVal == 32 || (charVal >=65 && charVal <=90)){
		return 1;
	}

	return 0;
}



void validateFileKey(struct session *thisSession)
{

	printf("About to get char count in files \n");
	/*Get count of plain file and key file & check for invalid chars*/
	fileCharValidation(thisSession->plainFile);

	fileCharValidation(thisSession->keyFile);


	/*exit with error: if keyfile is shorter than plain file*/
	if(thisSession->plainFile->charCount > thisSession->keyFile->charCount){
		printf("Error: key \'%s\' is too short\n", thisSession->keyFile->fileName);
		exit(1);
	}

	/*exit with error if files have invalid chars*/
	if(thisSession->plainFile->validChars == INVALID){
		printf("Error: invalid characters in file \'%s\'", thisSession->plainFile->fileName);
	}

	if(thisSession->keyFile->validChars == INVALID){
		printf("Error: invalid characters in file \'%s\'", thisSession->keyFile->fileName);
	}

}

void sendFile(struct session *thisSession)
{
	/*File descriptor for reading file*/
	FILE *fileFD;
	int result;

	/*buffer to store file contents*/
	char buffer[MAX_PACKET];


	/*Open file*/
	fileFD=fopen(thisSession->plainFile->fileName,"r");

	if(fileFD==NULL){
		fprintf(stderr,"Error: unable to open file.\n");
	}



	/*Continously load file items to buffer*/
	while(fgets(buffer, sizeof(buffer), fileFD)!= NULL){

		/*Send file to client*/
		result=send(thisSession->dataSocket, buffer, strlen(buffer),0);

		/*Ensure send worked*/
		if(result<0){
			fprintf(stderr,"Error: unable to send file data to client.");
		}

		/*Wait before sending next data */
		usleep(10);
	}

	printf("done sending plain text file");

	/*Close the file*/
	fclose(fileFD);
}

void sendKey(struct session *thisSession)
{
	/*File descriptor for reading file*/
	FILE *fileFD;
	int result;

	/*buffer to store file contents*/
	char buffer[MAX_PACKET];


	/*Open key file*/
	fileFD=fopen(thisSession->keyFile->fileName,"r");

	if(fileFD==NULL){
		fprintf(stderr,"Error: unable to open file.\n");
	}



	/*Continously load file items to buffer*/
	while(fgets(buffer, sizeof(buffer), fileFD)!= NULL){

		/*Send file to client*/
		result=send(thisSession->dataSocket, buffer, strlen(buffer),0);

		/*Ensure send worked*/
		if(result<0){
			fprintf(stderr,"Error: unable to send file data to client.");
		}

		/*Wait before sending next data */
		usleep(10);
	}

	printf("done sending key text file");

	/*Close the file*/
	fclose(fileFD);
}
