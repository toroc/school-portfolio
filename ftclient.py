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



if __name__ == '__main__':
    main()

#------------------------------------------------------------
#   Helper functions to print directions, get data from user 
#   and format the data that will be sent via socket connection
#------------------------------------------------------------
def printDirections(name, control):
    """Print to console program directions."""
    print("Control connection established with server "+ name + " on port "+ str(control)+"\n")
    print("Sending request.\n")

def buildMessage()


def getMessage(handle):
    """Return a string User wants to send."""
    curMessage = raw_input(handle + "> ")

    # Ask user for shorter message if over 500 characters
    while(len(curMessage) > 500):
        curMessage = raw_input("Message too long, try again:\n" + handle + "> ")

    return curMessage
    

def prepend(handle, message):
    """Return a stylized string of user's message."""
    newMessage = handle + "> " + message
    return newMessage

def commandType(command):
    """Print type of command received"""

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
def argsNeeded():
    """Ensure program is called with correct arguments."""
    parser = argparse.ArgumentParser(description='Simple file transfer client.')
    parser.add_argument('SERVER_HOST')
    parser.add_argument('SERVER_PORT')
    parser.add_argument('COMMAND')
    parser.add_argument('-FILENAME')
    parser.add_argument('DATA_PORT')
    args = parser.parse_args()


def main():
    argsNeeded()

    # 
    #save arguments passed into program
    serverName = str(sys.argv[1])
    serverPort = int(sys.argv[2])
    command=str(sys.argv[3])
    dataPort=int(sys.argv[4])
    filename

    if(len(sys.argv)==6):
        dataPort=int(sys.argv[5])
        filename=str(sys.argv[4]);
    
    # DEBUG: Figure out command type
    cType=commandType(command)

    # Create control connection to server SOCK_STREAM
    clientSocket = createSocket()

    # initiate handshake and begin TCP connection
    connectSocket(clientSocket, serverName, serverPort)

   
    

    while(True):

        printDirections()

        # Send command to server
        message = getMessage(handle)
        
        
        
        # stylize message to send with handle and >
        mToSend = prepend(handle, message)

        # send message via TCP connection
        sendMessage(clientSocket, mToSend)
       
        receivedMessage = getReply(clientSocket)
        
        # close connection whenever server \quits their connection
        amountReceived = len(receivedMessage)

        if (amountReceived < 1):
            print("Uh-oh, chatserve closed the connection. Program will exit.")
            break
        else:
            print(receivedMessage.decode())
        


    # close TCP connection
    clientSocket.close

