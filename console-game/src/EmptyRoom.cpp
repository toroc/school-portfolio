/*********************************************************************
** Program Filename: EmptyRoom.cpp
** Author: Carol D. Toro
** Date:  06/08/2015
** Description: This is the EmptyRoom class implementation file containing
**				the definitions for the EmptyRoom class.
** Input: n/a
** Output: n/a
*********************************************************************/
#include "../inc/EmptyRoom.hpp"

EmptyRoom::EmptyRoom()
{
	name = "The Streets of Pawnee Indiana";
	description = "Don't you have somewhere to go?";
}
EmptyRoom::~EmptyRoom()
{

}


/*********************************************************************
** Function: canCheckProgress
** Description: returns true if area has ability to show progress
** Parameters: n/a
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
bool EmptyRoom::canCheckProgress()
{
	return true;
}
