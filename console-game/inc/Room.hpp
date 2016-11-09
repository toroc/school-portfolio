
#ifndef ROOM_HPP
#define ROOM_HPP

/*********************************************************************
** Program Filename: Room.hpp
** Author: Carol D. Toro
** Date:  06/08/2015
** Description: This is the abstract Room class specification file containing
**				the declarations for the Room class.
** Input: n/a
** Output: n/a
*********************************************************************/

#include "../inc/Inventory.hpp"
#include "../inc/Player.hpp"

#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Room
{
protected:
	string name;
	Room *north, *east, *south, *west;
	bool visited;
	string description;
	

public:
	// Constructor
	Inventory *items;
	string location;
	string feedback;
	Room();
	Room(std::string);
	//Room::Room(std::string a, Room* n=NULL, Room* s=NULL, Room* e=NULL, Room* w=NULL);
	~Room();
	void setName(std::string a);
	void setDescription(std::string d);
	std::string getName();
	string getDesc();
	string getFirstDesc();
	string getRevisitDesc();

	// Setters for room pointers
	void setNorth(Room* n);
	void setEast(Room* e);
	void setSouth(Room *s);
	void setWest(Room* w);
	
	// Getters for room pointers
	Room* getNorth();
	Room* getEast();
	Room* getSouth();
	Room* getWest();

	void setItems(Item* i);
	virtual bool canCollectItems();
	virtual bool canCheckProgress();
	virtual bool canSearchRoom();
	virtual bool canGoUp();
	virtual bool canGoDown();
	virtual void dropItems(Player *);
	virtual bool gameWon();
	virtual Room* getUp();
	virtual Room* getDown();
	void searchRoom(Player *);
	void checkProgress(Player *);
};
#endif