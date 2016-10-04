CS 372 - Winter 2016 - Program 2
By: Carol Toro
Program Description: File Transfer Server & File Transfer Client

---------------------------------------------------------------------------
README.txt
---------------------------------------------------------------------------

COMPILE Instructions:
     Navigate to the directory containing the files and use the make command:
          make
     This will produce the executable for the File Transfer Server program: ftserver
     
     
EXECUTE Instructions:
     
    Host A ("flip3" server) enter:
        ftserver <Port Number>
          
    Host B ("flip2" server) enter:
        python ftclient.py <Host A> <Host A's Port Number> <Commands> <Host B's Data Port>
          **Prior to executing the ftclient, Host A must be listening for connections.

     EXECUTE:
        HOST A (flip3):
            make
            ftserver 61555
        HOST B (flip2):
            python ftclient.py flip3 61555 -l 42555
            python ftclient.py flip3 61555 -g <filename> 42555

CONTROL Instructions:
    HOST A (ftserver):
        ftserver waits for an incoming connection.
    HOST B (ftclient):
        ftclient sends requests connection with ft server and sends command.
    HOST A (ftserver):
        ftserver receives request and responds to ftclient.
    Host B (ftclient):
     	ftclient receives responde from ftserver and closes connection.


    
     *** After the connection has been closed:
     HOST A (ftserver):
          Continues to wait for a new connection from the ftclient until
          either a connection is accepted or a SIGINT is received. (Ctrl-C)


flip2 ~/GitHub/Networking/ft 25% ftserver 61555
---------------------------------------------------------------------------
EXAMPLE TEST RUN #1 Directory Request: HOST A (ftserver) Console
---------------------------------------------------------------------------

ftserver > now listening for incoming connections on port #61555.

ftserver > received connection from flip3.engr.oregonstate.edu

ftserver > List directory requested on port 42555

ftserver > sending directory contents on port 42555

ftserver > now listening for incoming connections on port #61555.



flip3 ~/GitHub/Networking/ft 25% python ftclient.py flip2 61555 -l 42555
---------------------------------------------------------------------------
EXAMPLE TEST RUN #1 Directory Request: HOST B (ftclient) Console
---------------------------------------------------------------------------

ftclient > control connection established with server flip2 on port 61555

ftclient > sending request to server.

ftclient > receiving directory structure from flip2 on port 42555.

.
..
ftclient.py
ftserver
makefile
ftserver.o
ftserver.cpp

ftclient > transfer is complete.


flip2 ~/GitHub/Networking/ft 56% ftserver 61555
---------------------------------------------------------------------------
EXAMPLE TEST RUN #2 File Request: HOST A (ftserver) Console
---------------------------------------------------------------------------

ftserver > now listening for incoming connections on port #61555.

ftserver > received connection from flip3.engr.oregonstate.edu

ftserver > file ftserver.cpp requested on port 4255

ftserver > sending ftserver.cpp on port 4255

ftserver > done sending ftserver.cpp

ftserver > now listening for incoming connections on port #61555.



flip3 ~/GitHub/Networking/ft 38% python ftclient.py flip2 61555 -g ftserver.cpp 4255 
---------------------------------------------------------------------------
EXAMPLE TEST RUN #2 File Request: HOST B (ftclient) Console
---------------------------------------------------------------------------

ftclient > control connection established with server flip2 on port 61555

ftclient > sending request to server.

ftclient > receiving ftserver.cpp from flip2 on port 4255.

ftclient > duplicate file name for file: ftserver.cpp

ftclient > duplicate file name for file: 1_ftserver.cpp

ftclient > transfer is complete.




flip2 ~/GitHub/Networking/ft 58% ftserver 61555
---------------------------------------------------------------------------
EXAMPLE TEST RUN #3 File Request: HOST A (ftserver) Console
---------------------------------------------------------------------------

ftserver > now listening for incoming connections on port #61555.

ftserver > received connection from flip3.engr.oregonstate.edu

ftserver > file randFile requested on port 4255

ftserver > file requested by client not found.

ftserver > now listening for incoming connections on port #61555.




flip3 ~/GitHub/Networking/ft 40% python ftclient.py flip2 61555 -g randFile 4255
---------------------------------------------------------------------------
EXAMPLE TEST RUN #3 File Request: HOST B (ftclient) Console
---------------------------------------------------------------------------

ftclient > control connection established with server flip2 on port 61555

ftclient > sending request to server.

ftclient > received following message from ftserver: 
	FILE NOT FOUND


