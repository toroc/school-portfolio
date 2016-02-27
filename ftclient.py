#!/usr/bin/python3.2

#------------------------------------------------------------
# CS 372 - Winter 2016 - Program 2
#   By: Carol Toro
#   File Created: 2/16/2016
#   Last Modified: 2/19/2016
#   Filename: ftclient.py
#   Description: 
#   
#   
#   
#    
#   
#  
#  
#   References:
#    Socket Programming, Section 2.7 from Computer Networking, A Top-Down Approach (6th Edition)
#   
#------------------------------------------------------------
from socket import *
import sys
import argparse





#------------------------------------------------------------
#   Helper functions to output program messages to console
#   
#------------------------------------------------------------
def printControlMsg(name, control):
    """Print to console message re control socket."""
    print("Control connection established with server "+ name + " on port "+ str(control)+"\n")
    print("Sending request.\n")  

def receiveDirMsg(server, data):
    """Print message to console."""
    print("Receiving directory contents from " +server+":"+str(data)+"\n")

def receiveFileMsg(server, file, data):
    """."""
    print("Receiving "+file+" from "+server+":"+data+"\n")


def commandType(command):
    """Return type of command"""

    if command[1]=="l":
        print("Requesting list\n")
        return 2
    
    if command[1]=="g":
        print("Getting file")
        return 1
    
    #wrong command 
    return -1 


#------------------------------------------------------------
#   Functions to carry out socket connection processes of
#       initializing, connecting, sending, and receiving
#------------------------------------------------------------
def createSocket():
    """Return a socket initialization."""
    return socket(AF_INET, SOCK_STREAM)


def connectSocket(sock, name, port):
    """Process a socket connection."""
    sock.connect((name, port))


def listenSocket(sock, port):
    """Bind dataSocket to port."""
    # Get local name
   
    # Bind to the port     
    sock.bind(("", port))

    print("Waiting for server response on port "+str(port)+"\n")

#------------------------------------------------------------
#   Functions to carry out printing messages to the console
#------------------------------------------------------------

def handleControl(sock, servName, servPort, command, file, dataPort):

    printControlMsg(servName, servPort)
    
    # String of message to send
    msg=commandMsg(command,dataPort, file)

    # Send command to server via control socket
    sendMessage(sock, msg)



#------------------------------------------------------------
#   Functions to carry out socket connection processes of
#       initializing, connecting, sending, and receiving
#------------------------------------------------------------

def commandMsg(comm, port,file):
    """Return stylized string of command message"""
    message=comm+" "+file+" "+str(port)

    return message


def sendMessage(sock, mess):
    """Send string via socket connection."""
    sock.send(mess.encode())

     
def getReply(sock):
    """Return a string received via socket connection."""
    return sock.recv(1024)

#------------------------------------------------------------
#   Function to parse the various arguments the program needs
#       to be able to run
#------------------------------------------------------------




def main():
    

    #save arguments passed into program
    serverName = str(sys.argv[1])
    serverPort = int(sys.argv[2])
    command=str(sys.argv[3])
    dataPort=int(sys.argv[4])
    filename=""

    if(len(sys.argv)==6):
        dataPort=int(sys.argv[5])
        filename=str(sys.argv[4]);
    
    # DEBUG: Figure out command type
    cType=commandType(command)

    # Create control connection to server SOCK_STREAM
    controlSocket = createSocket()

    # initiate handshake and begin TCP connection
    connectSocket(controlSocket, serverName, serverPort)

   
    

    while(True):

        # handle control connection
        handleControl(controlSocket, serverName, serverPort, command, filename, dataPort)
        

        
        
        #Create data socket
        dataSocket=createSocket()

        # Listen on 2nd port for server response
        listenSocket(dataSocket, dataPort)

        while(True):

            # Connected with server on data socket

            if(cType==1):
                receiveDirMsg(serverName, dataPort)






        
        


    # close TCP connection
    clientSocket.close

if __name__ == '__main__':
    main()