/******************************************************
#   CS 344 - Winter 2016 - Program 4
#   By: Carol Toro
#   File Created: 03/09/2016
#   Last Modified: 03/14/2016
#   Filename: otp_enc_d.c
#	Usage: otp_enc_d <portNumber>
#    Description: This program executes the server side request
#		from a client of a pad-like system to encrypt a file with a key.
#		The program receives the file text along with the key from the client.
#		The program encrypts the file with the key and sends to the client
#		the encrypted text.
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
#include <signal.h>

#define MAX_BUFFER 128000
#define MAX_PACKET 8000
#define MAX_NAME 50

/*Session data structure*/
typedef struct session session;
typedef struct childSession childSession;
/*text data structur*/
typedef struct textStruct textStruct;

struct textStruct{
	char *textBuffer;
	int charCount;
};

struct childSession{
	struct textStruct *plainText;
	struct textStruct *keyText;
	struct textStruct *cipherText;
	char *childPort;
	int newsocketFD;
	int newChildSocketFD;
};

struct session{
	int serverSocket; /*Server socket for listening*/
	int serverPort; /*int to store server port*/
	int socketFD; /*client socket for connection*/
	int newsocketFD;
	struct sockaddr_in serverAddr;
	struct sockaddr_in clientAddr;
	char *clientName;

};

/*Signal, Error, and Debug functions*/
void sigintHandle(int sigNum);
void debugTrace(const char *msg, int line);
void error(const char *msg);

/*Data Structure Functions*/
session* createSession();
void freeSession(struct session *thisSession);
childSession *createChildSession();
void freeChildSession(struct childSession *thisChild);
textStruct* createTextStruct();
void freeTextStruct(struct textStruct *thisText);


/*Validation Function*/
void checkCommandLine(int argcount, char *args[]);

/*Server Connections Functions*/
void startServer(struct session *thisSession);
void acceptConnection(struct session *thisSession);
void handleConnections(struct session *thisSession);

/*Process Functions*/
void handleChildProcess(struct session *thisSession, struct childSession *thisChild);

/*Server / Client Communication functions*/
void receiveHandShake(struct session *thisSession);
void getData(struct session *thisSession, struct textStruct *thisText);
void sendData(struct session *thisSession, struct textStruct *thisText);
void sendAck(struct session *thisSession);
void sendNACK(struct session *thisSession);
int confirmACK(const char *msg);

/*Encryption Function & Helper Functions*/
void encode(struct childSession *thisChild);
int charNum(char c);
char numChar(int val);
char encodeChar(char msgChar, char keyChar);

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

	/*Handle connections*/
	handleConnections(curSession);


	return 0;


}

void sigintHandle(int sigNum)
{
	pid_t pid;
	int status;
	/*Catch exiting children*/
	pid = waitpid(-1, &status, WNOHANG);
}

void debugTrace(const char *msg, int line){
	//printf("OTP_ENC_D > %s from line # %d \n", msg, line);

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
#   @param: pointer to session data structure object
#   @return: void
******************************************************/
void freeSession(struct session *thisSession)
{
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
	/*Allocate memory for textStruct vars*/
	textStruct *thisText = (textStruct*)malloc(sizeof(textStruct));
	thisText->textBuffer = (char *)malloc(sizeof(char)*MAX_BUFFER);

	return thisText;

}
/******************************************************
#   freeTextStruct
#   @desc: Deallocate the memory used up by text
#   @param: pointer to textStruc data structure object
#   @return: void
******************************************************/
void freeTextStruct(struct textStruct *thisText)
{
	free(thisText);
}

/******************************************************
#   createChildSession
#   @desc: allocate memory for child session data structure
#   @param: n/a
#   @return: pointer to childSession data structure
******************************************************/
childSession *createChildSession()
{
	childSession * thisChild = (childSession *)malloc(sizeof(childSession));
	thisChild->plainText = createTextStruct();
	thisChild->keyText = createTextStruct();
	thisChild->cipherText = createTextStruct();

	return thisChild;

}
/******************************************************
#   freeChildSession
#   @desc: Deallocate the memory used up childSession
#   @param: pointer to childSession data structure object
#   @return: void
******************************************************/
void freeChildSession(struct childSession *thisChild)
{
	freeTextStruct(thisChild->plainText);
	freeTextStruct(thisChild->keyText);
	freeTextStruct(thisChild->cipherText);

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


/******************************************************
#   Server & Connection Functions
******************************************************/

/******************************************************
#   fnName
#   @desc:
#   @param: pointer to session data structure object
#   @return: void
******************************************************/
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

/******************************************************
#   fnName
#   @desc:
#   @param: pointer to session data structure object
#   @return: void
******************************************************/
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
/******************************************************
#   handleConnections
#   @desc: accept connection, and fork new process to
#		handle data encryption.
#   @param: pointer to session data structure object
#   @return: void
******************************************************/
void handleConnections(struct session *thisSession)
{
	/*Counter for port*/
	int portCounter;
	/*Loop waiting for a connection*/
	while(1){

		/*Increment portCounter*/
		portCounter++;

		/*Set handler for children*/
		signal(SIGCHLD, sigintHandle);

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
			/*Close socket*/
			close(thisSession->socketFD);
			error("Error: unable to fork new process.\n");

		}
		/*Inside child*/
		if(childPID ==0){
			/*close the old server socket that is bound to main port*/
			close(thisSession->serverSocket);

			struct childSession *thisChild = createChildSession();

			/**/
			handleChildProcess(thisSession, thisChild);

			/*free data*/
			freeChildSession(thisChild);

		}
		else{
			/*back to parent*/
			/*Close child socket*/
			close(thisSession->socketFD);
		}

	}



}
/******************************************************
#  Process Function
******************************************************/
/******************************************************
#   handleChildProcess
#   @desc: Execute flow for receiving data from client,
#		encoding data, and sending encoded data to client.#
#   @param: pointer to session data structure object
#   @return: void
******************************************************/
void handleChildProcess(struct session *thisSession, struct childSession *thisChild)
{

	/*Confirm Handshake*/
	receiveHandShake(thisSession);

	/*Get plain data*/
	getData(thisSession, thisChild->plainText);

	/*Get key data*/
	getData(thisSession, thisChild->keyText);

	/*encode message*/
	encode(thisChild);

	/*Send message*/
	sendData(thisSession, thisChild->cipherText);

	/*Close connection*/
	close(thisSession->socketFD);


}

/******************************************************
#   Server / Client Communication Functions
******************************************************/
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
/******************************************************
#   sendNACK
#   @desc: send NACK to connected server
#   @param: pointer to session data structure
#   @return: n/a
******************************************************/
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

/******************************************************
#   getData
#   @desc: receive data from connected client and ensure
#		the entire text was sent to server.
#   @param: pointer to sesssion and textStruct data
#   @return: n/a
******************************************************/
void getData(struct session *thisSession, struct textStruct *thisText)
{
	/*Create buffers*/
	char buffer[MAX_PACKET];
	int msgLen;

	int bytesRead, result;

	/*Clear out buffers*/
	bzero(buffer, MAX_PACKET);

	/*# of bytes to expect for text*/
	bytesRead = recv(thisSession->socketFD, (char*)&msgLen, sizeof(msgLen),0);


	/*Ensure it was received*/
	if(bytesRead <0)
	{
		error("Error: unable to read from socket");
	}

	sendAck(thisSession);

	bytesRead =0;


	/*Get data*/
	while(bytesRead< msgLen){
		bytesRead+=recv(thisSession->socketFD, thisText->textBuffer, MAX_BUFFER,0);
	}
	

	/*Send ACK*/
	sendAck(thisSession);

	/*Set the text length*/
	thisText->charCount = strlen(thisText->textBuffer);

}

/******************************************************
#   receiveHandShake
#   @desc: receive initial handshake from client and confirm
#		it is the correct client attempting to connect
#   @param: pointer to session data structure
#   @return: n/a
******************************************************/
void receiveHandShake(struct session *thisSession)
{
	char buff[MAX_NAME];

	/*Clear buff*/
	bzero(buff, MAX_NAME);

	char *success = "ENC";
	int len = strlen(success);

	int bytesRead = recv(thisSession->socketFD, buff, sizeof(buff), 0);

	if (bytesRead < 0){
		error("Error: unable to read from socket.\n");
	}

	if(strncmp(buff, success, len)==0){
		/*send ACK*/
		sendAck(thisSession);
	}
	else{
		/*Wrong client connection*/
		sendNACK(thisSession);
		/*Close socket*/
		close(thisSession->socketFD);

		/*Exit child*/
		_exit(0);
	}


}
/******************************************************
#   sendData
#   @desc: send text to connected client and ensure
#		the entire text was sent to client.
#   @param: pointer to sesssion and textStruct data
#   @return: n/a
******************************************************/
void sendData(struct session *thisSession, struct textStruct *thisText)
{
	
	int result;
	int bytesSent;

	/*buffer to store response contents*/
	char buffer[MAX_PACKET];

	/*Clear the bufffer*/
	bzero(buffer, MAX_PACKET);

	/*Get size of buffer*/
	int textSize = strlen(thisText->textBuffer);

	/*Conver to netbyte order*/
	textSize = htonl(textSize);

	/*Send number of bytes to expect*/
	bytesSent = send(thisSession->socketFD, &textSize, sizeof(textSize),0);

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
	while(bytesSent < textSize){
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
#   Encryption & Helper Functions
******************************************************/

/******************************************************
#   charNum
#   @desc: calculate numerical val of ASCII char
#   @param: char c
#   @return: numerical value of ASCII char
******************************************************/
int charNum(char c)
{
	if (c == 32)
	{
		return 26;
	}

	return c - 65;

}
/******************************************************
#   charNum
#   @desc: calculate ASCII char corresponding to val
#   @param: int val
#   @return: ASCII char
******************************************************/
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
/******************************************************
#   encodeChar
#   @desc: encode msg char based on key char
#   @param: msg char key char
#   @return: encoded char
******************************************************/
char encodeChar(char msgChar, char keyChar)

{
	int msgVal = charNum(msgChar);
	int keyVal = charNum(keyChar);

	int charVal = (msgVal + keyVal) % 27;

	char c = numChar(charVal);

	return c;
}

/******************************************************
#   encode
#   @desc: encode plain text  and store as ciphertext
#   @param: pointer to childSession data struc
#   @return: encoded char
******************************************************/
void encode(struct childSession *thisChild)
{

	int plainLength = strlen(thisChild->plainText->textBuffer);
	int i;

	/*Loop through entire message*/
	for (i = 0; i < plainLength; i++){

		char ciphChar = encodeChar(thisChild->plainText->textBuffer[i], thisChild->keyText->textBuffer[i]);

		/*Set cipher char*/
		thisChild->cipherText->textBuffer[i] = ciphChar;
	}

	/*Done encoding*/
}
