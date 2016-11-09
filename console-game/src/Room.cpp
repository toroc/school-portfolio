/*********************************************************************
** Program Filename: Room.cpp
** Author: Carol D. Toro
** Date:  06/08/2015
** Description: This is the Room class implementation file containing
**				the definitions for the Room class.
** Input: n/a
** Output: n/a
*********************************************************************/
#include "../inc/Room.hpp"

Room::Room(std::string a)
{
	name = a;
	description = "";
	// north = n;
	// south = s;
	// east = e;
	// west = w;
	items = new Inventory;
}

Room::~Room()
{

}

void Room::setName(std::string a)
{
	name = a;
}
void Room::setDescription(std::string d)
{
	description = d;
}
void Room::setNorth(Room *n)
{
	north = n;
}
void Room::setEast(Room *e)
{
	east = e;
}
void Room::setSouth(Room *s)
{
	south = s;
}
void Room::setWest(Room *w)
{
	west = w;
}
Room* Room::getNorth()
{
	return north;
}
Room* Room::getEast()
{
	return east;
}
Room* Room::getSouth()
{
	return south;
}
Room* Room::getWest()
{
	return west;
}

/*********************************************************************
** Function: getDesc
** Description: returns a string with the description of the area
** Parameters: n/a
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
string Room::getDesc()
{
	if (!visited)
	{
		visited = true;
		return getFirstDesc();
	}
	else
	{
		return getRevisitDesc();
	}
}
/*********************************************************************
** Function:
** Description:
** Parameters: n/a
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
void Room::searchRoom(Player *p)
{
	if (items->isEmpty())
	{
		cout << "  There are no items in this area." << endl;
	}
	else
	{
		if (!p->backpackFull())
		{
			cout << "  Great job searching the area! You have found: " << endl;
			items->printItems();

			(*p).backpack.consolidateItems(items);
			items->clear();

			return;
		}
		else
		{
			cout << "Your backpack is full and you must drop off items on Lot48!" << endl;
		}
	}
	
	
}
void Room::dropItems(Player *p)
{
	
}
string Room::getName()
{
	return name;
}

string Room::getFirstDesc()
{
	return "Welcome to " + name + "! " + description + feedback;
}

string Room::getRevisitDesc()
{
	return "Welcome back to " + name + ".";
}
/*********************************************************************
** Function: canSearchRoom
** Description: returns true if area has ability to be search
** Parameters: n/a
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
bool Room::canSearchRoom()
{
	return true;
}
/*********************************************************************
** Function: canCollectItems
** Description: returns true if area has ability to collect items
** Parameters: n/a
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
bool Room::canCollectItems()
{
	return false;
}
/*********************************************************************
** Function: canCheckProgress
** Description: returns true if area has ability to show progress
** Parameters: n/a
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
bool Room::canCheckProgress()
{
	return false;
}
/*********************************************************************
** Function: gameWon
** Description: returns true if game has been won
** Parameters: n/a
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
bool Room::gameWon()
{
	return false;
}
/*********************************************************************
** Function: canGoUp
** Description: returns true if area has ability to go upstairs
** Parameters: n/a
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
bool Room::canGoUp()
{
	return false;
}
/*********************************************************************
** Function: canGoDown
** Description: returns true if area has ability to go downstairs
** Parameters: n/a
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
bool Room::canGoDown()
{
	return false;
}
Room* Room::getUp()
{
	return NULL;
}
Room* Room::getDown()
{
	return NULL;
}