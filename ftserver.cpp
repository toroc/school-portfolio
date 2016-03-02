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
	string clientName;
};
/*Data Structure initialization functions*/
session* createSession();
void freeSession(struct session *thisSession);


void checkCommandLine(int argcount, char *args[]);
/*function prototypes*/

/*Socket & Connection Prototypes */
void startServer(struct session *thisSession, int ftPort);
void startNewConnection(struct session *thisSession);
bool handleRequest(struct session *thisSession);
void setupDataConnection(struct session *thisSession);
void respondToRequest(struct session *thisSession);

void closeDataSocket(struct session *thisSession);


/*Connection helper functions*/

void receiveAll(struct session *thisSession);
void parseMessage(struct session *thisSession);
void identifyCommands(struct session *thisSession);

/*Helper functions*/
void listening(int port);
void intro();

void sigHandler(int n);
/*Request connections*/

/*File Transfer helper functions*/
string getDirectoryContents();
bool fileinDir(struct session *thisSession);
void sendDirectory(struct session *thisSession);
void sendFile(struct session *thisSession);
void sendError( struct session *thisSession);
void sendFileError(struct session *thisSession);
void readSocket(struct session *thisSession);
void socketSend();
void sendOK(struct session *thisSession);

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

	cout << "DEBUG----insided startServer" << endl;
}
/******************************************************
#   startNewConnection
#   @desc:
#   @param:
#   @return:
******************************************************/
void startNewConnection(struct session *thisSession)
{

	cout << "DEBUG----Inside start new connection" << endl;
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

		cout << "ftserver > received connection from " << clientName << endl;
	}

	/*Store client address in session data structure*/
	thisSession->client_adr=clientAddr;

	/*Store client name in session data structure*/
	thisSession->clientName=clientName;

}
/******************************************************
#   funcName
#   @desc:
#   @param:
#   @return:
******************************************************/
bool handleRequest(struct session *thisSession)
{
	cout << "DEBUG----Inside handleRequest" << endl;
	/*Get commands from connection*/
	receiveAll(thisSession);

	/*Parse message received into commands*/
	parseMessage(thisSession);

	/*Identify commands received*/
	identifyCommands(thisSession);

	/*Respond to client request*/
	respondToRequest(thisSession);

	/*Close Connections*/


	/*To indicate completion*/
	return false;
}


/******************************************************
#   funcName
#   @desc:
#   @param:
#   @return:
******************************************************/
void setupDataConnection(struct session *thisSession)
{
	cout << "DEBUG----Inside setupDataConnection" << endl;

	/*Save data port*/
	int port = thisSession->dataPort;

	/*Get the client's address*/
	struct sockaddr_in cl_address;

	socklen_t len=sizeof(thisSession->client_adr);

	int result =getpeername(thisSession->controlSocket, (struct sockaddr *)&cl_address,&len);
	
	if (result ==-1){
		cout << "Error: unable to get peer's address." << endl;
	}


	/*Change port to the data port */
	cl_address.sin_port = htons(thisSession->dataPort);


	/*Create data socket*/
	thisSession->dataSocket=socket(AF_INET, SOCK_STREAM,0);

	/*Attempt to connect with peer over different socket*/
	result = connect(thisSession->dataSocket, (struct sockaddr*)&cl_address, sizeof(cl_address));


	/*Ensure it worked*/
	if (result <0){
		/*error*/
		cout << "Error: unable to create data connection." << endl;
	}

}

void closeDataSocket(struct session *thisSession)
{
	int result = close(thisSession->dataSocket);

	if(result ==-1){
		cout << "Error: unable to close data socket." << endl;
	}
}
void respondToRequest(struct session *thisSession)
{
	cout << "DEBUG----Inside respondToRequest" << endl;

	if(thisSession->type==INVALID){
		/*Send error message on control socket*/
		sendError(thisSession);

		return;
	}

	/*Find if file exists in*/
	if(thisSession->type == GET){

		/*Was file found in directory*/
		bool fileFound=fileinDir(thisSession);

		if(!fileFound){
			/**/
			sendFileError(thisSession);
			/**/
			return;
		
		}
	}

	/*Since no error send OK on connection P*/
	sendOK(thisSession);

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

	cout << "DEBUG----Inside receiveAll" << endl;

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
	cout << "DEBUG----Inside parseMessage" << endl;

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

  
    thisSession->numCommands=num;
    
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
	cout << "DEBUG----Inside identifyCommands" << endl;
	char const *list="-l";
	int listLen=(unsigned)strlen(list);
	char const *get="-g";

	
	/*Get length of first command*/
	int firstCommLen  = strlen(thisSession->commands[0]);

	
	if(thisSession->numCommands>3){
		cout << "DEBUG----Too many commands" << endl;
		thisSession->type=INVALID;
	}

	/*List directory*/
	if(thisSession->numCommands==2){ /*Compare 1st command to -l */

		/*equal length commands*/
		if (firstCommLen == listLen) {

			/*Ensure the strings are identical*/
			if(strncmp(thisSession->commands[0], list, listLen)==0){
				
				cout << "DEBUG----Requesting list" << endl;

				/*Set command type*/
				thisSession->type=LIST;

				/*Convert 2nd command to int*/
				int data=atoi(thisSession->commands[1]);
		
				/*Store port in struct*/
				thisSession->dataPort=data
				
				/*Print console message*/
				cout << "ftserver > List directory requested on port "<< thisSession->dataPort<< endl;

			}
			else{
				/*Commands not identical in chars*/
				thisSession->type=INVALID;
			}
		}
		else{
			/*Commands not identical in length*/
			thisSession->type=INVALID;
		}

		
	}

	/*Get File*/
	if(thisSession->numCommands==3){ /*Compare 1st command to -g */

		/*equal length commands*/
		if(firstCommLen == listLen){

			/*Ensure strings are identical*/
			if(strncmp(comms,get,listLen)==0){

				cout << "DEBUG----Requesting file" << endl;

				
				/*Set command type*/
				thisSession->type=GET;

				/*Store filename in data structure*/
				thisSession->fileName=thisSession->commands[1];
				
				/*Convert 3rd command to int*/
				int data=atoi(thisSession->commands[2]);

				/*Store in struct*/
				thisSession->dataPort=data;			

				/*Print to console*/
				cout << "ftserver > file " << thisSession->fileName << " requested on port " << thisSession->dataPort << endl;
			}
			else{
				/*commands not identical in chars*/
				thisSession->type=INVALID;
			}

		}		
		else{
			/*Commands not identical in length*/
			thisSession->type=INVALID;
		}

		

	}
}
/******************************************************
#   Helper function used to print to console that
#   chat serve is listening
******************************************************/
void listening(int port)
{
	cout << "ftserver > now listening for incoming connections on port #" << port << "." << endl;
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
	cout << "DEBUG----Inside getDirectoryContents" << endl;
	/*directory pointer*/
	DIR *dirPointer;
	struct dirent *curDir;
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
	while(curDir=readdir(dirPointer)){

		/*Append file name string*/
		dirContents+=curDir->d_name;
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

	/*Ensure send worked*/
	if(bytesSent==-1){
		/*Error*/
		cout << "Error: unable to send directory contents" << endl;
	}
	else{
		cout << "ftserver > sending directory contents on port " << thisSession->dataPort << endl;
	}

	/*Close data socket*/
	closeDataSocket(thisSession);

}
/******************************************************
#   funcName
#   @desc:
#   @param:
#   @return:
******************************************************/
void sendFile(struct session *thisSession){

	/*Variables with messages to send*/
	/*File descriptor for reading file*/
	FILE *fileFD;
	int result;

	/*buffer to store file contents*/
	char buffer[MAX_PACKET*2];

	/*Send file to client*/	

	fileFD=fopen(thisSession->fileName.c_str(),"r");

	if(fileFD==NULL){
		cout << "Error: unable to open file." << endl;
	}

	/*Load file items to buffer*/
	while(fgets(buffer, sizeof(buffer), fileFD)!= NULL){

		result=send(thisSession->dataSocket, buffer, strlen(buffer),0);

		/*Ensure send worked*/
		if(result<0){
			cout << "Error: unable to send file data to client."<<endl;
		}
		else{
			cout << "ftserver > sending " << thisSession->fileName << " on port " << thisSession->dataPort<<endl;
		}

		/*Wait before sending next data */
		usleep(10);
	}

	/*Close the file*/
	fclose(fileFD);

	/*Close data socket*/
	closeDataSocket(thisSession);

}

bool fileinDir(struct session *thisSession)
{
	/*directory pointer*/
	DIR *dirPointer;
	struct dirent *curDir;

	/*Open directory*/
	dirPointer=opendir(".");


	/*Loop through files in directory*/
	while((curDir=readdir(dirPointer))){

		/*Length of file name*/
		int len=thisSession->fileName.length();

		/*Length of file in directory*/
		int dirFileLen=strlen(curDir->d_name);


		/*Return true if match*/
		if(strncmp(curDir->d_name, thisSession->fileName.c_str(),dirFileLen)==0){
			return true;
		}
	}

	closedir(dirPointer);

	return false;
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

		/*Print to console*/
		cout << "ftserver > received invalid command from client." << endl;

		/*Convert string to c string*/
		const void * errMsg = thisSession->message.c_str();

		/*Send message over control socket*/
		result=send(thisSession->controlSocket, errMsg, length,0);

		/*Ensure send worked*/
		if(result<0){
			cout << "Error: unable to send error message to client." << endl;
		}
		
	}

}

void sendFileError(struct session *thisSession)
{
	/*Print to console*/
	cout << "ftserver > file requested by client not found." << endl;

	string notFound="FILE NOT FOUND";
	int len=notFound.length();
	int result;

	/*convert string to c_string*/
	const void *errMsg=notFound.c_str();
	
	/*send over control socket*/
	result = send(thisSession->controlSocket, errMsg, len, 0);

	/*Ensure send worked*/
	if(result<0){
		cout << "Error: unable to send error message to client." << endl;
	}		

}
void sendOK(struct session *thisSession)
{
	string msg="OK";
	int len = msg.length();

	/*Convert string to c string*/
	const void *okMsg = msg.c_str();

	int result = send(thisSession->controlSocket, okMsg,len,0);

	if(result<0){
		cout << "Error: unable to send OK response to client." << endl;
	}
}
