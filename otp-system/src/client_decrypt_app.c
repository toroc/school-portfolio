/******************************************************
#   CS 344 - Winter 2016 - Program 4
#   By: Carol Toro
#   File Created: 03/09/2016
#   Last Modified: 03/14/2016
#   Filename: otp_dec.c
#	Usage: otp_dec <cipherText> <keyText> <portNumber>
#   Description: This program executes the client side request
#		of a pad-like system to decrypt a file with a key. 
#		The program verifies that the files contain valid characters, 
#		and sends the file along with key to server for decryption.
#		The program receives the decrypted text and outputs it.
#	References: 
#	Beej's Guide
#		http://beej.us/guide/bgnet/output/html/multipage/syscalls.html#sendrecv
******************************************************/

#include "../inc/client_side.h"
#include "../inc/helpers.h"

/*Validation Functions*/
void checkCommandLine(int argcount, char *args[]);


int main(int argc, char *argv[])
{
	/*Check command line arguments are correct*/
	checkCommandLine(argc, argv);

	struct session *curSession = createSession();

	/*save commands*/
	curSession->cipherText->fileName=argv[1];
	curSession->keyText->fileName=argv[2];
	curSession->serverPort=atoi(argv[3]);

	/*Set request type*/
	curSession->request = DEC;
	/**/
	validateFiles(curSession);

	/*Start client connection*/
	startClient(curSession);

	/*take care of request*/
	handleRequest(curSession);

	/*Close socket*/
	close(curSession->socketFD);

	/*Deallocate memory*/
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
	if(argcount<=3){
		printf("Usage: %s <cipherText> <keyText> <portNumber>\n",args[0]);
		exit(2);
	}
}
