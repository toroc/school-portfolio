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

#define MAX_BUFFER 128000
#define MAX_PACKET 1000
#define MAX_NAME 50

typedef enum{INVALID=0, VALID}CTYPE;
/*Session data structure*/
typedef struct session session;

typedef struct fileStruct fileStruct;

struct fileStruct{
	char *fileName;
	char fileBuffer[MAX_BUFFER];
	int charCount;
	CTYPE validChars;
	int confirm; /*0: false, 1: true*/
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
void handleRequest(struct session *thisSession);

void sendComms(struct session *thisSession, struct fileStruct *thisFile);

void receiveCipher(struct session *thisSession);

int confirmACK(const char *msg);

void confirmReceived(const char *msg, struct fileStruct *thisFile);

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

	/*Start client connection*/
	startClient(curSession);

	/*take care of request*/
	handleRequest(curSession);


	return 0;





}



void error(const char *msg)
{
    perror(msg);
    exit(1);
}

fileStruct* createFileStruct()
{
	/*Allocate memory for file struct vars*/
	fileStruct *thisFileStruct = (fileStruct*)malloc(sizeof(fileStruct));
	thisFileStruct->fileName=(char*)malloc(sizeof(char)*MAX_NAME);
	thisFileStruct->charCount=0;
	thisFileStruct->confirm=0;

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
	printf("OTP_ENC > %s from line # %d \n", msg, line);
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
	/*File descriptor for reading file*/
	FILE *fileFD;
	int charCount=0;
	int result;

	/*open file for read*/
	fileFD=fopen(thisFile->fileName,"r");

	if(fileFD==0)
	{
		printf("File failed to open");
	}

	/*Save file contents in file buffer*/
	fgets(thisFile->fileBuffer, sizeof(thisFile->fileBuffer), fileFD);

	charCount=strlen(thisFile->fileBuffer);

	/*close file descriptor*/
	fclose(fileFD);

	/*Loop through char buffer & validate chars*/
	int i;
	char c;
	for (i=0; i < charCount - 1; i++)
	{
		c = thisFile->fileBuffer[i];

		/*Found invalid char*/
		if(validChar(c)==0)
		{
			debugTrace("Invalid char", 230);
			/*Set to INVALID*/
			thisFile->validChars=INVALID;
			/*Exit function*/
			return;
		}
	}
	
	/*Set char count and whether valid chars*/
	thisFile->charCount=charCount-1;
	thisFile->validChars=VALID;

	

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
	int optVal =1;

	/*Create client socket endpoint*/
	thisSession->socketFD=socket(AF_INET,SOCK_STREAM, 0);

	/*Ensure it worked*/
	if(thisSession->socketFD == -1){
		error("Error: unable to open client socket endpoint.\n");
		exit(1);
	}


	/*Allow reuse of address*/
	int result = setsockopt(thisSession->socketFD, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal));

	/*Ensure it worked*/
	if (result < 0){
		error("Error: unable to setup SO_REUSEADDR.\n");
	}

	/*setup server socket*/
	thisSession->serverIP=gethostbyname("localhost");

	/*Ensure it worked*/
	if(thisSession->serverIP == NULL){
		error("Error: unable to resolve server host name.\n");
	}

	/*Set up client socket */
	thisSession->serverAddr.sin_family = AF_INET;
	thisSession->serverAddr.sin_port=htons(thisSession->serverPort);
	thisSession->serverAddr.sin_addr.s_addr = INADDR_ANY;




	debugTrace("before connect", 321);

	/*Connect to server*/
	result = connect(thisSession->socketFD, (struct sockaddr *) &thisSession->serverAddr, sizeof(thisSession->serverAddr));

	/*Ensure it worked*/
	if (result < 0){
		error("Error: unable to connect to server.\n");
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
	
	int result;
	int bytesSent;

	/*buffer to store response contents*/
	char buffer[MAX_PACKET];

	/*Clear the bufffer*/
	bzero(buffer, MAX_PACKET);

	int val = thisFile->charCount;

	debugTrace("Before sending expected bytes", 394);

	/*Send number of bytes to expect*/
	bytesSent = send(thisSession->socketFD, &val, sizeof(int),0);

	/*Ensure message sent*/
	if(bytesSent <0){
		error("No bytes sent");
	}

	debugTrace("Before waiting for ACK", 404);

	/*Wait for ACK*/
	result = recv(thisSession->socketFD, buffer, strlen(buffer),0);

	debugTrace("After calling rcv", 409);

	/*Confirm ACK*/
	if(confirmACK(buffer)==0)
	{
		debugTrace("Did not receive ACK for bytes to expect", 403);
	}

	/*Set bytesSent to 0*/
	bytesSent =0;


	debugTrace("before sending file contents", 421);
	/*Send MAX PACKET at a time*/
	do
	{
		/*Send MAX PACKET at a time*/
		bytesSent+=send(thisSession->socketFD, thisFile->fileBuffer, MAX_PACKET, 0);

	}while(bytesSent < thisFile->charCount);

	
	/*Clear the bufffer*/
	bzero(buffer, MAX_PACKET);

	/*Wait for received message*/
	result = recv(thisSession->socketFD, buffer, strlen(buffer), 0);

	if(confirmACK(buffer)==0)
	{
		debugTrace("did not receive ACK for file data", 437);
		thisFile->confirm =0;
	}
	else{
		debugTrace("received ACK for file data", 440);
		thisFile->confirm = 1;
	}

	/*Ensure it worked*/
	if(result < 0){
		error("Error: unable to read data from socket.\n");
	}

	

}


void handleRequest(struct session *thisSession)
{

	debugTrace("Before plainFile", 458);
	/*Send plain file first*/
	do {
		sendComms(thisSession, thisSession->plainFile);
	}
	while(thisSession->plainFile->confirm == 0);

		
	debugTrace("Before keyFile", 466);
	
	/*Send key file*/
	do{
		sendComms(thisSession, thisSession->keyFile);
	}
	while(thisSession->keyFile->confirm == 0);
		
	

	/*Get response*/


	/*Print response*/

}





void confirmReceived(const char *buff, struct fileStruct *thisFile)
{

	char* received = "ACK";
	int len = strlen(received);

	/*Compare strings*/

	if(strncmp(buff, received, len)==0){
		thisFile->confirm = 1;
	}
	else{
		thisFile->confirm =0;
	}
}

int confirmACK(const char *buff)
{
	char *success = "ACK";
	int len = strlen(success);

	if(strncmp(buff, success, len)==0){
		return 1;
	}
	return 0;
}