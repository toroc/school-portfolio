/*********************************************************************
** Program Filename: EmptyRoom.hpp
** Author: Carol D. Toro
** Date:  06/08/2015
** Description: This is the EmptyRoom class specification file containing
**				the declarations for the EmptyRoom class.
** Input: n/a
** Output: n/a
*********************************************************************/
#ifndef EMPTYROOM_HPP
#define EMPTYROOM_HPP

#include "../inc/Room.hpp"
#include <string>
using namespace std;

class EmptyRoom :public Room
{
public:
	EmptyRoom();
	~EmptyRoom();
	virtual bool canCheckProgress();
};

#endif
