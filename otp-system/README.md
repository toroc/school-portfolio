#One Time Password System

##About
These are programs that can encrypt and decrypt text similar to using a one-time password system. These programs were written for an Operating Systems course.

The programs utilize a client-server architecture to accomplish decryption and encryption.  The program consists of 2 clients and 2 servers. Each client handles taking in a file along with key file and sending it to its respective server for encryption or decryption via a socket. Each server performs its respective task and sends it back to the client via a socket. Servers may run in the background and accept multiple connections.

Both the client and server side of the programs are written in C. The programs support unix files as long all characters in the file are valid.


##Configuration

###How to build
The build process is controlled by the `makefile` and accepts the following commands:
```
make
make clean
```
To build the server side of the system run make at the command line in the current directory.

This will generate 5 executables:
keygen_app
client_encrypt_app
server_encrypt_app
client_decrypt_app
server_decrypt_app


###How to generate random keys

To generate a key file to use for encryption/decryption, type
```
keygen_app <Key Length> < <File Name>
```

###How to run encryption app

To turn on the server side part of the encryption system and run it in the background, type
```
server_encrypt_app <Encryption Port #> &
```

To turn on the client side of the system and request the encryption of a file with a keyfile
```
client_encrypt_app <File Name> <Key File Name> <Encryption Port #>
```



###How to run decryption app

To turn on the server side part of the decryption system and run it in the background, type
```
server_decrypt_app <Decryption Port #> &
```

To turn on the client side of the decryption system and request the decryption of a file with a keyfile
```
client_decrypt_app <File Name> <Key File Name> <Decryption Port #>
```



##Run Tests

Ensure that all test files have proper permisions for running.
```
chmod +x test_*
```
To run an individual test, type
```
./test_<Test Name>
```
To run all tests, type
```
./test_all <Encryption Port> <Decryption Port>
```


##License
All rights reserved.




     
     
