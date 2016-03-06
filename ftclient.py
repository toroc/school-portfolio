#!/usr/bin/python3.2

#------------------------------------------------------------
# CS 372 - Winter 2016 - Program 2
#   By: Carol Toro
#   File Created: 2/16/2016
#   Last Modified: 03/05/2016
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
#   Socket Programming, Section 2.7 from Computer Networking, A Top-Down Approach (6th Edition)
#   Python Central: Check if a file exists in a directory with Python 
#       http://pythoncentral.io/check-file-exists-in-directory-python/  
#   Stackoverflow: Detect socket hangup without sending or receiving?
#       http://stackoverflow.com/questions/5686490/detect-socket-hangup-without-sending-or-receiving
#   Tutorialspoint: Python Files I/O   
#        http://www.tutorialspoint.com/python/python_files_io.htm
#------------------------------------------------------------
from socket import *
import sys
import os
import argparse



def main():
    """ Function to handle primary flow of program."""
    # ensure user called program with valid args
    validateCommandLine()

    #save arguments passed into program
    serverName = str(sys.argv[1])
    serverPort = int(sys.argv[2])
    command=str(sys.argv[3])
    dataPort = 0
    if(len(sys.argv)==5):
        dataPort=int(sys.argv[4])

    fileName=""

    if(len(sys.argv)==6):
        dataPort=int(sys.argv[5])
        fileName=str(sys.argv[4]);
    
    # Figure out command type
    cType=commandType(command)

    # Create control connection to server SOCK_STREAM
    controlSocket = startControlConnection(serverName, serverPort)

    # Send command to server
    handleControl(controlSocket, serverName, serverPort, command, fileName, dataPort)

    # Start data connection
    handleData(dataPort, cType, serverName, fileName)

    # close control connection
    closeSocket(controlSocket)



#------------------------------------------------------------
#   Helper functions to carry out socket connection processes of
#       initializing socket, connecting to another socket, 
#       accepting an incoming connection, and closing a socket
#------------------------------------------------------------

def createSocket():
    """Return a socket initialized."""
    return socket(AF_INET, SOCK_STREAM)


def connectSocket(sock, name, port):
    """Process a socket connection."""
    sock.connect((name, port))


def acceptSocket(sock):
    """Accept a connected socket."""
    sock.accept()


def closeSocket(sock):
    """Close socket connection."""
    sock.close()


#------------------------------------------------------------
#  Functions to carry out program proccesses to send
#       initializing, connecting, sending, and receiving
#------------------------------------------------------------

def startControlConnection(serverName, serverPort):
    """ Begin control connection with ftserver."""

    # Create client socket of type SOCK_STREAM
    clientSocket=createSocket()

    # Connect to server
    connectSocket(clientSocket, serverName, serverPort)

    # Return the connection
    return clientSocket



def startDataSocket(dataPort):
    """Initialize data socket."""

    # Create server like socket of type SOCK_STREAM
    dataSocket=createSocket()

    # Bind to the data port
    dataSocket.bind(('', dataPort))

    # Returm data connection
    return dataSocket


def handleControl(sock, servName, servPort, command, file, dataPort):
    """ Process flow for control connection."""
    
    # Print to console
    printControlMsg(servName, servPort)

    printReq()
    
    # String of message to send
    msg=commandMsg(command, dataPort, file)

    # Send command to server via control socket
    sendMessage(sock, msg)

    # get reply from server
    msg=getReply(sock)

    # validate reply
    validateControl(msg)



def validateControl(msg):
    """Validate control message from server."""

    if(msg == "OK"):
        return True
    else:
        # Error message
        errMsg(msg)
        # exit since error receive
        exit()


def handleData(dataPort, cType, serverName, filename):
    """ Process flow for data transmission."""

    # Create data connection
    dataSocket = startDataSocket(dataPort)

    # Listen for incoming connection
    dataSocket.listen(1)


    while 1:
        # Accept connection
        connectionSocket, addr = dataSocket.accept()

        # directory list requested
        if(cType==1):

            # print to console
            receiveDirMsg(serverName, dataPort)
            # receive data
            message = connectionSocket.recv(1024)
            # print to console
            printDirContents(message)
            # print to console
            transComplete()
            # exit
            break

        
        # file requested
        if(cType==2):

            # console message
            receiveFileMsg(serverName, filename, dataPort)

            # rPass connection socket to receive file
            receiveFile(connectionSocket, filename)
            # print to console
            transComplete()
            # exit
            break



def receiveFile(sock, filename):
    """Receive file content."""
    # Create list to store file contents
    fileContents = []
    data =''

    # Reference: stackoverflow
    while True:
        # receive 1024 bytes at a time
        data= sock.recv(1024)


        # append data received to fileContents
        fileContents.append(data)

        # connection closed by server
        if len(data) == 0:
            break

    # Call function to save file
    saveFile(fileContents, filename)




def saveFile(fileContents, filename):
    """ Save file contents to file."""
    # Figure out whether duplicate file
    filename = findDuplicate(filename)

    # Create file and open for write
    file = open(filename, "w")

    # Write one line at a time
    for line in fileContents:
        print >> file, line

    
    # close file
    file.close()


def findDuplicate(filename):
    """Determine duplicate file."""
    # Check for duplicate file name
    fileRequested = filename
    fileVersion = 1

    # true when file exists
    while os.path.isfile(filename): #Reference: Python Central

        # console message
        duplicateFile(filename)

        # Change the file name
        filename = str(fileVersion) + "_" + fileRequested 

        # increment version incase it already exists
        fileVersion += 1

    return filename



#------------------------------------------------------------
#   Helper functions to output program messages to console
#   
#------------------------------------------------------------
def printControlMsg(name, control):
    """Print to console message re control socket."""
    print("ftclient > control connection established with server "+ name + " on port "+ str(control)+"\n")
  

def duplicateFile(fname):
    """ ."""
    print("ftclient > duplicate file name for file: "+fname+"\n")

def printReq():
    """ ."""
    print("ftclient > sending request to server.\n")

def receiveDirMsg(server, data):
    """Print message to console."""
    print("ftclient > receiving directory structure from " +server+" on port "+str(data)+".\n")

def receiveFileMsg(server, file, data):
    """."""
    print("ftclient > receiving "+file+" from "+server+" on port "+str(data)+".\n")


def commandType(command):
    """Return type of command"""

    if command[1]=="l":
        return 1
    
    if command[1]=="g":
        return 2
    
    #wrong command 
    return -1 

def printDirContents(msg):
    """Print directory contents to console."""
    print(msg)


def transComplete():
    """Print transfer is complete."""
    print("ftclient > transfer is complete.\n")

def errMsg(msg):
    """Print error message received."""
    print("ftclient > received following message from ftserver: \n\t"+msg)


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
#   Helper function to validate the various arguments the program needs
#       to be able to run
#------------------------------------------------------------
def validateCommandLine():
    """ Ensure user passed correct commands to run program."""
    # Too few arguments
    if len(sys.argv)<5:
        print("Usage: python ftclient.py <SERVER_HOST> <SERVER_PORT> <COMMAND> [<FILENAME>] <DATA_PORT>")
        # exit
        exit()

    # Missing file name, for get command
    if sys.argv[3] == "-g":
        if(len(sys.argv)<6):
            print("Usage: python ftclient.py <SERVER_HOST> <SERVER_PORT> <COMMAND> <FILENAME> <DATA_PORT>")
            # exit
            exit()
 


if __name__ == '__main__':
    main()