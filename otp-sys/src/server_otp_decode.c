/******************************************************
#   CS 344 - Winter 2016 - Program 4
#   By: Carol Toro
#   File Created: 03/09/2016
#   Last Modified: 03/14/2016
#   Filename: otp_dec_d.c
#	Usage: otp_dec_d <portNumber>
#    Description: This program executes the server side request
#		from a client of a pad-like system to decrypt a file with a key. 
#		The program receives the encrypted text along with the key from the client.
#		The program decrypts the file with the key and sends to the client
#		the decrypted text.
#	References: 
#	Beej's Guide
#		http://beej.us/guide/bgnet/output/html/multipage/syscalls.html#sendrecv
******************************************************/

#include "../include/server_side.h"
#include "../include/helpers.h"


/*Validation Function*/
void checkCommandLine(int argcount, char *args[]);

int main(int argc, char *argv[])
{
	/*Set handler for children*/
	signal(SIGCHLD, sigintHandle);

	/*Check command line arguments are correct*/
	checkCommandLine(argc, argv);

	struct session *curSession = createSession();

	/*Store port #*/
	curSession->serverPort=atoi(argv[1]);

	// Set connection type
	curSession->connType = DEC;

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
