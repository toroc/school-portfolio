#include "../inc/UpStairsRoom.hpp"
/*********************************************************************
** Program Filename: UpStairsRoom.cpp
** Author: Carol D. Toro
** Date:  06/08/2015
** Description: This is the UpStairsRoom class implementation file containing
**				the definitions for the UpStairsRoom class.
** Input: n/a
** Output: n/a
*********************************************************************/

UpStairsRoom::UpStairsRoom(string a)
{
	name = a;
}
UpStairsRoom::~UpStairsRoom()
{

}

void UpStairsRoom::setDown(Room *r)
{
	down = r;
}
Room* UpStairsRoom::getDown()
{
	return down;
}
bool UpStairsRoom::canGoDown()
{
	return true;
}