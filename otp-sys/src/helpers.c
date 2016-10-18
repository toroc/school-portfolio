/******************************************************
#   CS 344 - Winter 2016 - Program 4
#   By: Carol Toro
#   File Created: 03/09/2016
#   Last Modified: 10/17/2016
#   Filename: helpers.c
#   Description: Key generator function definitons.
#
#
******************************************************/


void sigintHandle(int sigNum)
{
	pid_t pid;
	int status;
	/*Catch exiting children*/
	pid = waitpid(-1, &status, WNOHANG);
}

void debugTrace(const char *msg, int line){
	//printf("OTP_ENC_D > %s from line # %d \n", msg, line);

	fflush(stdout);
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}