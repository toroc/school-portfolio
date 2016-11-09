/*********************************************************************
** Program Filename: GameMaster.hpp
** Author: Carol D. Toro
** Date:  06/08/2015
** Description: This is the GameMaster class specification file containing
**				the declarations for the GameMaster class.
** Input: n/a
** Output: n/a
*********************************************************************/
#ifndef GAMEMASTER_HPP
#define GAMEMASTER_HPP

#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include "../inc/Map.hpp"
#include "../inc/Player.hpp"
#include "../inc/Room.hpp"
#include "../inc/Item.hpp"
#include "../inc/Inventory.hpp"
#include "../inc/EmptyRoom.hpp"
#include "../inc/DropRoom.hpp"
#include "../inc/DownStairsRoom.hpp"
#include "../inc/UpStairsRoom.hpp"

class GameMaster
{
protected:
	Map gameMap;
	Player *p;
public:
	GameMaster();
	void begin();
	void displayMenu(Room *);
	void getUserMove(Room* );
	void moveNorth(Room*);
	void moveEast(Room*);
	void moveSouth(Room*);
	void moveWest(Room*);
	void moveUp(Room*);
	void moveDown(Room*);
	void getWhereAmI(Room *);
	void getBackpack(Room *);
	void dropOffBackpack(Room *);
	void playerProgress();
	void gameWon();
	void printSeparator();
	void printAreaInfo(Room*);
	void printGameIntro();
	void printGMSeparator();
};

#endif 