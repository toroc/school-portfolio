/******************************************************
#	CS 372 - Winter 2016 - Program 2
#   By: Carol Toro
#   File Created: 2/16/2016
#   Last Modified: 10/04/2016
#   Filename: ftserver.hpp
#   Description: The server side file transfer system definitions.
#	
******************************************************/
#ifndef FT_SERVER_HPP
#define FT_SERVER_HPP

#include <arpa/inet.h>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;
//#define DEBUG 0
#define MAX_PACKET 1000
#define MAX_COMMANDS 5
#define MAX_LENGTH 50


typedef enum{INVALID=0, LIST, GET}CTYPE;

/*Session Data Structure*/
typedef struct session session;

struct session{
	char *msgBuffer; /*String to capture incoming message*/
	string message;
	int msgLength;
	char **commands;
	int numCommands;
	CTYPE type;
	int commValid; /*0=false, 1=true*/
	char * fileName;
	int dataPort;
	int controlPort;
	/*Socket file descriptors*/
	int serverSocket; /*Used for listening*/
	int controlSocket; /*Used for connecting with client*/
	int dataSocket; /*Used for sending data to client*/
	struct sockaddr_in client_adr;
	char *clientName;
};



/*Data Structure initialization functions*/
session* createSession();
void freeSession(struct session *thisSession);

/*Socket & Connection Functions */
void startServer(struct session *thisSession, int ftPort);
void startNewConnection(struct session *thisSession);
void setupDataConnection(struct session *thisSession);
void closeDataSocket(struct session *thisSession);
void closeControlSocket(struct session *thisSession);
void listening(int port);

/*Connection helper functions*/
void receiveCtrl(struct session *thisSession);
void parseMessage(struct session *thisSession);
void identifyCommands(struct session *thisSession);

/*File transfer Flow */
bool handleRequest(struct session *thisSession);
void respondToRequest(struct session *thisSession);

/*File Transfer helper functions*/
string getDirectoryContents();
bool fileinDir(struct session *thisSession);
void sendDirectory(struct session *thisSession);
void sendFile(struct session *thisSession);
void sendError( struct session *thisSession);
void sendFileError(struct session *thisSession);
void readSocket(struct session *thisSession);
void sendOK(struct session *thisSession);
#endif