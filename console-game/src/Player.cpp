/*********************************************************************
** Program Filename: Player.cpp
** Author: Carol D. Toro
** Date:  06/08/2015
** Description: This is the Player class implementation file containing
**				the definitions for the Player class.
** Input: n/a
** Output: n/a
*********************************************************************/
#include "../inc/Player.hpp"

Player::Player()
{
	name = "player";
}
/*********************************************************************
** Function: getPoints
** Description: Function returns the number of items dropped from the
**				player's backpack.
** Parameters: n/a
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
int Player::getPoints()
{
	return itemsDropped.items.size();
}
/*********************************************************************
** Function: backpackFull
** Description: Function returns full if the backpack has 5 different
**				types of items.
** Parameters: n/a
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
bool Player::backpackFull()
{
	if (backpack.items.size() == 5)
		return true;
	else
		return false;
}
