/*********************************************************************
** Program Filename: DownStairsRoom.hpp
** Author: Carol D. Toro
** Date:  06/08/2015
** Description: This is the DownStairsRoom class specification file containing
**				the declarations for the DownStairsRoom class.
** Input: n/a
** Output: n/a
*********************************************************************/
#ifndef DOWNSTAIRSROOM_HPP
#define DOWNSTAIRSROOM_HPP

#include "../inc/Room.hpp"
#include <string>
using namespace std;

class DownStairsRoom :public Room
{
protected:
	Room* up;
public:
	DownStairsRoom(string a);
	~DownStairsRoom();
	virtual bool canGoUp();
	virtual Room* getUp();
	void setUp(Room*);
};
#endif
