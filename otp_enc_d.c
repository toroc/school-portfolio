/******************************************************
#   CS 344 - Winter 2016 - Program 4
#   By: Carol Toro
#   File Created: 03/09/2016
#   Last Modified: 03/09/2016
#   Filename: otp_enc_d.c
#	Usage: otp_enc_d <portNumber>
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

/*Session data structure*/
typedef struct serverSession serverSession;
typedef struct childSession childSession;

typedef struct textStruct textStruct;


struct textStruct{
	char *textBuffer;
	int charCount;
};

struct childSession{
	struct textStruct *plainTxt;
	struct textStruct *keyTxt;
	struct textStruct *cipherTxt;
};

struct serverSession{
	int serverSocket; /*Server socket for listening*/
	int serverPort; /*int to store server port*/
	int clientSocket; /*client socket for connection*/
	struct sockaddr_in serverAddr;
	struct sockaddr_in clientAddr;
	char *clientName;

};

void debugTrace(const char *msg, int line);

/*Data Structure Functions*/
serverSession* createSession();
void freeSession(struct serverSession *thisSession);

textStruct* createTextStruct();
void freeTextStruct(struct textStruct *thisText);

childSession *createChildSession();
void freeChildSession(struct childSession *thisChild);

void receiveHandShake(struct serverSession *thisSession);


void startServer(struct serverSession *thisSession);
void acceptConnection(struct serverSession *thisSession);
void handleConnections(struct serverSession *thisSession);
void handleChildProcess(struct serverSession *thisSession, struct childSession *thisChild);

void getData(struct serverSession *thisSession, struct textStruct *thisText);

void sendAck(struct serverSession *thisSession);
void sendNACK(struct serverSession *thisSession);

void checkCommandLine(int argcount, char *args[]);

void error(const char *msg);

void sendCipher(struct serverSession *thisSession);


void encode(char *plain, char *key, char *cipher);

int main(int argc, char *argv[])
{
	/*Check command line arguments are correct*/
	checkCommandLine(argc, argv);

	struct serverSession *curSession = createSession();

	/*Store port #*/
	curSession->serverPort=atoi(argv[1]);

	/*Setup server and listen for connections*/
	startServer(curSession);

	/**/
	while(1)
	{
		/*Handle connections*/
		handleConnections(curSession);
	}

	/**/



	return 0;





}

void debugTrace(const char *msg, int line){
	printf("OTP_ENC_D > %s from line # %d \n", msg, line);

	fflush(stdout);
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
#       and return pointer to serverSession object
#   @param: n/a
#   @return: pointer to serverSession *theSession object
******************************************************/
serverSession* createSession(){

	/*Allocate memory for the serverSession variables*/
	serverSession *theSession = (serverSession*)malloc(sizeof(serverSession));
	theSession->clientName=(char*)malloc(sizeof(char)*MAX_NAME);

	return theSession;
}

/******************************************************
#   freeSession
#   @desc: Deallocate the memory used up by user
#   @param: pointer to serverSession datat structure object
#   @return: void
******************************************************/
void freeSession(struct serverSession *thisSession)
{
	free(thisSession);

}


textStruct* createTextStruct()
{
	/*Allocate memory for textStruct vars*/
	textStruct *thisText = (textStruct*)malloc(sizeof(textStruct));
	thisText->textBuffer = (char *)malloc(sizeof(char)*MAX_BUFFER);

	return thisText;

}
void freeTextStruct(struct textStruct *thisText)
{
	free(thisText);
}

childSession *createChildSession()
{
	childSession * thisChild = (childSession *)malloc(sizeof(childSession));
	thisChild->plainTxt = createTextStruct();
	thisChild->keyTxt = createTextStruct();
	thisChild->cipherTxt = createTextStruct();

	return thisChild;

}
void freeChildSession(struct childSession *thisChild)
{
	freeTextStruct(thisChild->plainTxt);
	freeTextStruct(thisChild->keyTxt);
	freeTextStruct(thisChild->cipherTxt);

	free(thisChild);
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
	if(argcount != 2){
		printf("Usage: %s <portNumber>\n",args[0]);
		exit(1);
	}
}



void startServer(struct serverSession *thisSession)
{

	debugTrace("begin startServer", 171);

	int optVal =1;
	/*Create server socket*/
	thisSession->serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	/*Ensure it worked*/
	if (thisSession->serverSocket < 0){
		error("Error: unable to open socket.\n");
	}

	/*setup server socket*/
	thisSession->serverAddr.sin_family = AF_INET;	/*Protocol Family*/
	thisSession->serverAddr.sin_port = htons(thisSession->serverPort); /*port # to use*/
	thisSession->serverAddr.sin_addr.s_addr= INADDR_ANY; /*Local address*/


	/*Allow reuse of address*/
	int result = setsockopt(thisSession->serverSocket, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal));

	/*Ensure it worked*/
	if (result < 0){
		error("Error: unable to setup SO_REUSEADDR.\n");
	}


	/*Bind server socket*/
	result = bind(thisSession->serverSocket, (struct sockaddr *)&thisSession->serverAddr, sizeof(thisSession->serverAddr));

	/*Ensure it worked*/
	if (result < 0){
		error("Error: unable to bind to host address.\n");
	}

	debugTrace("before listen ", 205);
	/*Listen for incoming connections*/
	result = listen(thisSession->serverSocket, 5);

	if(result < 0){
		error("Error: unable to listen for connections.\n");
	}
}

void acceptConnection(struct serverSession *thisSession)
{
	debugTrace("before accept connection ", 212);

	/*Vars to store client's info at connection and accept*/
	struct sockaddr_in clientAddr;
	socklen_t clientLength = sizeof(clientAddr);
	char clientName[1024];
	char service[20];


	/*Accept incoming connection*/
	thisSession->clientSocket = accept(thisSession->serverSocket, (struct sockaddr*) &clientAddr,&clientLength);

	/*Ensure it worked*/
	if(thisSession->clientSocket < 0){
		error("Error: unable to accept connection.\n");
	}
	else{
		/*Get name of client*/
		getnameinfo((struct sockaddr *)&clientAddr, sizeof(clientAddr),clientName,sizeof(clientName), service, sizeof(service),0);
	}

	debugTrace("After accept connection", 224);

	/*Store client address in session data structure*/
	thisSession->clientAddr = clientAddr;

}

void handleConnections(struct serverSession *thisSession)
{
	
	socklen_t clientLength = sizeof(thisSession->clientAddr);

	/*Wait for connection*/
	thisSession->clientSocket = accept(thisSession->serverSocket, (struct sockaddr*) &thisSession->clientAddr, &clientLength);

	/*Ensure it worked*/
	if (thisSession->clientSocket<0)
	{
		error("Error: unable to accept connection.\n");
	}


	pid_t childPID;

	debugTrace("before fork,", 312);
	/*Fork*/
	childPID  = fork();

	/*Check for error*/
	if(childPID < 0){

		error("Error: unable to fork new process.\n");

	}

	/*Inside child*/
	if(childPID ==0){

		debugTrace("inside child ", 325);

		struct childSession *thisChild = createChildSession();

		handleChildProcess(thisSession, thisChild);
		
		
	}

}

void handleChildProcess(struct serverSession *thisSession, struct childSession *thisChild)
{
	debugTrace("handleChildProcess", 344);

	/*Confirm Handshake*/
	receiveHandShake(thisSession);

	/*Get plain data*/
	debugTrace("before sending for plain data", 350);
	getData(thisSession, thisChild->plainTxt);

	debugTrace("before sending for key data", 353);
	
	/*Get key data*/
	getData(thisSession, thisChild->keyTxt);




	/*encode message*/
}
void sendAck(struct serverSession *thisSession)
{
	debugTrace("inside sendAck", 365);
	int result;
	char *msg = "ACK";


	/*Send ACK*/
	result = send(thisSession->clientSocket, msg, sizeof(msg) , 0);

	debugTrace("After trying to send ACK", 373);

	if (result <0){
		error("Error: unable to send to socket\n");
	}
}

void sendNACK(struct serverSession *thisSession)
{
	debugTrace("inside sendAck", 382);
	int result;
	char *msg = "NACK";


	/*Send ACK*/
	result = send(thisSession->clientSocket, msg, sizeof(msg) , 0);

	debugTrace("After trying to send NACK", 336);

	if (result <0){
		error("Error: unable to send to socket\n");
	}
}


void getData(struct serverSession *thisSession, struct textStruct *thisText)
{
	debugTrace("getData ", 341);
	/*Create buffers*/
	char buffer[MAX_PACKET];
	int msgLen;

	int bytesRead, result;

	/*Clear out buffers*/
	bzero(buffer, MAX_PACKET);

	debugTrace("before calling recv", 350);
	/*# of bytes to expect for text*/
	bytesRead = recv(thisSession->clientSocket, &msgLen, sizeof(int),0);

	debugTrace("after calling recv", 354);

	/*Ensure it was received*/
	if(bytesRead <0)
	{
		error("Error: unable to read from socket");
	}

	debugTrace("before calling sendAck for data length", 362);
	sendAck(thisSession);

	debugTrace("After calling sendAck for data length", 362);

	bytesRead =0;



	/*Get data*/
	do{
		debugTrace("before trying to get file data", 378);
		bytesRead+=recv(thisSession->clientSocket, thisText->textBuffer, MAX_BUFFER,0);

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


void receiveHandShake(struct serverSession *thisSession)
{
	char buff[MAX_NAME];

	/*Clear buff*/
	bzero(buff, MAX_NAME);

	char *success = "ENC";
	int len = strlen(success);

	int bytesRead = recv(thisSession->clientSocket, buff, sizeof(buff), 0);

	/*Print handshake received*/
	printf(buff);
	fflush(stdout);

	if (bytesRead < 0){
		error("Error: unable to read from socket.\n");
	}

	if(strncmp(buff, success, len)==0){
		/*send ACK*/
		sendAck(thisSession);
		debugTrace("handshake sucess", 473);
	}
	else{
		debugTrace("sending NACK", 475);
		sendNACK(thisSession);
		/*Close socket*/
		close(thisSession->clientSocket);

		/*Exit child*/
		_exit(0);
	}

	
}
void encode(char *plain, char *key, char *cipher);
