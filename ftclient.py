#!/usr/bin/python3.2

#------------------------------------------------------------
# CS 372 - Winter 2016 - Program 2
#   By: Carol Toro
#   File Created: 2/16/2016
#   Last Modified: 2/19/2016
#   Filename: ftclient.py
#   Description: This program runs a chat client that connects to chatserver
#   as specified by the IP and PortNumber. A TCP socket is created to perform
#   a 3-way handshake with the chatserver specified. The program prompts
#   the user to enter a handle and an outgoing message to send to the chat
#   server. Reply messages received from the server are displayed on the 
#   console. The user may quit the client by entering the command "\quit"
#   and the connection is closed. If the server closes the connection, 
#   the client is notified and the program is terminated.
#   References:
#    Socket Programming, Section 2.7 from Computer Networking, A Top-Down Approach (6th Edition)
#   
#------------------------------------------------------------
from socket import *
import sys
import argparse




#------------------------------------------------------------
#   Helper functions to print directions, get data from user 
#   and format the data that will be sent via socket connection
#------------------------------------------------------------
def printDirections():
    """Print to console program directions."""
    print("Welcome to the chatclient!\nUse the \quit command to end connection.")

def getHandle():
    """Return a string with at most 10 characters."""
    handle = raw_input('Input a 10 character chatclient handle: ')
    
    # Ask user for shorter handle if over 10 characters
    while (len(handle) > 10):
        handle = raw_input("Handle is too long, please try again: ")
        
    return handle


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
    parser = argparse.ArgumentParser()
    parser.add_argument('SERVER_HOST')
    parser.add_argument('SERVER_PORT')
    parser.add_argument('COMMAND')
    parser.add_argument('FILENAME')
    parser.add_argument('DATA_PORT')
    args = parser.parse_args()


def main():
    argsNeeded()

    #save arguments passed into program
    serverName = str(sys.argv[1])
    serverPort = int(sys.argv[2])

    

    # create client socket with type SOCK_STREAM
    clientSocket = createSocket()

    # initiate handshake and begin TCP connection
    connectSocket(clientSocket, serverName, serverPort)

    printDirections()
    handle = getHandle()

    while(True):

        message = getMessage(handle)
        
        if (message == "\quit"):
            break
        
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

if __name__ == '__main__':
    main()
