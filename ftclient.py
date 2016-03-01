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
import os
import argparse


#------------------------------------------------------------
#   Helper functions to carry out socket connection processes of
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

    sock.listen(1)

def acceptSocket(sock):

    sock.accept()


def getMessage(sock): 
    msg = getReply(sock)

    print(msg.decode())

def sendMessage(sock, msg):
    # Send the message
    sock.sendall(msg)


def receiveMessage(sock):

    message = recvAll(sock)

    return message



#------------------------------------------------------------
#  Helper Functions to carry out socket connection processes of
#       initializing, connecting, sending, and receiving
#------------------------------------------------------------

def startControlConnection(serverName, serverPort):

    print("inside startControlConnection")

    # Create client socket of type SOCK_STREAM
    clientSocket=createSocket()

    # Connect to server
    connectSocket(clientSocket, serverName, serverPort)

    # Return the connection
    return clientSocket



def startDataSocket(dataPort):

    print("inside startDataSocket")
    # Create server like socket of type SOCK_STREAM
    dataSocket=createSocket()

    # Bind to the data port
    dataSocket.bind(('', dataPort))




    # Returm data connection
    return dataSocket


def handleControl(sock, servName, servPort, command, file, dataPort):
    # Print to console

    print("inside handleControl")
    printControlMsg(servName, servPort)

    printReq()
    
    # String of message to send
    msg=commandMsg(command, dataPort, file)

    # Send command to server via control socket
    sendMessage(sock, msg)

    msg=getReply(sock)

    # validate reply
    validateControl(msg)


def validateControl(msg):

    if(msg == "OK"):
        print("Message received by server")
    else:
        # Error message
        print(msg)


def handleData(dataPort, cType, serverName, filename):

    print("inside handleData")
    # Create data connection
    dataSocket = startDataSocket(dataPort)

    # Listen for incoming connection
    dataSocket.listen(1)

    print("Data socket is ready to receive")

    while 1:
        print("inside the while")
        connectionSocket, addr = dataSocket.accept()
        print("after the accept")
         # Print to console
         if(cType==1):
            receiveDirMsg(serverName, dataPort)
            message = connectionSocket.recv(1024)
            printDirContents(message)
            break

        if(cType==2):
            receiveFileMsg(serverName, fileName, dataPort)
            # receive greater quantity

            # handle saving file


   # close connection


#------------------------------------------------------------
#   Helper functions to output program messages to console
#   
#------------------------------------------------------------
def printControlMsg(name, control):
    """Print to console message re control socket."""
    print("ftclient > control connection established with server "+ name + " on port "+ str(control)+"\n")
  

def printReq():

    print("ftclient > sending request to server. ")

def receiveDirMsg(server, data):
    """Print message to console."""
    print("Receiving directory structure from " +server+":"+str(data)+"\n")

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

def printDirContents(msg):

    print(msg)







def printMessage(message):
    print(message.encode)


def commandMsg(comm, port,file):
    """Return stylized string of command message"""
    message = comm+" "+file+" "+str(port)

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
def validateCommandLine():

    # Too few arguments
    if len(sys.argv)<5:
        print("Usage: python ftclient.py <SERVER_HOST> <SERVER_PORT> <COMMAND> [<FILENAME>] <DATA_PORT>")

    # Missing file name, for get command
    if sys.argv[3] == "-g":
        if(len(sys.argv)<6):
            print("Usage: python ftclient.py <SERVER_HOST> <SERVER_PORT> <COMMAND> <FILENAME> <DATA_PORT>")
 


def main():
    
    validateCommandLine()

    #save arguments passed into program
    serverName = str(sys.argv[1])
    serverPort = int(sys.argv[2])
    command=str(sys.argv[3])
    dataPort=int(sys.argv[4])
    fileName=""

    if(len(sys.argv)==6):
        dataPort=int(sys.argv[5])
        fileName=str(sys.argv[4]);
    
    # DEBUG: Figure out command type
    cType=commandType(command)

    # Create control connection to server SOCK_STREAM
    controlSocket = startControlConnection(serverName, serverPort)

    # Send command to server
    handleControl(controlSocket, serverName, serverPort, command, fileName, dataPort)

    # Start data connection
    handleData(dataPort, cType, serverName, fileName)



if __name__ == '__main__':
    main()