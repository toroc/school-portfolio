/******************************************************
#	CS 372 - Winter 2016 - Program 2
#   By: Carol Toro
#   File Created: 2/16/2016
#   Last Modified: 03/05/2016
#   Filename: ftserver.cpp
#   Description: This program is the server side of a file transfer system.
#	A TCP socket is created to listen for incoming connections from
#	clients. When a connection is accepted, the file transfer server gets
#	the incoming request on a control connection, and responds to the request
#	on a data connection if the request is valid. When the transfer is complete,
#	the server goes back to listening for new connections. Server will continue 
#	to listen until a SIGINT is received to end the program.
#   References:
#		Beej's Guide
#		http://beej.us/guide/bgnet/output/html/multipage/syscalls.html#sendrecv
#		UNIX: List a Directory
#		http://www.gnu.org/software/libc/manual/html_node/Simple-Directory-Lister.html#Simple-Directory-Lister
******************************************************/
#include "../include/ft_server.hpp"


/*Main Program helper functions*/
void sigHandler(int n);
void checkCommandLine(int argcount, char *args[]);

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
#   Main Program Helper Functions
******************************************************/

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
#   sigHandler
#   @desc: catch signal and exit with signal number
#   @param: signal #
#   @return: void
******************************************************/
void sigHandler(int n)
{
	/*exit w signal #*/
	exit(n);
}
