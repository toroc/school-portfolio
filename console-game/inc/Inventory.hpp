/*********************************************************************
** Program Filename: Inventory.hpp
** Author: Carol D. Toro
** Date:  06/08/2015
** Description: This is the Inventory class specification file containing
**				the declarations for the Inventory class.
** Input: n/a
** Output: n/a
*********************************************************************/
#ifndef INVENTORY_HPP
#define INVENTORY_HPP
#include "../inc/Item.hpp"
#include <vector>
#include <iostream>
#include <utility>
using namespace std;

class Inventory
{
public:
	vector<pair<Item*, int> > items;
	Inventory();
	Inventory(int);
	Inventory(vector<pair<Item*, int> > i);
	void addItem(Item* i, int count);
	void addItem(pair<Item*, int>);
	pair<Item*, int> removeItem();
	void clear();
	bool isEmpty();
	int printItems();
	void consolidateItems(Inventory *);

};
#endif