#File Transfer System

##About
This is a simple one-way file transfer system written for a Networking Course.

The client side of the system is written in Python. It supports  the request of UNIX file or a list of available files.

The server side of the system is written in C++. It supports sending unix files to the client side of the system. When a transfer completes, the server side listens for new incoming connections.



##Configuration

###How to build
The build process is controlled by the `makefile` and accepts the following commands:
```
make
make clean
```
To build the server side of the system run make at the command line in the current directory.

This will generate an executabled called `ftserver`.

###How to run

To turn on the server side part of the system on Host A, type
```
ftserver <Command Port #>
```

To turn on the client side of the system on Host B and request the list of available files from Host A, type
```
python src/ftclient.py <Host Name> <Command Port #> -l <Data Port #>
```

To turn on the client side of the system on Host B and request a specific file from Host A, type
```
python src/ftclient.py <Host Name> <Command Pord #> -f <File Name> <Data Port #>
```

###Control Instructions
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


##Sample Runs

####EXAMPLE TEST RUN #1 Directory Request: HOST A (ftserver) Console
```
flip2 ~/GitHub/Networking/ft 25% ftserver 61555

    ftserver > now listening for incoming connections on port #61555.

    ftserver > received connection from flip3.engr.oregonstate.edu

    ftserver > List directory requested on port 42555

    ftserver > sending directory contents on port 42555

    ftserver > now listening for incoming connections on port #61555.
```

####EXAMPLE TEST RUN #1 Directory Request: HOST B (ftclient) Console
```
flip3 ~/GitHub/Networking/ft 25% python ftclient.py flip2 61555 -l 42555

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
```


####EXAMPLE TEST RUN #2 File Request: HOST A (ftserver) Console
```
flip2 ~/GitHub/Networking/ft 56% ftserver 61555

    ftserver > now listening for incoming connections on port #61555.

    ftserver > received connection from flip3.engr.oregonstate.edu

    ftserver > file ftserver.cpp requested on port 4255

    ftserver > sending ftserver.cpp on port 4255

    ftserver > done sending ftserver.cpp

    ftserver > now listening for incoming connections on port #61555.
```

####EXAMPLE TEST RUN #2 File Request: HOST B (ftclient) Console
```
flip3 ~/GitHub/Networking/ft 38% python ftclient.py flip2 61555 -g ftserver.cpp 4255 

    ftclient > control connection established with server flip2 on port 61555

    ftclient > sending request to server.

    ftclient > receiving ftserver.cpp from flip2 on port 4255.

    ftclient > duplicate file name for file: ftserver.cpp

    ftclient > duplicate file name for file: 1_ftserver.cpp

    ftclient > transfer is complete.
```

####EXAMPLE TEST RUN #3 File Request: HOST A (ftserver) Console
```
flip2 ~/GitHub/Networking/ft 58% ftserver 61555

    ftserver > now listening for incoming connections on port #61555.

    ftserver > received connection from flip3.engr.oregonstate.edu

    ftserver > file randFile requested on port 4255

    ftserver > file requested by client not found.

    ftserver > now listening for incoming connections on port #61555.

```
####EXAMPLE TEST RUN #3 File Request: HOST B (ftclient) Console
```
flip3 ~/GitHub/Networking/ft 40% python ftclient.py flip2 61555 -g randFile 4255

    ftclient > control connection established with server flip2 on port 61555

    ftclient > sending request to server.

    ftclient > received following message from ftserver: 
        FILE NOT FOUND
```


##License
All rights reserved.




     
     
