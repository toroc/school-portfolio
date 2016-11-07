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

#include "../inc/server_side.h"
#include "../inc/helpers.h"

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
	curSession->connType = ENC;

	/*Setup server and listen for connections*/
	startServer(curSession);

	/**/
	while(1){
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

