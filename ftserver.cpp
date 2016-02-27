/******************************************************
#	CS 372 - Winter 2016 - Program 2
#   By: Carol Toro
#   File Created: 2/16/2016
#   Last Modified: 2/17/2016
#   Filename: ftserver.cpp
#   Description: This program is the server side of a chat client.
#	A TCP socket is created to listen for incoming connections from
#	clients. When a connection is accepted, the chat server gets
#	the incoming message, displays it on the screen, and prompts the
#	user for an outgoing reply. The outgoing reply is sent from the
#	server to the connected client. If at any point the connected
#	client quits, the server goes back to listening for new connections.
#	Server will continue to listen until a SIGINT is received to end the program.
#   References:
#		Beej's Guide
#		UNIX: List a Directory
#		http://www.gnu.org/software/libc/manual/html_node/Simple-Directory-Lister.html#Simple-Directory-Lister
******************************************************/

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
	string fileName;
	int dataPort;
	/*Socket file descriptors*/
	int serverSocket; /*Used for listening*/
	int controlSocket; /*Used for connecting with client*/
	int dataSocket; /*Used for sending data to client*/
	struct sockaddr_in client_adr;
};
/*Data Structure initialization functions*/
session* createSession();
void freeSession(struct session *thisSession);


void checkCommandLine(int argcount, char *args[]);
/*function prototypes*/

/*Socket & Connection Prototypes */
void startServer(struct session *thisSession, int ftPort);
void startNewConnection(struct session *thisSession);
void handleRequest(struct session *thisSession);
void setupDataConnection(struct session *thisSession);
void respondToRequest(struct session *thisSession);

/*Connection helper functions*/
void receiveAll(struct session *thisSession);
void parseMessage(struct session *thisSession);
void identifyCommands(struct session *thisSession);

/*Helper functions*/
void listening(int port);
void intro();
void saveClientAddr(struct session *thisSession, struct sockaddr_in client);

void sigHandler(int n);
/*Request connections*/

/*File Transfer helper functions*/
string getDirectoryContents();
void sendDirectory(struct session *thisSession);
void sendFile(struct session *thisSession);
void sendError( struct session *thisSession);
void readSocket(struct session *thisSession);
void socketSend();


int main(int argc, char *argv[])
{

	/*Check command line arguments are correct*/
	checkCommandLine(argc, argv);

	/*Store port #*/
	int portNumber = atoi(argv[1]);

	/*Create session*/
	struct session *curSession=createSession();

	/*Setup server & listen for connections*/
	startServer(curSession, portNumber);


	while (true)
	{
		/*Start control connection*/
		startNewConnection(curSession);


		
			/*Handle client requests*/
			handleRequest(curSession);
			/*Read message*/

		
	}

	freeSession(curSession);

	return 0;
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
	if(argcount<=1){
		cout << "Usage: " << args[0] << " <portNumber>\n";
		exit(1);
	}
}
/******************************************************
#   Session Data Structure Functions
******************************************************/
/******************************************************
#   createSession
#   @desc: allocate memory for  data structure
#       and return pointer to session object
#   @param: n/a
#   @return: struct session *theSession
******************************************************/
session* createSession(){

	/*Allocate memory for the session variables*/

	session *theSession = (session*)malloc(sizeof(session));
	theSession->msgLength=0;
	theSession->msgBuffer=(char*)malloc(sizeof(char)* MAX_PACKET);
	theSession->commands=(char**)malloc(MAX_COMMANDS * sizeof(char*));

	/*Allocate memory for array of strings*/
	for(int i=0; i < MAX_COMMANDS; i++){
		theSession->commands[i]=(char*)malloc(20 * sizeof(char));
	}

	return theSession;
}

/******************************************************
#   freeUser
#   @desc: Deallocate the memory used up by user
#   @param: pointer to user object
#   @return: void
******************************************************/
void freeSession(struct session *thisSession)
{
	free(thisSession);

}




/******************************************************
#   funcName
#   @desc:
#   @param:
#   @return:
******************************************************/
void intro()
{
	cout << "Welcome to ftserver! \n\n" << endl;
}
/******************************************************
#   Socket & Connection Functions
******************************************************/
/******************************************************
#   startServer
#   @desc:
#   @param: n/a
#   @return:
******************************************************/
void startServer(struct session *thisSession, int ftPort)
{
	/*Create server socket*/
	thisSession->serverSocket=socket(AF_INET, SOCK_STREAM, 0);
	
	/*Ensure it worked*/
	if(thisSession->serverSocket<0){
		cout << "Error: unable to create socket." << endl;
	}

	struct sockaddr_in server;
	int optval=1;

	/*Setup server socket*/
	server.sin_family = AF_INET;
	server.sin_port=htons(ftPort);
	server.sin_addr.s_addr=INADDR_ANY;

	/*Allow reuse of address*/
	int res=setsockopt(thisSession->serverSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

	/*Ensure it worked*/
	if(res==-1){
		cout << "Error: unable to setup SO_REUSEADDR." << endl;
	}

	/*Bind server socket*/
	res=bind(thisSession->serverSocket, (struct sockaddr *) &server, sizeof(server));
	if(res<0){
		cout << "Error: unable to bind to host address." << endl;
		exit(EXIT_FAILURE);
	}

	/*Listen for incoming connections*/
	listen(thisSession->serverSocket, 1);

	/*Print to console*/
	listening(ftPort);

	cout << "insided startServer" << endl;
}
/******************************************************
#   startNewConnection
#   @desc:
#   @param:
#   @return:
******************************************************/
void startNewConnection(struct session *thisSession)
{

	cout << "Inside start new connection" << endl;
	char clientName[1024];
	char service[20];

	/*Vars to store client's info at connection and accept*/
	struct sockaddr_in clientAddr;
	socklen_t clientLength = sizeof(clientAddr);

	/*Accept incoming  control connection*/
	thisSession->controlSocket=accept(thisSession->serverSocket, (struct sockaddr*) &clientAddr,&clientLength);

	/*Ensure connection worked*/
	if(thisSession->controlSocket<0){
		cout << "Error: unable to accept connection." << endl;
	}
	else{

		/*get name of client*/
		getnameinfo((struct sockaddr *)&clientAddr, sizeof(clientAddr),clientName,sizeof(clientName), service, sizeof(service),0);

		cout << "Connection from " << clientName << endl;
	}

	/*Store client address in struct object*/
	saveClientAddr(thisSession, clientAddr);

}
/******************************************************
#   funcName
#   @desc:
#   @param:
#   @return:
******************************************************/
void handleRequest(struct session *thisSession)
{
	cout << "Inside handleRequest" << endl;
	/*Get commands from connection*/
	receiveAll(thisSession);

	/*Parse message received into commands*/
	parseMessage(thisSession);

	/*Identify commands received*/
	identifyCommands(thisSession);

	/*Respond to client request*/
	respondToRequest(thisSession);

	
}


/******************************************************
#   funcName
#   @desc:
#   @param:
#   @return:
******************************************************/
void setupDataConnection(struct session *thisSession)
{
	cout << "Inside setupDataConnection" << endl;
	/*Create client socket*/
	thisSession->dataSocket=socket(AF_INET, SOCK_STREAM,0);

	int port = thisSession->dataPort;

	/*Change port of the */
	struct sockaddr_in client_addr;
	client_addr.sin_family=AF_INET;
	client_addr.sin_port=htons(port);
	client_addr.sin_addr.s_addr=thisSession->client_adr.sin_addr.s_addr;


	/*Connect*/ /*Ensure it worked*/
	if (connect(thisSession->dataSocket, (struct sockaddr*)&client_addr, sizeof(client_addr))==-1){
		/*error*/
		cout << "Error: unable to create data connection" << endl;
	}

}


void respondToRequest(struct session *thisSession)
{
	cout << "Inside respondToRequest" << endl;

	if(thisSession->type==INVALID){


		/*Send error message on control socket*/
		sendError(thisSession);

		return;

	}


	/*Start data connection*/
	setupDataConnection(thisSession);

	if(thisSession->type==LIST){

		sendDirectory(thisSession);

		return;
	}


	if(thisSession->type==GET){
		sendFile(thisSession);

		return;
	}
}



/******************************************************
#   Connection Helper Functions
******************************************************/
/******************************************************
#   receiveall
#   @desc:
#   @param: n/a
#   @return:
******************************************************/
void receiveAll(struct session *thisSession){

	cout << "Inside receiveAll" << endl;

	/*Clear the buffer*/
	bzero(thisSession->msgBuffer, MAX_PACKET);
	/*Variable to keep track of bytes read*/
	int bytesRead=0;

	bytesRead=recv(thisSession->controlSocket, thisSession->msgBuffer, MAX_PACKET-1,0);

	if(bytesRead==-1){

		/*Error*/
		exit(1);
	}

	/*Make received message null terminated*/
	thisSession->msgBuffer[bytesRead]=0;

}
/******************************************************
#   parseMessage
#   @desc: parse the command stored in  user's userInput into
#       individual arguments stored in user's arguments
#   @param: pointer to user object
#   @return: void
******************************************************/
/*Repurposed this function from a CS344 function */
void parseMessage(struct session *thisSession)
{
	cout << "Inside parseMessage" << endl;

	char *buf=thisSession->msgBuffer;
	char **comms=thisSession->commands;
	int num=0;

	while((*buf != '\0')) {

		/*Strip the whitespace*/
		while ((*buf == ' ') || (*buf == '\n')) {
            *buf++ = '\0';
        }

        /*Save the command.*/
        *comms++ = buf;

       /*Skip over valid commands*/
        while ((*buf != '\0') && (*buf != ' ') && (*buf != '\n')) {
            buf++;
        }


        num++;
    }

    num--;
    thisSession->numCommands=num;
    *comms--;
    *comms=0;/*Make it null terminated*/

}
/******************************************************
#   identifyCommands
#   @desc:
#   @param: 
#   @return:
******************************************************/
void identifyCommands(struct session *thisSession)
{
	/**/
	cout << "Inside identifyCommands" << endl;
	char const *list="-l";
	int listLen=(unsigned)strlen(list);
	char const *get="-g";

	/*Make copy of command */
	int cmLen=(unsigned)strlen(thisSession->msgBuffer);

	char comms[cmLen];

	strcpy(comms, thisSession->msgBuffer);

	if(thisSession->numCommands>3){
		cout << "Too many commands" << endl;
		thisSession->type=INVALID;
	}
	/*List directory*/
	if(thisSession->numCommands==2){

		/*Compare 1st command to -l */
		if(strncmp(comms, list, listLen)==0){
			cout << "Requesting list" << endl;
		}
		else{
			thisSession->type=INVALID;
		}

		/*Convert 2nd command to int*/
		int data=atoi(thisSession->commands[1]);
		
		/*Store it in struct*/
		thisSession->dataPort=data;

		/*Set command type*/
		thisSession->type=LIST;

		/*Print console message*/
		cout << "ftserver > List directory requested on port "<< thisSession->dataPort<< endl;
	}

	if(thisSession->numCommands==3){

		/*Compare 1st command to -g*/
		if(strncmp(comms,get,listLen)==0){
			cout << "Requesting file" << endl;
		}
		else{
			thisSession->type=INVALID;
		}

		/*Store filename in data structure*/
		thisSession->fileName=thisSession->commands[1];
		
		/*Convert 3rd command to int*/
		int data=atoi(thisSession->commands[2]);

		/*Set command type*/
		thisSession->type=GET;


		cout << "ftserver > File " << thisSession->fileName << " requested on port " << thisSession->dataPort << endl;

	}
}
/******************************************************
#   Helper function used to print to console that
#   chat serve is listening
******************************************************/
void listening(int port)
{
	cout << "ftserver > Port #" << port << ": ftserver is now listing for incoming connections.\n" << endl;
}


/******************************************************
#   Function to handle the signal interruptions
#   and exit the program
#
******************************************************/
void sigHandler(int n)
{
	exit(n);
}

/******************************************************
#   funcName
#   @desc:
#   @param:
#   @return:
******************************************************/
void saveClientAddr(struct session *thisSession, struct sockaddr_in client)
{
	cout << "Inside saveClientAddr" << endl;
	thisSession->client_adr=client;

}

/******************************************************
#   File Transfer Helper Functions
******************************************************/

/******************************************************
#   getDirectoryContents
#   @desc: open directory, and append file names to
#		to string
#   @param: n/a
#   @return: string containing dir contents
******************************************************/
string getDirectoryContents()
{
	cout << "Inside getDirectoryContents" << endl;
	/*directory pointer*/
	DIR *dirPointer;
	struct dirent *ep;
	/*Variable to contain contents of directory*/
	string dirContents="";

	/*Open directory*/
	dirPointer=opendir(".");

	/*Ensure open worked*/
	if(dirPointer == NULL){
		cout << "Error: unable to open folder to read file" << endl;
		exit(1);
	}

	/*Loop thru files in directory*/
	while(ep=readdir(dirPointer)){

		/*Append file name string*/
		dirContents+=ep->d_name;
		/*Append new line*/
		dirContents+="\n";

	}

	/*Close directory*/
	closedir(dirPointer);

	/*Return string containing dir contents*/
	return dirContents;

}





/******************************************************
#   funcName
#   @desc:
#   @param:
#   @return:
******************************************************/
void sendDirectory(struct session *thisSession)
{
	int bytesSent=0;
	string contents = getDirectoryContents();
	int len=contents.length();

	/*Convert contents string to c_string*/
	const void * dirCont = contents.c_str();

	/*Send string to client*/
	bytesSent=send(thisSession->dataSocket, dirCont, len, 0);

	if(bytesSent==-1){
		/*Error*/
		cout << "Error: unable to send directory contents" << endl;
	}
	else{
		cout << "ftserver > sending directory contents on port " << thisSession->dataPort << endl;
	}




}
/******************************************************
#   funcName
#   @desc:
#   @param:
#   @return:
******************************************************/
void sendFile(struct session *thisSession){

	/*Variables with messages to send*/

	string notFound="FILE NOT FOUND";


	


}


void sendError( struct session *thisSession)
{
	/**/
	string invalidCommand="Error. Command is invalid.";
	thisSession->message=invalidCommand;


	int length = thisSession->message.length();

	int result;

	/*Send on control connection*/
	if(thisSession->type==INVALID){

		cout << "ftserver > Client sent over an invalid command." << endl;

		/*Convert string to c string*/
		const void * errMsg = thisSession->message.c_str();

		result=send(thisSession->controlSocket, errMsg, length,0);

		/*Ensure it worked*/
		if(result<0){
			cout << "Error: unable to send error message to client" << endl;
		}
		
	}

}


