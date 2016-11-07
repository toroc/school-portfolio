/******************************************************
#   CS 344 - Winter 2016 - Program 4
#   By: Carol Toro
#   File Created: 03/09/2016
#   Last Modified: 10/17/2016
#   Filename: server_side.h
#    Description: This server side OTP system definitions.
#	References:
#	Beej's Guide
#		http://beej.us/guide/bgnet/output/html/multipage/syscalls.html#sendrecv
******************************************************/
#ifndef SERVER_SIDE_H
#define SERVER_SIDE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include "../inc/encryption_decryption.h"
#include "../inc/helpers.h"

#define MAX_BUFFER 128000
#define MAX_PACKET 8000
#define MAX_NAME 50

typedef enum{ENC=0, DEC}CONNECTION;

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
	// Added based on otp_enc_d
	char *childPort;
	int clientSocketFD;
	int newChildSocketFD;


};

struct session{
	int serverSocket; /*Server socket for listening*/
	int serverPort; /*int to store server port*/
	int childSocket; /*client socket for connection*/
	int newsocketFD;
	struct sockaddr_in serverAddr;
	struct sockaddr_in clientAddr;
	char *clientName;
	CONNECTION connType;


};



/*Data Structure Functions*/
session* createSession();
void freeSession(struct session *thisSession);
childSession *createChildSession();
void freeChildSession(struct childSession *thisChild);
textStruct* createTextStruct();
void freeTextStruct(struct textStruct *thisText);


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

void encode(struct childSession *thisChild);
void decode(struct childSession *thisChild);
#endif