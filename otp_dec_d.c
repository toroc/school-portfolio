/******************************************************
#   CS 344 - Winter 2016 - Program 4
#   By: Carol Toro
#   File Created: 03/09/2016
#   Last Modified: 03/09/2016
#   Filename: otp_dec_d.c
#	Usage: otp_dec_d <portNumber>
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
#include <signal.h>

#define MAX_BUFFER 128000

#define MAX_PACKET 1000
#define MAX_NAME 50

/*Session data structure*/
typedef struct session session;
typedef struct childSession childSession;

typedef struct textStruct textStruct;


struct textStruct{
	char *textBuffer;
	int charCount;
};

struct childSession{
	struct textStruct *cipherText;
	struct textStruct *keyText;
	struct textStruct *plainText;
};

struct session{
	int serverSocket; /*Server socket for listening*/
	int serverPort; /*int to store server port*/
	int socketFD; /*client socket for connection*/
	struct sockaddr_in serverAddr;
	struct sockaddr_in clientAddr;
	char *clientName;

};

void sigintHandle(int sigNum);
void debugTrace(const char *msg, int line);

/*Data Structure Functions*/
session* createSession();
void freeSession(struct session *thisSession);

textStruct* createTextStruct();
void freeTextStruct(struct textStruct *thisText);

childSession *createChildSession();
void freeChildSession(struct childSession *thisChild);

void receiveHandShake(struct session *thisSession);
int confirmACK(const char *msg);

void startServer(struct session *thisSession);
void acceptConnection(struct session *thisSession);
void handleConnections(struct session *thisSession);
void handleChildProcess(struct session *thisSession, struct childSession *thisChild);

void getData(struct session *thisSession, struct textStruct *thisText);
void sendData(struct session *thisSession, struct textStruct *thisText);

void sendAck(struct session *thisSession);
void sendNACK(struct session *thisSession);

void checkCommandLine(int argcount, char *args[]);

void error(const char *msg);

void sendCipher(struct session *thisSession);


void decode(struct childSession *thisChild);

int charNum(char c);
char numChar(int val);
char decodeChar(char msgChar, char keyChar);

int main(int argc, char *argv[])
{
	/*Set handler for children*/
	signal(SIGCHLD, sigintHandle);

	/*Check command line arguments are correct*/
	checkCommandLine(argc, argv);

	struct session *curSession = createSession();

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

	return 0;


}

void debugTrace(const char *msg, int line){
	perror("OTP_DEC_D > %s from line # %d \n", msg, line);

	fflush(stdout);
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void sigintHandle(int sigNum)
{
	pid_t pid;
	int status;

	pid = waitpid(-1, &status, WNOHANG);
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
	theSession->clientName=(char*)malloc(sizeof(char)*MAX_NAME);

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
	thisChild->cipherText = createTextStruct();
	thisChild->keyText = createTextStruct();
	thisChild->plainText = createTextStruct();

	return thisChild;

}
void freeChildSession(struct childSession *thisChild)
{
	freeTextStruct(thisChild->cipherText);
	freeTextStruct(thisChild->keyText);
	freeTextStruct(thisChild->plainText);

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



void startServer(struct session *thisSession)
{

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

	/*Listen for incoming connections*/
	result = listen(thisSession->serverSocket, 5);

	if(result < 0){
		error("Error: unable to listen for connections.\n");
	}
}

void acceptConnection(struct session *thisSession)
{
	/*Vars to store client's info at connection and accept*/
	struct sockaddr_in clientAddr;
	socklen_t clientLength = sizeof(clientAddr);
	char clientName[1024];
	char service[20];


	/*Accept incoming connection*/
	thisSession->socketFD = accept(thisSession->serverSocket, (struct sockaddr*) &clientAddr,&clientLength);

	/*Ensure it worked*/
	if(thisSession->socketFD < 0){
		error("Error: unable to accept connection.\n");
	}
	else{
		/*Get name of client*/
		getnameinfo((struct sockaddr *)&clientAddr, sizeof(clientAddr),clientName,sizeof(clientName), service, sizeof(service),0);
	}

	/*Store client address in session data structure*/
	thisSession->clientAddr = clientAddr;

}

void handleConnections(struct session *thisSession)
{
	
	socklen_t clientLength = sizeof(thisSession->clientAddr);

	/*Wait for connection*/
	thisSession->socketFD = accept(thisSession->serverSocket, (struct sockaddr*) &thisSession->clientAddr, &clientLength);

	/*Ensure it worked*/
	if (thisSession->socketFD<0)
	{
		error("Error: unable to accept connection.\n");
	}


	pid_t childPID;

	/*Fork*/
	childPID  = fork();

	/*Check for error*/
	if(childPID < 0){

		error("Error: unable to fork new process.\n");

	}

	/*Inside child*/
	if(childPID ==0){

		struct childSession *thisChild = createChildSession();

		handleChildProcess(thisSession, thisChild);

		/*free data*/
		freeChildSession(thisChild);

	}



}

void handleChildProcess(struct session *thisSession, struct childSession *thisChild)
{
	/*Confirm Handshake*/
	receiveHandShake(thisSession);

	/*Get plain data*/
	getData(thisSession, thisChild->cipherText);

	/*Get key data*/
	getData(thisSession, thisChild->keyText);

	/*decode message*/
	decode(thisChild);

	/*Send message*/
	sendData(thisSession, thisChild->plainText);


	/*Close connection*/
	close(thisSession->socketFD);


}
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

void sendNACK(struct session *thisSession)
{
	int result;
	char *msg = "NACK";

	/*Send ACK*/
	result = send(thisSession->socketFD, msg, sizeof(msg) , 0);

	if (result <0){
		error("Error: unable to send to socket\n");
	}
}


void getData(struct session *thisSession, struct textStruct *thisText)
{
	/*Create buffers*/
	char buffer[MAX_PACKET];
	int msgLen;

	int bytesRead, result;

	/*Clear out buffers*/
	bzero(buffer, MAX_PACKET);

	/*# of bytes to expect for text*/
	bytesRead = recv(thisSession->socketFD, &msgLen, sizeof(int),0);

	/*Ensure it was received*/
	if(bytesRead <0)
	{
		error("Error: unable to read from socket");
	}

	sendAck(thisSession);

	bytesRead =0;

	/*Get data*/
	do{
		bytesRead+=recv(thisSession->socketFD, thisText->textBuffer, MAX_BUFFER,0);

	} while(bytesRead < msgLen);

	/*Send ACK*/
	sendAck(thisSession);

	/*Set the text length*/
	thisText->charCount = strlen(thisText->textBuffer);

}


void receiveHandShake(struct session *thisSession)
{
	char buff[MAX_NAME];

	/*Clear buff*/
	bzero(buff, MAX_NAME);

	char *success = "DEC";
	int len = strlen(success);

	int bytesRead = recv(thisSession->socketFD, buff, sizeof(buff), 0);

	/*Print handshake received*/
	printf(buff);
	fflush(stdout);

	if (bytesRead < 0){
		error("Error: unable to read from socket.\n");
	}

	if(strncmp(buff, success, len)==0){
		/*send ACK*/
		sendAck(thisSession);
	}
	else{
		sendNACK(thisSession);
		/*Close socket*/
		close(thisSession->socketFD);

		/*Exit child*/
		_exit(0);
	}

	
}


int charNum(char c)
{
	if (c == 32)
	{
		return 26;
	}
	
	return c - 65;	

}

char numChar(int val)
{
	int i;
	char c;

	if (val <26)
	{
		i = val + 65;
		c = i;
		
	}
	else
	{
		/*Space character*/
		i = 32;
		c = i;	
	}

	return c;
}




char decodeChar(char msgChar, char keyChar)

{
	int msgVal = charNum(msgChar);
	int keyVal = charNum(keyChar);

	int charVal = (msgVal - keyVal) % 27;

	/*Check if charVal is negative*/
	if (charVal < 0){
		charVal += 27;
	}

	/*Get numerical ASCII value of charVal*/
	char c = numChar(charVal);

	return c;
}

void decode(struct childSession *thisChild)
{

	int plainLength = strlen(thisChild->cipherText->textBuffer);
	int i;

	for (i = 0; i < plainLength; i++){

		char ciphChar = decodeChar(thisChild->cipherText->textBuffer[i], thisChild->keyText->textBuffer[i]);

		/*Set cipher char*/
		thisChild->plainText->textBuffer[i] = ciphChar;
	}

	/*Done encoding*/
}

void sendData(struct session *thisSession, struct textStruct *thisText)
{
	int result;
	int bytesSent;

	/*buffer to store response contents*/
	char buffer[MAX_PACKET];

	/*Clear the bufffer*/
	bzero(buffer, MAX_PACKET);

	int val = thisText->charCount;

	/*Send number of bytes to expect*/
	bytesSent = send(thisSession->socketFD, &val, sizeof(int),0);

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
		//debugTrace("did not receive ACK for file data", 437);
		
	}
	else{
		//debugTrace("received ACK for file data", 440);
		
	}

	/*Ensure it worked*/
	if(result < 0){
		error("Error: unable to read data from socket.\n");
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