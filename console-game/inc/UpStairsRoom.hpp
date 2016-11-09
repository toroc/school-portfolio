/*********************************************************************
** Program Filename: UpStairsRoom.hpp
** Author: Carol D. Toro
** Date:  06/08/2015
** Description: This is the UpStairsRoom class specification file containing
**				the declarations for the UpStairsRoom class.
** Input: n/a
** Output: n/a
*********************************************************************/
#ifndef UPSTAIRSROOM_HPP
#define UPSTAIRSROOM_HPP

#include "../inc/Room.hpp"
#include <string>
using namespace std;

class UpStairsRoom :public Room
{
protected:
	Room *down;
public:
	UpStairsRoom(string a);
	~UpStairsRoom();
	virtual string getFirstDesc();
	virtual string getRevisitDesc();
	virtual Room* getDown();
	virtual bool canGoDown();
	void setDown(Room*);
};
#endif
