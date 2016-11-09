/*********************************************************************
** Program Filename: Map.hpp
** Author: Carol D. Toro
** Date:  06/08/2015
** Description: This is the Map class specification file containing
**				the declarations for the Map class.
** Input: n/a
** Output: n/a
*********************************************************************/
#ifndef MAP_HPP
#define MAP_HPP

#include <string>
#include "../inc/Room.hpp"
#include "../inc/DropRoom.hpp"
#include "../inc/EmptyRoom.hpp"
#include "../inc/DownStairsRoom.hpp"
#include "../inc/UpStairsRoom.hpp"

class Map
{

protected:
	/*Items*/
	Item *pony;
	Item *hoop;
	Item *playground;
	Item *seesaw;
	Item *swing;
	Item *sign;
	Item *fountain;
	Item *plans;
	Item *petition;
	Item *permit;
	
	/*Pawnee Areas*/
	// Lot48 *lot;
	// EmptyRoom *lotSouth;
	// JJDiner *waffle;
	// EmptyRoom *dinerEast;
	// CityHall *cityhall;
	// ParksOffice *office;
	// FourthFloor *fourthFloor;
	// EmptyRoom *officeEast;
	// Cabin *ronsCabin;
	// EmptyRoom *officeSouth;
	// RentSwag *tomSwag;
	// Sandwich *sandwich;
	// Sweetums *sweet;
	// Harvest *festival;
	// Ferris *ferrisWheel;
	// EmptyRoom *sandwichEast;
	// EmptyRoom *sweetEast;
	// Bistro *tomBistro;
	// Unity *concert;
	DropRoom* lot48;
	EmptyRoom *lotSouth;
	Room* jjDiner;
	EmptyRoom* dinerEast;
	Room* cityHall;
	DownStairsRoom* parksOffice;
	UpStairsRoom* fourthFloor;
	EmptyRoom* officeEast;
	Room* ronsCabin;
	EmptyRoom* officeSouth;
	Room* rentSwag;
	Room* sandwich;
	EmptyRoom* sandwichEast;
	Room* sweetums;
	EmptyRoom* sweetEast;
	Room* festival;
	Room* ferrisWheel;
	Room* tomBistro;
	Room* unityConcert;

public:
	Map();
	Room* createMap();
	void loadItems();
	void loadFeedback();
	void loadInventory();
	void loadLocations();
};

#endif