/******************************************************
# 	CS 372 - Winter 2016 - Program 1
#   By: Carol Toro
#   Date: 2/2/2016
#   Filename: chatserve.cpp
#   Description: This program is the server side of a chat client.
#	A TCP socket is created to listen for incoming connections from
#	clients. When a connection is accepted, the chat server gets
#	the incoming message, displays it on the screen, and prompts the
#	user for an outgoing reply. The outgoing reply is sent from the
#	server to the connected client. If at any point the connected
#	client quits, the server goes back to listening for new connections.
#	Server will continue to listen until a SIGINT is received to end the program.
#   References:
#	   API: Linux socket Programming In C++ from TLDP
#	   http ://tldp.org/LDP/LG/issue74/tougher.html
#	   List a Directory 
#		http://www.gnu.org/software/libc/manual/html_node/Simple-Directory-Lister.html#Simple-Directory-Lister
******************************************************/

#include "ServerSocket.h"
#include "SocketException.h"
#include <signal.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <cstdlib>
#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <vector>
using namespace std;
//#define DEBUG 0
#define MAX_PACKET 1024
char buffer[MAX_PACKET];

/*Global variables*/
/*function prototypes*/
void intro();
void printDirections();
void listening(int port);
string getHandle();
void print(const string &s);
string getReply(const string &h);
string appendHandle(const string &h, const string &s);
void sigHandler(int n);
void acceptConnection(ServerSocket &s, ServerSocket &n);
string getMessage(ServerSocket &sock);
void sendReply(const string &m, ServerSocket &sock);

void dataConnection(ServerSocket &server, ServerSocket &newSock);
void getDirectory(string dir, vector<string> &list);
void sendDirectory(ServerSocket &dataSock);

void sendFile(ServerSocket &dataSock);

int main(int argc, char *argv[])
{

	/*Check command line arguments are correct*/
	if (argc <= 1) // argc should be 2 for correct execution
	{
		cout << "Usage: " << argv[0] << " <portNumber>\n";
		exit(1);

	}
	/*Store port #*/
	int portNumber = atoi(argv[1]);

	/*Get user's handle*/
	string handle = getHandle();

	string quit = "\\quit";

	/*Handle interrupt signal*/
	signal(SIGINT, sigHandler);

	//signal(SIGKILL, sigHandler);

	try
	{
		/*Create Socket*/
		ServerSocket server(portNumber);
		

		while (true)
		{
			/*Wait for connection*/
			listening(portNumber);

			/*Create new socket for client*/
			ServerSocket newSock;

			/*Accept client request*/
			acceptConnection(server, newSock);

			/*Fork new process*/
			dataConnection(server, newSock);

			try
			{
				while (true)
				{
					/*Get incoming message*/
					string data = getMessage(newSock);

					/*Print incoming message*/
					print(data);

					/*Get outgoing reply*/
					string message = getReply(handle);

					/*Close the current connection*/
					if (message == quit)
					{
						break;
					}

					/*Edit outgoing reply*/
					string toSend = appendHandle(handle, message);

					/*Send outgoing reply*/
					sendReply(toSend, newSock);
				}
			}
			catch (SocketException&) {}
		}

	}
	catch (SocketException& e)

	{
		cout << "Exception was caught:" << e.description() << "\nExiting.\n";
	}

	return 0;
}
void intro()
{
	cout << "Welcome to ftserver! \n\n" << endl;
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
#   Helper function used to print to console the
#   program instructions
******************************************************/
void printDirections()
{
	cout << "Connection with chatclient established.\n";
	cout << "Use \\quit command to end connection.\n" << endl;
}
/******************************************************
#   Helper function used to get a less than 11 char
#   user handle
******************************************************/
string getHandle()
{
	string h;
	cout << "Input a 10 character chatserve handle: ";
	getline(cin, h);

	while (h.length() > 10)
	{
		cout << "Handle too long, Try again: ";
		getline(cin, h);
	}
	cout << endl << endl;

	return h;
}
/******************************************************
#   Helper function used to print the incoming
#   message from the connected client
#
******************************************************/
void print(const string &s)
{
	cout << s << endl;
}
/******************************************************
#   Helper function used to append the handle to
#   the outgoing reply message
#
******************************************************/
string appendHandle(const string &h, const string &s)
{
	return h + string("> ") + s;
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
#   Helper function used to get outgoing reply from
#   user and ensure reply does not exceed 500 chars
#
******************************************************/
string getReply(const string &h)
{
	string str;
	str.append(h);
	str.append("> ");

	string thisMessage;
	/*Print to the console so user types in message*/
	cout << str;
	getline(cin, thisMessage);
	/*Repeat if message is over 500 chars*/
	while (thisMessage.length() > 500)
	{
		cout << "Message too long: ";
		getline(cin, thisMessage);
	}

	return thisMessage;
}


/******************************************************
#   Helper function to accept the incoming connection
#
#
******************************************************/
void acceptConnection(ServerSocket &s, ServerSocket &n)
{
	s.accept(n);
}

/******************************************************
#   Helper function to get the incoming message
#   from the connected client
#
******************************************************/
string getMessage(ServerSocket &sock)
{
	string str;
	sock >> str;
	return str;
}
/******************************************************
#   Helper function to send the outgoing reply
#   to the connected client
#
******************************************************/
void sendReply(const string &m, ServerSocket &sock)
{
	sock << m;
}


void getDirectory(string dir, vector<string> &list)
{
	DIR *dirPointer;
	struct dirent *ep;

	/*Open directory*/
	dirPointer=opendir(dir);

	if(dirPointer!= NULL)
	{
		while(ep=readdir(dirPointer)){
			list.push_back(string(ep->d_name));
		}
		closedir(dirPointer);
	}
	else{
		perror("Couldn't open the directory");
	}
}
void sendDirectory(ServerSocket &dataSock)
{
	string dir = string("./");

	/*vector of files in directory*/
	vector<string> fileList=vector<string>();

	getDirectory(dir, fileList);




}