/*********************************************************************
** Program Filename: Player.hpp
** Author: Carol D. Toro
** Date:  06/08/2015
** Description: This is the Player class specification file containing
**				the declarations for the Player class.
** Input: n/a
** Output: n/a
*********************************************************************/
#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "../inc/Inventory.hpp"
#include <vector>
#include <string>
using namespace std;

class Player
{
protected:
	string name;
public:
	Inventory itemsDropped;
	Inventory backpack;
	Player();
	int getPoints();
	bool backpackFull();

};

#endif
