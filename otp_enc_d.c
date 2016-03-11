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

};

void debugTrace(const char *msg, int line);

/*Data Structure Functions*/
serverSession* createSession();
void freeSession(struct serverSession *thisSession);

textStruct* createTextStruct();
void freeTextStruct(struct textStruct *thisText);

childSession *createChildSession();
void freeChildSession(struct childSession *thisChild);


void startServer(struct serverSession *thisSession);
void acceptConnection(struct serverSession *thisSession);
void handleConnection(struct serverSession *thisSession);
void handleChildProcess(struct serverSession *thisSession, struct childSession *thisChild);

void getData(struct serverSession *thisSession, struct textStruct *thisText);

void sendAck(struct serverSession *thisSession);

void checkCommandLine(int argcount, char *args[]);

void error(const char *msg);
void receiveFile(struct serverSession *thisSession);
void receiveKey(struct serverSession *thisSession);
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

		/*Start connection*/
		acceptConnection(curSession);

		/*Handle request*/
		handleConnection(curSession);
	}

	/**/



	return 0;





}

void debugTrace(const char *msg, int line){
	printf("%s from line # %d \n", msg, line);
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
	childSession * thisChildSession = (childSession *)malloc(sizeof(childSession));
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
	thisSession->serverAddr.sin_family = AF_INET;
	thisSession->serverAddr.sin_port = htons(thisSession->serverPort);
	thisSession->serverAddr.sin_addr.s_addr= INADDR_ANY;


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
	listen(thisSession->serverSocket, 5);
}

void acceptConnection(struct serverSession *thisSession)
{
	debugTrace("before accept connection ", 212);

	socklen_t clientLength = sizeof(thisSession->clientAddr);

	/*Accept incoming connection*/
	thisSession->clientSocket = accept(thisSession->serverSocket, (struct sockaddr*) &thisSession->clientAddr, &clientLength);

	/*Ensure it worked*/
	if(thisSession->clientSocket < 0){
		error("Error: unable to accept connection.\n");
	}

	debugTrace("After accept connection", 224);


}

void handleConnection(struct serverSession *thisSession)
{
	pid_t childPID;

	debugTrace("before fork,", 232);
	/*Fork*/
	childPID  = fork();

	/*Check for error*/
	if(childPID < 0){
		error("Error: unable to fork new process.\n");

	}

	/*Inside child*/
	if(childPID ==0){

		debugTrace("inside child " 244);

		struct childSession *thisChild = createChildSession();

		handleChildProcess(thisSession);
		
		

		
	}

}

void handleChildProcess(struct serverSession *thisSession, struct childSession *thisChild)
{
	
	/*Get plain data*/
	getData(thisSession, thisChild->plainTxt);


	/*Get key data*/
	getData(thisSession, thisChild->keyTxt);




	/*encode message*/
}
void sendAck(struct serverSession *thisSession)
{
	int result;

	/*Send ACK*/
	result = send(thisSession->clientSocket, "ACK", 3, 0);

	if (result != 3){
		error("Error: unable to send to socket");
	}
}

void getData(struct serverSession *thisSession, struct textStruct *thisText)
{
	/*Create buffers*/
	char buffer[MAX_PACKET];
	int msgLen;

	int bytesRead, result;

	/*Clear out buffers*/

	/*# of bytes to expect for text*/
	bytesRead = recv(thisSession->clientSocket, buffer, 1,0);

	/*Ensure it was received*/
	if(bytesRead != 1)
	{
		error("Error: unable to read from socket");
	}

	/*Save length of message*/
	msgLen = atoi(buffer);

	sendAck(thisSession);

	bytesRead =0;

	/*Get data*/
	do{
		bytesRead+=recv(thisSession->clientSocket, thisText->textBuffer, MAX_BUFFER,0)
	} while(bytesRead < msgLen);
	
	/*Send ACK*/
	sendAck(thisSession);

	thisText->charCount = bytesRead;
}


void encode(char *plain, char *key, char *cipher);
