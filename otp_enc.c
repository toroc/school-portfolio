/******************************************************
#   CS 344 - Winter 2016 - Program 4
#   By: Carol Toro
#   File Created: 03/09/2016
#   Last Modified: 03/14/2016
#   Filename: otp_enc.c
#	Usage: otp_enc <plainText> <keyText> <portNumber>
#   Description: This program executes the client side request
#		of a pad-like system to encrypt a file with a key. 
#		The program verifies that the files contain valid characters, 
#		and sends the file along with key to server for encryption.
#		The program receives the encrypted text and outputs it.
#	References: 
#	Beej's Guide
#		http://beej.us/guide/bgnet/output/html/multipage/syscalls.html#sendrecv
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
#define MAX_PACKET 8000
#define MAX_NAME 50

typedef enum{INVALID=0, VALID}CTYPE;
/*Session data structure*/
typedef struct session session;
/*text data strcture*/
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
	
	int socketFD; /*client socket endpoint file descriptor*/
	struct sockaddr_in serverAddr;
	struct hostent *serverIP;
};

/*Data Structure Functions*/
textStruct* createTextStruct();
void freeTextStruct(struct textStruct *thisText);
session* createSession();
void freeSession(struct session *thisSession);

void error(const char *msg);
void debugTrace(const char *msg, int line);

/*Validation Functions*/
void checkCommandLine(int argcount, char *args[]);
void validateFiles(struct session *thisSession);

/*File and Key validation helper functions*/
void fileCharValidation(struct textStruct *thisText);
int validChar(char curChar);


void startClient(struct session *thisSession);
void handleRequest(struct session *thisSession);

/*Client / Server Communication functions*/
void sendHandShake(struct session *thisSession);
void sendComms(struct session *thisSession, struct textStruct *thisText);
void getData(struct session *thisSession, struct textStruct *thisText);
void sendAck(struct session *thisSession);
int confirmACK(const char *msg);

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

/******************************************************
#   Data Structure Functions
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
#   @desc: Deallocate the memory used up by session
#   @param: pointer to session data structure object
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
#   createTextStruct
#   @desc: allocate memory for  data structure
#       and return pointer to textStruct object
#   @param: n/a
#   @return: pointer to textStruct *thisTextStruct object
******************************************************/
textStruct* createTextStruct()
{
	/*Allocate memory for file struct vars*/
	textStruct *thisTextStruct = (textStruct*)malloc(sizeof(textStruct));
	thisTextStruct->fileName=(char*)malloc(sizeof(char)*MAX_NAME);
	thisTextStruct->charCount=0;
	thisTextStruct->confirm=0;

	return thisTextStruct;
}
/******************************************************
#   freeTextStruct
#   @desc: Deallocate the memory used up by text
#   @param: pointer to textStruc data structure object
#   @return: void
******************************************************/
void freeTextStruct(struct textStruct *thisText){
	free(thisText);
}

/*Provided by assignment*/
void error(const char *msg)
{
    perror(msg);
    exit(1);
}
/*Used for debugging*/
void debugTrace(const char *msg, int line){
	//printf("OTP_ENC > %s from line # %d \n", msg, line);

	//fflush(stdout);
}

/******************************************************
#   Validation Functions
******************************************************/
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
		fflush(stdout);
		exit(2);
	}
}

/******************************************************
#   validateFiles
#   @desc:  exit with error if files contain invalid chars
#   @param: pointer to session data structure
#   @return: n/a
******************************************************/
void validateFiles(struct session *thisSession)
{

	/*Validate for bad characters*/
	fileCharValidation(thisSession->plainText);
	fileCharValidation(thisSession->keyText);

	/*exit with error if files have invalid chars*/
	if(thisSession->plainText->validChars == INVALID){
		fprintf(stderr,"Error: invalid characters in file \'%s\'\n", thisSession->plainText->fileName);
		exit(1);
	}

	if(thisSession->keyText->validChars == INVALID){
		fprintf(stderr, "Error: invalid characters in file \'%s\'\n", thisSession->keyText->fileName);
		exit(1);
	}

	/*exit with error: if keyText is shorter than plain file*/
	if(thisSession->plainText->charCount > thisSession->keyText->charCount){
		fprintf(stderr, "Error: key \'%s\' is too short\n", thisSession->keyText->fileName);
		exit(1);
	}
}

/******************************************************
#   fileCharValidation
#   @desc: Execute flow for storing file contents, validating 
#		for bad chars in, setting char count of files,
#		and removing newline character
#   @param: pointer to textStruct data structure
#   @return: n/a
******************************************************/
void fileCharValidation(struct textStruct *thisText)
{
	/*File descriptor for reading file*/
	FILE *fileFD;
	int charCount=0;
	int result;

	/*open file for read*/
	fileFD=fopen(thisText->fileName,"r");

	if(fileFD==0)
	{
		perror("File failed to open");
	}

	/*Save file contents in text buffer*/
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
	return 0;
}


/******************************************************
#   startClient
#   @desc: Set up client connection to server
#   @param: pointer to session data structure
#   @return: 
******************************************************/
void startClient(struct session *thisSession)
{

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


	/*Connect to server*/
	result = connect(thisSession->socketFD, (struct sockaddr *) &thisSession->serverAddr, sizeof(thisSession->serverAddr));

	/*Ensure it worked*/
	if (result < 0){
		error("Error: unable to connect to server.\n");
	}

}
/******************************************************
#   handleRequest
#   @desc: Execute flow for sending initial handshake, 
#		sending	plain text, sending key text, 
#		receiving cipher text and printing it.
#   @param: pointer to session data structure
#   @return: n/a
******************************************************/
void handleRequest(struct session *thisSession)
{
	/*Take care of handshake*/
	sendHandShake(thisSession);

	/*Send plain file first*/
	do {
		sendComms(thisSession, thisSession->plainText);
	}
	while(thisSession->plainText->confirm == 0);

	/*Send key file*/
	do{
		sendComms(thisSession, thisSession->keyText);
	}
	while(thisSession->keyText->confirm == 0);
		
	/*Get response*/
	getData(thisSession, thisSession->cipherText);

	/*Print response*/
	printf("%s\n",thisSession->cipherText->textBuffer);

}

/******************************************************
#   Client / Server Communication Functions
******************************************************/

/******************************************************
#   sendComms
#   @desc: send text to connected server and ensure
#		the entire text was sent to server.
#   @param: pointer to sesssion and textStruct data
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

	/*Get size of buffer*/
	long textVal = sizeof(thisText->textBuffer);

	/*Send number of bytes to expect*/
	bytesSent = send(thisSession->socketFD, &textVal, sizeof(long),0);

	/*Ensure message sent*/
	if(bytesSent <0){
		error("No bytes sent");
	}

	/*Wait for ACK*/
	result = recv(thisSession->socketFD, buffer, sizeof(buffer),0);

	/*Confirm ACK*/
	if(confirmACK(buffer)==0)
	{
		//debugTrace("Did not receive ACK for bytes to expect", 403);
	}

	/*Set bytesSent to 0*/
	bytesSent =0;


	/*Send MAX PACKET at a time*/
	while(bytesSent < textVal){
		/*Send MAX PACKET at a time*/
		bytesSent+=send(thisSession->socketFD, thisText->textBuffer, MAX_PACKET, 0);
	}
		
	
	/*Clear the bufffer*/
	bzero(buffer, MAX_PACKET);

	/*Wait for received message*/
	result = recv(thisSession->socketFD, buffer, sizeof(buffer), 0);

	/*Ensure it worked*/
	if(result < 0){
		error("Error: unable to read data from socket.\n");
	}

	/*Confirm received entire file*/
	if(confirmACK(buffer)==0)
	{
		thisText->confirm =0;
	}
	else{
		thisText->confirm = 1;
	}

	
}

/******************************************************
#   sendHandShake
#   @desc: send initial handshake to server and confirm
#		it was unable to connect to otp_dec_d
#   @param: pointer to session data structure
#   @return: n/a
******************************************************/
void sendHandShake(struct session *thisSession)
{
	char buff[MAX_NAME];

	/*clear buffer*/
	bzero(buff, MAX_NAME);

	char *msg ="ENC";

	/*Send Handshake*/
	int result = send(thisSession->socketFD, msg, sizeof(msg),0);

	if (result <0)
	{
		error("Error: unable to send handshake.\n");
	}

	/*Confirm response*/
	result = recv(thisSession->socketFD, buff, sizeof(buff), 0);

	if (result < 0)
	{
		error("Error: unable to read from socket.\n");
	}


	if(confirmACK(buff)==0){
		printf("Error: could not contact otp_dec_d on port %d.\n", thisSession->serverPort);
		exit(2);
	}
}


/******************************************************
#   confirmACK
#   @desc: confirm buff has an ACK msg
#   @param: const char *buff
#   @return: 0: false, 1:true
******************************************************/
int confirmACK(const char *buff)
{
	char *success = "ACK";
	int len = strlen(success);

	if(strncmp(buff, success, len)==0){
		return 1;
	}
	return 0;
}
/******************************************************
#   getData
#   @desc: receive data containing ciphertext from
#		connected server
#   @param: pointer to session and textStruct data
#   @return: n/a
******************************************************/
void getData(struct session *thisSession, struct textStruct *thisText)
{
	/*Create buffers*/
	char buffer[MAX_PACKET];
	long msgLen;

	int bytesRead, result;

	/*Clear out buffers*/
	bzero(buffer, MAX_PACKET);

	/*# of bytes to expect for text*/
	bytesRead = recv(thisSession->socketFD, &msgLen, sizeof(long),0);


	/*Ensure it was received*/
	if(bytesRead <0)
	{
		error("Error: unable to read from socket");
	}

	sendAck(thisSession);

	bytesRead =0;


	/*Get data*/
	while(bytesRead< msgLen)
		bytesRead+=recv(thisSession->socketFD, thisText->textBuffer, MAX_BUFFER,0);
	} 

	/*Send ACK*/
	sendAck(thisSession);

	/*Set the text length*/
	thisText->charCount = strlen(thisText->textBuffer);

}
/******************************************************
#   sendAck
#   @desc: send ACK to connected server
#   @param: pointer to session data structure
#   @return: n/a
******************************************************/
void sendAck(struct session *thisSession)
{
	int result;
	char *msg = "ACK";

	/*Send ACK*/
	result = send(thisSession->socketFD, msg, sizeof(msg) , 0);

	if (result <0){
		error("Error: unable to send to socket\n");
	}
}