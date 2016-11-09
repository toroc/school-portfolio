/*********************************************************************
** Program Filename: Item.cpp
** Author: Carol D. Toro
** Date:  06/08/2015
** Description: This is the Item class implementation file containing
**				the definitions for the Item class.
** Input: n/a
** Output: n/a
*********************************************************************/
#include "../inc/Item.hpp"

Item::Item(string n, string d)
{
	this->name = n;
	this->description = d;
}
Item::~Item()
{

}
/*********************************************************************
** Function: equal
** Description: override the equal operator
** Parameters: n/a
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
bool Item::equal(Item *n)
{
	if (n == NULL)
	{
		return false;
	}

	if (this->name == n->name)
		return true;
	else
		return false;
}