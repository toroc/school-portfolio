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

#define MAX_PACKET 1000
#define MAX_NAME 50
/*Session data structure*/
typedef struct serverSession serverSession;

struct serverSession{
	int serverSocket; /*Server socket descriptor*/
	int serverPort; /*int to store server port*/
	int clientSocket;
	struct sockaddr_in serverAddr;
	struct sockaddr_in clientAddr;

	char *plainFile; /*string to store file name*/
	int plainLen;/*length of plain file text*/
	char *keyFile; /*string to store key file name*/
	int keyLen; /*length of keymfile text*/
	
	char *cipherTxt;/*string to store cipher text*/
	int cipherLen; /*length of cipher text*/

};

/*Data Structure Functions*/
serverSession* createSession();
void freeSession(struct serverSession *thisSession);


void startServer(struct serverSession *thisSession, int ftPort);

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
	while(true)
	{

		/*Start connection*/

		/*Handle request*/
	}

	/**/
	


	return 0;





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
	theSession->plainFile=(char*)malloc(sizeof(char)*MAX_NAME);
	theSession->plainLen=0;
	theSession->keyFile=(char*)malloc(sizeof(char)*MAX_NAME);
	theSession->keyLen=0;
	
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

	/*Listen for incoming connections*/
	listen(thisSession->serverSocket, 5);
}

void encode(char *plain, char *key, char *cipher);