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
<<<<<<< HEAD
#   Helper functions to output program messages to console
#   
#------------------------------------------------------------
def printControlMsg(name, control):
    """Print to console message re control socket."""
    print("Control connection established with server "+ name + " on port "+ str(control)+"\n")
    print("Sending request.\n")  

def receiveDirMsg(server, data):
    """Print message to console."""
    Print("Receiving directory contents from " +server+":"+str(data)+"\n")

def receiveFileMsg(server, file, data):
    """."""
    Print("Receiving "+file+" from "+server+":"+data+"\n")


def commandType(command):
    """Return type of command"""
=======
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
>>>>>>> ba63d5303123062252458ba5f19bd558cdc473c1

    if command[1]=="l":
        print("Requesting list\n")
        return 2
    
    if command[1]=="g":
        print("Getting file")
        return 1
<<<<<<< HEAD
    
=======
>>>>>>> ba63d5303123062252458ba5f19bd558cdc473c1
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


<<<<<<< HEAD
def listenSocket(sock, port):
    """Bind dataSocket to port."""
    # Get local name
    host = socket.gethostname()
    # Bind to the port     
    sock.bind((host, port))

    Print("Waiting for server response on port "+str(port)+"\n")

#------------------------------------------------------------
#   Functions to carry out printing messages to the console
#------------------------------------------------------------

def handleControl(sock, servName, servPort, command, file, dataPort):

    printControlMsg(servName, servPort)
    
    # String of message to send
    msg=commandMsg(command,file, dataPort)

    # Send command to server via control socket
    sendMessage(sock, msg)



#------------------------------------------------------------
#   Functions to carry out socket connection processes of
#       initializing, connecting, sending, and receiving
#------------------------------------------------------------

def commandMsg(comm, port,file, type):
    """Return stylized string of command message"""
    message=comm+" "+file+" "+str(port)

    return message


=======
>>>>>>> ba63d5303123062252458ba5f19bd558cdc473c1
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
<<<<<<< HEAD
    filename=""
=======
    filename
>>>>>>> ba63d5303123062252458ba5f19bd558cdc473c1

    if(len(sys.argv)==6):
        dataPort=int(sys.argv[5])
        filename=str(sys.argv[4]);
    
    # DEBUG: Figure out command type
    cType=commandType(command)

    # Create control connection to server SOCK_STREAM
<<<<<<< HEAD
    controlSocket = createSocket()

    # initiate handshake and begin TCP connection
    connectSocket(controlSocket, serverName, serverPort)
=======
    clientSocket = createSocket()

    # initiate handshake and begin TCP connection
    connectSocket(clientSocket, serverName, serverPort)
>>>>>>> ba63d5303123062252458ba5f19bd558cdc473c1

   
    

    while(True):

<<<<<<< HEAD
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






        
=======
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
>>>>>>> ba63d5303123062252458ba5f19bd558cdc473c1
        


    # close TCP connection
    clientSocket.close

