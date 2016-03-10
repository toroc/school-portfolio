#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 


/*Session data structure*/
typedef struct session session;

struct session{
	char *fileName; /*string to store file name*/
	char *plainTxt;/*String to store plain text*/
	int plainLen;/*length of plain text*/
	char *keyTxt; /*string to store key text*/
	int keyLen; /*length of key*/
	int serverPort; /*int to store server port*/
	char *cipherTxt;/*string to store cipher text*/
	int cipherLen; /*length of cipher text*/

};

/*Data Structure Functions*/
session* createSession();
void freeSession(struct session *thisSession);



void eror(const char *msg);
void validateFileKey(
int main(int argc, char *argv[])
{

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
#       and return pointer to session object
#   @param: n/a
#   @return: pointer to session *theSession object
******************************************************/
session* createSession(){

	/*Allocate memory for the session variables*/
	session *theSession = (session*)malloc(sizeof(session));
	theSession->msgLength=0;
	theSession->msgBuffer=(char*)malloc(sizeof(char)* MAX_PACKET);
	theSession->commands=(char**)malloc(MAX_COMMANDS * sizeof(char*));
	theSession->clientName=(char*)malloc(sizeof(char)*MAX_LENGTH);

	/*Allocate memory for array of strings*/
	for(int i=0; i < MAX_COMMANDS; i++){
		theSession->commands[i]=(char*)malloc(MAX_LENGTH * sizeof(char));
	}

	return theSession;
}

/******************************************************
#   freeSession
#   @desc: Deallocate the memory used up by user
#   @param: pointer to session datat structure object
#   @return: void
******************************************************/
void freeSession(struct session *thisSession)
{
	free(thisSession);

}