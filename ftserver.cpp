/******************************************************
# 	CS 372 - Winter 2016 - Program 2
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
#	   API: Linux socket Programming In C++ from TLDP
#	   http ://tldp.org/LDP/LG/issue74/tougher.html
#	   List a Directory 
#		http://www.gnu.org/software/libc/manual/html_node/Simple-Directory-Lister.html#Simple-Directory-Lister
******************************************************/

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
#include <vector>

using namespace std;
//#define DEBUG 0
#define MAX_PACKET 1000
#define MAX_COMMANDS 5

const int NUM_SIZE=10;
/*Global variables*/
char buffer[MAX_PACKET];

typedef session session;

struct session{
	char *msgBuffer;
	string message;
	int msgLength;
	char **commands;
	int numCommands;
	char *fileName;
	int dataPort;
	struct sockaddr_in clientAddr;
};


session* createSession();

void indentifyCommands(struct session *thisSession);
/*function prototypes*/

/*Socket & Connection Prototypes */
void startServer(int &serverSocket, int ftPort);
void startNewConnection(int &controlSocket, int serverSocket, struct sockaddr_in &client, socklen_t &clientLength);
void setupDataConnection(int controlSocket, int &dataSocket, struct session *thisSession);
bool handleRequest(int controlSocket, int serverSocket, struct session *thisSession);
/*Socket helper functions*/
void listening(int port);
void intro();
void saveClientAddr(struct session *thisSession, struct sockaddr_in client);

void sigHandler(int n);
void clientConnected(string name);
/*Request connections*/

/*File Transfer helper functions*/
string getDirectoryContents();

void sendDirectory(int controlSocket, int dataSocket, struct session *thisSession);
void sendFile(int controlSocket, int dataSocket, struct session *thisSession);
void sendAll(int socketFD, struct session *thisSession);
void receiveAll(int socketFD, struct session *thisSession);

void readSocket(int socketFD, struct session *thisSession);
void socketSend();
void parseMessage(int socketFD, struct session *thisSession);
void sendFile(ServerSocket &dataSock);

int main(int argc, char *argv[])
{

	/*Check command line arguments are correct*/
	if (argc <= 1) // argc should be 2 for correct execution
	{
		cout << "Usage: " << argv[0] << " <portNumber>\n";
		exit(1);

	}
	/*Create session*/
	struct session *curSession=createSession();

	/*Socket File descriptors*/
	int serverSocket;
	int controlSocket;
	

	struct sockadrr_in clientAddr;
	socklen_t clientLength = sizeof(clientAddr);
	
	/*Store port #*/
	int portNumber = atoi(argv[1]);

	
	/*Setup server & listen for connections*/
	startServer(serverSocket, portNumber);



	
	while (true)
	{
		/*Start control connection*/
		startNewConnection(controlSocket, serverSocket, clientAddr, clientLength);

		/*Store client address in struct object*/
		saveClientAddr(curSession, clientAddr);

		while (true)
		{
			/*Handle client requests*/
			handleRequest(controlSocket, serverSocket, curSession);
			/*Read message*/
			
		}
	}

	
	

	return 0;
}

session* createSession(){

	/*Allocate memory for the session variables*/

	session *theSession = malloc(sizeof(session));
	theSession->msgLength=0;
	theSession->msgBuffer=malloc(sizeof(char)* MAX_PACKET);
	theSession->commands=(char**)malloc(MAX_COMMANDS * sizeof(char*));

	/*Allocate memory for array of strings*/
	for(int i=0; i < MAX_COMMANDS; i++){
		theSession->commands[i]=(char*)malloc(20 * sizeof(char));
	}
}
/******************************************************
#   startServer
#   @desc: 	
#   @param: n/a
#   @return: 
******************************************************/
void intro()
{
	cout << "Welcome to ftserver! \n\n" << endl;
}

/******************************************************
#   startServer
#   @desc: 	
#   @param: n/a
#   @return: 
******************************************************/
void startServer(int &serverSocket, int ftPort)
{
	/*Create server socket*/
	serverSocket=socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server;

	/*Setup server socket*/
	server.sin_family = AF_INET;

	server.sin_port=htons(ftPort);
	server.sin_addr.s_addr=INADDR_ANY;

	/*Bind server socket*/
	bind(serverSocket, (struct sockaddr *) &server, sizeof(server));

	/*Listen for incoming connections*/
	listen(serverSocket, 1);

	/*Print to console*/
	listening(ftPort);
}

void startNewConnection(int &controlSocket, int serverSocket, struct sockaddr_in &client, socklen_t &clientLength)
{
	int newSocketFD;
	char clientName[1024];
	char service[20];

	controlSocket=accept(serverSocket, (struct sockaddr*) &client,&clientLength);

	/*Ensure connection worked*/
	if(controlSocket<0){
		cout << "Error: unable to accept connection." << endl;
	}
	else{

		/*get name of client*/
		getnameinfo((struct sockaddr *)&client, sizeof(client),clientName,sizeof(clientName), service, sizeof(service),0);

		cout << "Connection from " << clientName << endl;
	}

	/*get message from connected client*/



}
/******************************************************
#   Helper function used to print to console that
#   chat serve is listening
******************************************************/
void listening(int port)
{
	cout << "Port #" << port << ": ftserver is now listing for incoming connections.\n" << endl;
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


void saveClientAddr(struct session *thisSession, struct sockaddr_in client)
{
	thisSession->clientAddr=client;

}

/******************************************************
#   getDirectoryContents
#   @desc: open directory, and append file names to 
#		to string
#   @param: n/a
#   @return: string containing dir contents
******************************************************/
string getDirectoryContents()
{	
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
void sendDirectory(int controlSocket, int dataSocket, struct session *thisSession)
{
	int bytesSent=0;
	int len=contents.length();
	string contents = getDirectoryContents();

	/*Send string to client*/
	bytesSent=send(dataSocket, contents, len, 0);

	if(bytesSent==-1){
		/*Error*/
		cout << "Error: unable to send directory contents" << endl;
	}


}

void sendFile(int controlSocket, int dataSocket, struct session *thisSession){

}

void setupDataConnection(int controlSocket, int &dataSocket, struct session *thisSession)
{
	/*Create client socket*/
	dataSocket=socket(AF_INET, SOCK_STREAM,0);
	
	/*Ensure it worked*/

	int port = thisSession->portNumber;
	/*Change port of the */
	theSession->clientAddr.sin_port=htons(port);

	/*Connect*/
	if (connect(dataSocket, (struct sockaddr*)thisSession->clientAddr, sizeof(thisSession->clientAddr))==-1){
		/*error*/
		cout << "Error: unable to create data connection" << endl;
	}

}
bool handleRequest(int controlSocket, int serverSocket, struct session *thisSession)
{
	/*Get commands from connection*/
	receiveAll(controlSocket, thisSession);

	/*Parse message received into commands*/
	parseMessage(thisSession);


	/*Identify commands received*/	

	/*Start data connection*/
	int dataSocket;
	setupDataConnection(&controlSocket, &dataSocket, thisSession);

}
/******************************************************
#   startServer
#   @desc: 	
#   @param: n/a
#   @return: 
******************************************************/
void receiveAll(int socketFD, struct session *thisSession){

	/*Clear the buffer*/
	bzero(thisSession->msgBuffer, MAX_PACKET);
	/*Variable to keep track of bytes read*/
	int bytesRead=0;

	if((bytesRead=recv(socketFD, thisSession->msgBuffer, MAX_PACKET-1,0)==-1){
		/*Error*/
		exit(1);
	}

	/*Make received mesage null terminated*/
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
void parseMessage(int socketFD, struct session *thisSession)
{
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