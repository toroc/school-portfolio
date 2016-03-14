/******************************************************
#   CS 344 - Winter 2016 - Program 4
#   By: Carol Toro
#   File Created: 03/09/2016
#   Last Modified: 03/09/2016
#   Filename: otp_enc.c
#	Usage: otp_enc <plainText> <keyText> <portNumber>
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

typedef struct textStruct textStruct;

struct textStruct{
	char *fileName;
	char textBuffer[MAX_BUFFER];
	int charCount;
	CTYPE validChars;
	int confirm; /*0: false, 1: true*/
};

struct session{
	struct textStruct *plainText;
	struct textStruct *keyText;
	struct textStruct *cipherText;
	int serverPort; /*int to store server port*/
	
	int dataSocket;
	int socketFD; /*client socket endpoint file descriptor*/
	int serverFD; /*Socket file descriptor*/
	struct sockaddr_in serverAddr;
	struct hostent *serverIP;
};

/*Data Structure Functions*/
textStruct* createTextStruct();
void freeTextStruct(struct textStruct *thisText);
session* createSession();
void freeSession(struct session *thisSession);

void checkCommandLine(int argcount, char *args[]);
void sendAck(struct session *thisSession);


void validateFiles(struct session *thisSession);

/*File and Key validation helper functions*/
void fileCharValidation(struct textStruct *thisText);
int validChar(char curChar);


void error(const char *msg);
void debugTrace(const char *msg, int line);

void startClient(struct session *thisSession);
void handleRequest(struct session *thisSession);

void sendHandShake(struct session *thisSession);
void sendComms(struct session *thisSession, struct textStruct *thisText);


void receiveCipher(struct session *thisSession);

int confirmACK(const char *msg);

void confirmReceived(const char *msg, struct textStruct *thisText);

int main(int argc, char *argv[])
{
	/*Check command line arguments are correct*/
	checkCommandLine(argc, argv);

	struct session *curSession = createSession();

	/*save commands*/
	curSession->plainText->fileName=argv[1];
	curSession->keyText->fileName=argv[2];
	curSession->serverPort=atoi(argv[3]);

	/**/
	validateFiles(curSession);

	/*Start client connection*/
	startClient(curSession);

	/*take care of request*/
	handleRequest(curSession);


	/*Close socket*/
	close(curSession->socketFD);
	/*Deallocate memory*/
	freeSession(curSession);

	return 0;


}



void error(const char *msg)
{
    perror(msg);
    exit(1);
}

textStruct* createTextStruct()
{
	/*Allocate memory for file struct vars*/
	textStruct *thisFileStruct = (textStruct*)malloc(sizeof(textStruct));
	thisFileStruct->fileName=(char*)malloc(sizeof(char)*MAX_NAME);
	thisFileStruct->charCount=0;
	thisFileStruct->confirm=0;

	return thisFileStruct;
}
void freeTextStruct(struct textStruct *thisText){
	free(thisText);
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
	theSession->plainText=createTextStruct();
	theSession->keyText=createTextStruct();
	theSession->cipherText=createTextStruct();

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
	freeTextStruct(thisSession->plainText);
	freeTextStruct(thisSession->keyText);
	freeTextStruct(thisSession->cipherText);

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
		printf("Usage: %s <plainText> <keyText> <portNumber>\n",args[0]);
		exit(2);
	}
}


void debugTrace(const char *msg, int line){
	printf("OTP_ENC > %s from line # %d \n", msg, line);

	fflush(stdout);
}

/******************************************************
#   	funcName
#   @desc:
#
#   @param:
#
#   @return: n/a
******************************************************/

void fileCharValidation(struct textStruct *thisText)
{
	printf("char & count validation \n");
	/*File descriptor for reading file*/
	FILE *fileFD;
	int charCount=0;
	int result;

	/*open file for read*/
	fileFD=fopen(thisText->fileName,"r");

	if(fileFD==0)
	{
		printf("File failed to open");
	}

	/*Save file contents in file buffer*/
	fgets(thisText->textBuffer, sizeof(thisText->textBuffer), fileFD);

	charCount=strlen(thisText->textBuffer);

	/*close file descriptor*/
	fclose(fileFD);

	/*Loop through char buffer & validate chars*/
	int i;
	char c;
	for (i=0; i < charCount - 1; i++)
	{
		c = thisText->textBuffer[i];

		/*Found invalid char*/
		if(validChar(c)==0)
		{
			debugTrace("Invalid char", 230);
			/*Set to INVALID*/
			thisText->validChars=INVALID;
			/*Exit function*/
			return;
		}
	}
	
	/*Set char count and whether valid chars*/
	thisText->charCount=charCount-1;
	thisText->validChars=VALID;

	/*Get rid of the newline character*/
	thisText->textBuffer[charCount-1]='\0';
	

	printf("%s: count %d\n", thisText->fileName, charCount);



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
	fileCharValidation(thisSession->plainText);
	fileCharValidation(thisSession->keyText);

	/*exit with error if files have invalid chars*/
	if(thisSession->plainText->validChars == INVALID){
		fprintf(stderr,"Error: invalid characters in file \'%s\'", thisSession->plainText->fileName);
		exit(1);
	}

	if(thisSession->keyText->validChars == INVALID){
		fprintf(stderr, "Error: invalid characters in file \'%s\'", thisSession->keyText->fileName);
		exit(1);
	}

	/*exit with error: if keyText is shorter than plain file*/
	if(thisSession->plainText->charCount > thisSession->keyText->charCount){
		fprintf(stderr, "Error: key \'%s\' is too short\n", thisSession->keyText->fileName);
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
void sendComms(struct session *thisSession, struct textStruct *thisText)
{
	
	int result;
	int bytesSent;

	/*buffer to store response contents*/
	char buffer[MAX_PACKET];

	/*Clear the bufffer*/
	bzero(buffer, MAX_PACKET);

	int val = thisText->charCount;

	debugTrace("Before sending expected bytes", 394);

	/*Send number of bytes to expect*/
	bytesSent = send(thisSession->socketFD, &val, sizeof(int),0);

	/*Ensure message sent*/
	if(bytesSent <0){
		error("No bytes sent");
	}

	debugTrace("Before waiting for ACK", 404);

	/*Wait for ACK*/
	result = recv(thisSession->socketFD, buffer, sizeof(buffer),0);

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
		bytesSent+=send(thisSession->socketFD, thisText->textBuffer, MAX_PACKET, 0);

	}while(bytesSent < thisText->charCount);

	
	/*Clear the bufffer*/
	bzero(buffer, MAX_PACKET);

	/*Wait for received message*/
	result = recv(thisSession->socketFD, buffer, sizeof(buffer), 0);

	if(confirmACK(buffer)==0)
	{
		debugTrace("did not receive ACK for file data", 437);
		thisText->confirm =0;
	}
	else{
		debugTrace("received ACK for file data", 440);
		thisText->confirm = 1;
	}

	/*Ensure it worked*/
	if(result < 0){
		error("Error: unable to read data from socket.\n");
	}

	

}


void handleRequest(struct session *thisSession)
{


	/*Take care of handshake*/
	sendHandShake(thisSession);

	debugTrace("Before plainText", 458);
	/*Send plain file first*/
	do {
		sendComms(thisSession, thisSession->plainText);
	}
	while(thisSession->plainText->confirm == 0);

		
	debugTrace("Before keyText", 466);
	
	/*Send key file*/
	do{
		sendComms(thisSession, thisSession->keyText);
	}
	while(thisSession->keyText->confirm == 0);
		
	

	/*Get response*/
	getData(thisSession, thisSession->cipherText);


	/*Print response*/

	printf(thisSession->cipherText->textBuffer);

}


void sendHandShake(struct session *thisSession)
{
	char buff[MAX_NAME];

	/*clear buffer*/
	bzero(buff, MAX_NAME);

	char *msg ="ENC";

	debugTrace("before sending handshake", 494);
	/*Send Handshake*/

	int result = send(thisSession->socketFD, msg, sizeof(msg),0);

	if (result <0)
	{
		error("Error: unable to send handshake.\n");
	}

	debugTrace("after sending handshake", 503);


	/*Confirm response*/
	result = recv(thisSession->socketFD, buff, sizeof(buff), 0);

	if (result < 0)
	{
		error("Error: unable to read from socket.\n");
	}

	debugTrace("After receiving handshake confirmation", 517);

	if(confirmACK(buff)==0){
		printf("Could not contact otp_dec_d on port %d.\n", thisSession->serverPort);
		exit(2);
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

void getData(struct session *thisSession, struct textStruct *thisText)
{
	debugTrace("getData ", 560);
	/*Create buffers*/
	char buffer[MAX_PACKET];
	int msgLen;

	int bytesRead, result;

	/*Clear out buffers*/
	bzero(buffer, MAX_PACKET);

	debugTrace("before calling recv", 350);
	/*# of bytes to expect for text*/
	bytesRead = recv(thisSession->socketFD, &msgLen, sizeof(int),0);

	debugTrace("after calling recv", 354);

	/*Ensure it was received*/
	if(bytesRead <0)
	{
		error("Error: unable to read from socket");
	}

	debugTrace("before calling sendAck for data length", 578);
	sendAck(thisSession);

	debugTrace("After calling sendAck for data length", 581);

	bytesRead =0;



	/*Get data*/
	do{
		debugTrace("before trying to get file data", 589);
		bytesRead+=recv(thisSession->socketFD, thisText->textBuffer, MAX_BUFFER,0);

		debugTrace("print bytes read line 381", bytesRead);

	} while(bytesRead < msgLen);

	/*Print data received*/

	debugTrace("Before trying to print buffer", 402);
	debugTrace(thisText->textBuffer, 380);

	debugTrace("before calling ACK after data received", 382);
	
	/*Send ACK*/
	sendAck(thisSession);

	/*Set the text length*/
	thisText->charCount = strlen(thisText->textBuffer);

	/**/
	debugTrace("Done with getData fn", 413);
}

void sendAck(struct session *thisSession)
{
	debugTrace("inside sendAck", 365);
	int result;
	char *msg = "ACK";


	/*Send ACK*/
	result = send(thisSession->socketFD, msg, sizeof(msg) , 0);

	debugTrace("After trying to send ACK", 373);

	if (result <0){
		error("Error: unable to send to socket\n");
	}
}