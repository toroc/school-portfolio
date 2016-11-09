/*********************************************************************
** Program Filename: Item.hpp
** Author: Carol D. Toro
** Date:  06/08/2015
** Description: This is the Item class specification file containing
**				the declarations for the Item class.
** Input: n/a
** Output: n/a
*********************************************************************/
#ifndef ITEM_HPP
#define ITEM_HPP
#include <string>
using namespace std;

class Item
{
public:
	string name;
	string description;
	Item();
	~Item();
	Item(string n, string d);
	bool equal(Item *n);
};
#endif
