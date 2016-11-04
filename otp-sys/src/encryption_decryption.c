#include "../inc/encryption_decryption.h"
/******************************************************
#   Encryption & Helper Functions
******************************************************/

/******************************************************
#   charNum
#   @desc: calculate numerical val of ASCII char
#   @param: char c
#   @return: numerical value of ASCII char
******************************************************/
int charNum(char c)
{
	if (c == 32)
	{
		return 26;
	}

	return c - 65;

}
/******************************************************
#   charNum
#   @desc: calculate ASCII char corresponding to val
#   @param: int val
#   @return: ASCII char
******************************************************/
char numChar(int val)
{
	int i;
	char c;

	if (val <26)
	{
		i = val + 65;
		c = i;

	}
	else
	{
		/*Space character*/
		i = 32;
		c = i;
	}

	return c;
}
/******************************************************
#   encodeChar
#   @desc: encode msg char based on key char
#   @param: msg char key char
#   @return: encoded char
******************************************************/
char encodeChar(char msgChar, char keyChar)

{
	int msgVal = charNum(msgChar);
	int keyVal = charNum(keyChar);

	int charVal = (msgVal + keyVal) % 27;

	char c = numChar(charVal);

	return c;
}


/******************************************************
#   decodeChar
#   @desc: decode msg char based on key char
#   @param: msg char key char
#   @return: encoded char
******************************************************/
char decodeChar(char msgChar, char keyChar)

{
	int msgVal = charNum(msgChar);
	int keyVal = charNum(keyChar);

	int charVal = (msgVal - keyVal) % 27;

	/*Check if charVal is negative*/
	if (charVal < 0){
		charVal += 27;
	}

	/*Get numerical ASCII value of charVal*/
	char c = numChar(charVal);

	return c;
}