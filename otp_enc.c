/******************************************************
#   CS 344 - Winter 2016 - Program 4
#   By: Carol Toro
#   File Created: 03/09/2016
#   Last Modified: 03/09/2016
#   Filename: otp_enc.c
#	Usage: otp_enc <plainFile> <keyFile> <portNumber>
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
	int serverPort; /*int to store server port*/
	char *cipherTxt;/*string to store cipher text*/
	int cipherLen; /*length of cipher text*/
	int dataSocket;
	int socketFD; /*client socket endpoint file descriptor*/
	int serverFD; /*Socket file descriptor*/
	struct sockaddr_in serverAddr;
	struct hostent *serverIP;
};

/*Data Structure Functions*/
fileStruct* createFileStruct();
void freeFileStruct(struct fileStruct *thisFile);
session* createSession();
void freeSession(struct session *thisSession);

void checkCommandLine(int argcount, char *args[]);


void validateFiles(struct session *thisSession);

/*File and Key validation helper functions*/
void fileCharValidation(struct fileStruct *thisFile);
int validChar(char curChar);


void error(const char *msg);
void debugTrace(const char *msg, int line);

void startClient(struct session *thisSession);

void sendComms(struct session *thisSession, struct fileStruct *thisFile);

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
	curSession->keyFile->fileName=argv[2];
	curSession->serverPort=atoi(argv[3]);

	/**/
	validateFiles(curSession);


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
		exit(2);
	}
}


void debugTrace(const char *msg, int line){
	printf("%s from line # %d \n", msg, line);
}

/******************************************************
#   	funcName
#   @desc:
#
#   @param:
#
#   @return: n/a
******************************************************/

void fileCharValidation(struct fileStruct *thisFile)
{
	printf("char & count validation \n");
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

	/*Set char count and whether valid chars*/
	thisFile->charCount=charCount;
	thisFile->validChars=VALID;

	/*close file descriptor*/
	fclose(fileP);

	printf("%s: count %d\n", thisFile->fileName, charCount);



}

/******************************************************
#   	validChar
#   @desc: helper function returns whether char passed
#       in as param is valid
#   @param: char curChar
#   @return: 1: char is valid, 0: char is invalid
******************************************************/
int validChar(char curChar)
{
	/*Get int value of char*/
	int charVal=curChar;

	/*Return 1 if space or uppercase letter*/
	if(charVal == 32 || (charVal >=65 && charVal <=90)){
		return 1;
	}

	/*Char not valid*/
	printf("char not valid\n");
	return 0;
}


/******************************************************
#   	validateFiles
#   @desc: Execute flow for validating for bad chars in
#		plain and key files, as well as char count,
#		exit with error if invalid
#   @param: pointer to session data structure
#   @return: n/a
******************************************************/
void validateFiles(struct session *thisSession)
{

	printf("Validating files \n");

	/*Validate for bad characters*/
	fileCharValidation(thisSession->plainFile);
	fileCharValidation(thisSession->keyFile);

	/*exit with error if files have invalid chars*/
	if(thisSession->plainFile->validChars == INVALID){
		fprintf(stderr,"Error: invalid characters in file \'%s\'", thisSession->plainFile->fileName);
		exit(1);
	}

	if(thisSession->keyFile->validChars == INVALID){
		fprintf(stderr, "Error: invalid characters in file \'%s\'", thisSession->keyFile->fileName);
		exit(1);
	}

	/*exit with error: if keyfile is shorter than plain file*/
	if(thisSession->plainFile->charCount > thisSession->keyFile->charCount){
		fprintf(stderr, "Error: key \'%s\' is too short\n", thisSession->keyFile->fileName);
		exit(1);
	}


}



void startClient(struct session *thisSession)
{

	debugTrace("startClient ", 299);
	/*Create client socket endpoint*/
	thisSession->socketFD=(AF_INET,SOCK_STREAM, 0);

	/*Ensure it worked*/
	if(thisSession->socketFD == -1){
		fprintf(stderr, "Error: unable to open client socket endpoint.\n");
		exit(1);
	}

	/*setup server socket*/
	thisSession->serverIP=gethostbyname("localhost");

	/*Ensure it worked*/
	if(thisSession->serverIP == NULL){
		fprintf(stderr, "Error: unable to resolve server host name.\n");
	}

	/*Set up addresss*/
	thisSession->serverAddr.sin_family = AF_INET;
	thisSession->serverAddr.sin_port=htons(thisSession->serverPort);

	debugTrace("before connect", 321);

	/*Connect to server*/
	int result = connect(thisSession->socketFD, (struct sockaddr *) &thisSession->serverAddr, sizeof(thisSession->serverAddr));

	/*Ensure it worked*/
	if (result < 0){
		fprintf(stderr, "Error: unable to connect to server.\n");
	}

	debugTrace("after connect", 331);
}

/******************************************************
#   	funcName
#   @desc:
#
#   @param:
#
#   @return: n/a
******************************************************/
void sendComms(struct session *thisSession, struct fileStruct *thisFile)
{
	/*File descriptor for reading file*/
	FILE *fileFD;
	int result;

	/*buffer to store file contents*/
	char buffer[MAX_PACKET];


	/*Open file*/
	fileFD=fopen(thisFile->fileName,"r");

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

/******************************************************
#   	funcName
#   @desc:
#
#   @param:
#
#   @return: n/a
******************************************************/
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
