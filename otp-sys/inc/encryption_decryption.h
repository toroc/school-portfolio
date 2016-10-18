/******************************************************
#   CS 344 - Winter 2016 - Program 4
#   By: Carol Toro
#   File Created: 10/17/2016
#   Last Modified: 10/17/2016
#   Filename: encryption.h
#   Description: Decryption and Encryption definitions.
#
#
******************************************************/

/*Encryption Function & Helper Functions*/
int charNum(char c);
char numChar(int val);
char encodeChar(char msgChar, char keyChar);
char decodeChar(char msgChar, char keyChar);