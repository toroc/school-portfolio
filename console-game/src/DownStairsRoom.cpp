/*********************************************************************
** Program Filename: ParkOffice.cpp
** Author: Carol D. Toro
** Date:  06/08/2015
** Description: This is the ParkOffice class implementation file containing
**				the definitions for the ParkOffice class.
** Input: n/a
** Output: n/a
*********************************************************************/
#include "../inc/DownStairsRoom.hpp"

DownStairsRoom::DownStairsRoom(string a)
{
	name = a;
}
DownStairsRoom::~DownStairsRoom()
{

}

void DownStairsRoom::setUp(Room *r)
{
	up = r;
}
bool DownStairsRoom::canGoUp()
{
	return true;
}
Room* DownStairsRoom::getUp()
{
	return up;
}