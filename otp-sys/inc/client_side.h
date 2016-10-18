/******************************************************
#   CS 344 - Winter 2016 - Program 4
#   By: Carol Toro
#   File Created: 03/09/2016
#   Last Modified: 10/17/2016
#   Filename: client_side.h
#   Description: The client side otp system definitions.
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
#include "../include/encryption_decryption.h"
#include "../include/helpers.h"

#define MAX_BUFFER 128000
#define MAX_PACKET 8000
#define MAX_NAME 50
// Request type: encode or decode
typedef enum{ENC=0, DEC}RTYPE;
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
	struct textStruct *cipherText;
	struct textStruct *keyText;
	struct textStruct *plainText;
	int serverPort; /*int to store server port*/
	
	int socketFD; /*client socket endpoint file descriptor*/
	struct sockaddr_in serverAddr;
	struct hostent *serverIP;
	RTYPE request; /*0: encode, 1: decode*/
};

/*Data Structure Functions*/
textStruct* createTextStruct();
void freeTextStruct(struct textStruct *thisText);
session* createSession();
void freeSession(struct session *thisSession);

void startClient(struct session *thisSession);
void handleRequest(struct session *thisSession);

/*Client / Server Communication functions*/
void sendHandShake(struct session *thisSession);
void sendData(struct session *thisSession, struct textStruct *thisText);
void getData(struct session *thisSession, struct textStruct *thisText);
void sendAck(struct session *thisSession);
int confirmACK(const char *msg);

/*File and Key validation helper functions*/
void validateFiles(struct session *thisSession);
void fileCharValidation(struct textStruct *thisText);
int validChar(char curChar);