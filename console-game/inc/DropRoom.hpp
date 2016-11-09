/*********************************************************************
** Program Filename: Lot48.hpp
** Author: Carol D. Toro
** Date:  06/08/2015
** Description: This is the Lot48 class specification file containing
**				the declarations for the Lot48 class.
** Input: n/a
** Output: n/a
*********************************************************************/
#ifndef DROPROOM_HPP
#define DROPROOM_HPP

#include "../inc/Room.hpp"
#include "../inc/Player.hpp"
#include <string>
using namespace std;

class DropRoom :public Room
{
public:
	Inventory *needed;
	Inventory *dropped;
	int droppedCount;
	DropRoom(string a);
	~DropRoom();
	virtual string getFirstDesc();
	virtual string getRevisitDesc();
	virtual bool canCollectItems();
	virtual bool canSearchRoom();
	virtual bool gameWon();
	virtual void dropItems(Player *);
};
#endif
