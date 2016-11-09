/*********************************************************************
** Program Filename: DropRoom.cpp
** Author: Carol D. Toro
** Date:  06/08/2015
** Description: This is the DropRoom class implementation file containing
**				the definitions for the DropRoom class.
** Input: n/a
** Output: n/a
*********************************************************************/
#include "../inc/DropRoom.hpp"

DropRoom::DropRoom(string a)
{
	name = a;
	droppedCount = 0;
	needed = new Inventory;
	dropped = new Inventory;
	
}
DropRoom::~DropRoom()
{

}

/*********************************************************************
** Function: gameWon
** Description: Returns true if the dropped items is equal to the items
**				needed to win the game
** Parameters: n/a
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
bool DropRoom::gameWon()
{
	
	if (dropped->items.size() != needed->items.size())
	{
		return false;
	}

	for (size_t i = 0; i < needed->items.size(); i++)
		{
			bool notFound = true;

			for (size_t j = 0; j < dropped->items.size(); j++)
			{
				if (needed->items.at(i).first->equal(dropped->items.at(j).first))
				{
					if (needed->items.at(i).second == dropped->items.at(j).second)
					{
						notFound = false;
						break;
					}
					else
					{
						return false;
					}
				}
			}
			if (notFound)
			{
				return false;
			}
		}
		return true;
}
/*********************************************************************
** Function: dropItems
** Description: Adds items from player's backpack to the dropped inventory
** Parameters: pointer to player
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
void DropRoom::dropItems(Player *p)
{
	unsigned int size = p->backpack.items.size();
	for (size_t i = 0; i < size; i++)
	{
		pair<Item*, int> x = p->backpack.removeItem();
		dropped->addItem(x.first, x.second);
		p->itemsDropped.addItem(x.first, x.second);
		droppedCount += 1;
	}
	cout << "So far you have dropped off the following items: " << endl;
	dropped->printItems();
	bool checkStatus=gameWon();
	
}
/*********************************************************************
** Function: canCollectItems
** Description: returns true if area has ability to collect items
** Parameters: n/a
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
bool DropRoom::canCollectItems()
{
	return true;
}
/*********************************************************************
** Function: canSearchRoom
** Description: returns true if area has ability to be search
** Parameters: n/a
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
bool DropRoom::canSearchRoom()
{
	return false;
}
