/*********************************************************************
** Program Filename: Inventory.cpp
** Author: Carol D. Toro
** Date:  06/08/2015
** Description: This is the Inventory class implementation file containing
**				the definitions for the Inventory class.
** Input: n/a
** Output: n/a
*********************************************************************/
#include "../inc/Inventory.hpp"

Inventory::Inventory()
{
	this->items = vector<pair<Item*, int> >();
}

Inventory::Inventory(vector<pair<Item*, int> > i)
{
	this->items = i;
}
/*********************************************************************
** Function: addItem
** Description: function adds item to the inventory
** Parameters: pointer to item, and int count
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
void Inventory::addItem(Item *i, int count)
{
	/*increment count of item if already in inventory*/
	for (vector<pair<Item*, int> >::iterator it = this->items.begin(); it < this->items.end(); ++it)
	{
		if (it->first == i)
		{
			it->second += count;
			return;
		}
	}
	/*Add item if it doesn't exist in inventory*/
	this->items.push_back(make_pair(i, count));
}
/*********************************************************************
** Function: addItem(overloaded)
** Description: function addss an item to the inventory
** Parameters: pair of Item* and int count
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
void Inventory::addItem(pair<Item*, int> i)
{
	for (vector<pair<Item*, int> > ::iterator it = this->items.begin(); it < this->items.end(); ++it)
	{
		if (it->first == i.first)
		{
			it->second += i.second;
			return;
		}
	}

	this->items.push_back(i);
}
/*********************************************************************
** Function: removeItem
** Description: function returns the Item removed from the inventory
** Parameters: n/a
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
pair<Item*, int> Inventory::removeItem()
{
	int size = this->items.size();
	pair<Item*, int> element = this->items.back();
	this->items.pop_back();

	return element;

}
/*********************************************************************
** Function: clear
** Description: function clears all the items from the inventory
** Parameters: n/a
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
void Inventory::clear()
{
	this->items.clear();
}
/*********************************************************************
** Function: printItems
** Description: function prints to console the items in the inventory
** Parameters: n/a
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
int Inventory::printItems()
{
	bool label = false;
	unsigned int i = 1;

	for (vector<pair<Item*, int> > ::iterator it = this->items.begin(); it < this->items.end(); ++it)
	{
		/*# of items*/
		if (label)
		{
			std::cout << i++ << ": ";
		}
		/*Print to Console: item name, qty and description*/
		cout << "" << it->first->name << " (Qty. " << it->second << ") - ";
		cout << it->first->description << endl;
	}

	// Return the number of items outputted, for convenience
	return this->items.size();
}
/*********************************************************************
** Function: isEmpty
** Description: returns true if the inventory is empty
** Parameters: n/a
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
bool Inventory::isEmpty()
{
	if (this->items.empty())
		return true;
	else
		return false;
}
/*********************************************************************
** Function: consolidateItems
** Description: function adds inventory passed in as argument
** Parameters: a pointer to an inventory
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
void Inventory::consolidateItems(Inventory *i)
{
	if (i == this)
	{
		return;
	}
	for (vector<pair<Item*, int> > ::iterator it = i->items.begin(); it < i->items.end(); ++it)
	{
		this->addItem(it->first, it->second);
	}

	return;
}