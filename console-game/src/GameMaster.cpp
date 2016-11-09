/*********************************************************************
** Program Filename: GameMaster.cpp
** Author: Carol D. Toro
** Date:  06/08/2015
** Description: This is the GameMaster class implementation file containing
**				the definitions for the GameMaster class.
** Input: n/a
** Output: n/a
*********************************************************************/
#include "../inc/GameMaster.hpp"
#include <iostream>
#include <iomanip>
using namespace std;

GameMaster::GameMaster()
{
	p = new Player;
}
/*********************************************************************
** Function: diplayMenu
** Description: This function prints to the console the default menu, 
**				along with more specialized options that only certain
**				rooms have.
** Parameters: pointer to a room
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
void GameMaster::displayMenu(Room *currentRoom)
{
	printGMSeparator();
	cout << "GameMaster: ";
	cout << "   Game Options" << endl;
	cout << "\t0. Where am I?" << endl;
	cout << "\t1. Walk North" << endl;
	cout << "\t2. Walk East" << endl;
	cout << "\t3. Walk South" << endl;
	cout << "\t4. Walk West" << endl;
	cout << "\t5. View contents of backpack" << endl;
	if (currentRoom->canSearchRoom())
	{
		cout << "\t6. Search area for items" << endl;
	}

	if (currentRoom->canCollectItems())
	{
		cout << "\t6. Drop off contents from backpack." << endl;
	}
	else if (currentRoom->canCheckProgress())
	{
		cout << "\t7. Check your progress." << endl;
	}
	if (currentRoom->canGoUp())
	{
		cout << "\t7. Take elevator upstairs" << endl;
	}
	if (currentRoom->canGoDown())
	{
		cout << "\t7. Take elevator downstairs" << endl;
	}

}
/*********************************************************************
** Function: begin
** Description: This function begins the game.
** Parameters: n/a
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
void GameMaster::begin()
{
	char user = ' ';
	printGameIntro();

	cout << "\tGame Master: Are you ready to play? (y/n) ";
	cin >> user;

	if (user == 'y' || user == 'Y')
	{
		Room *currentRoom;
		currentRoom = gameMap.createMap();

		printAreaInfo(currentRoom);
		getUserMove(currentRoom);
	}
	else
	{
		cout << "\tGame Master: Come back later when you're ready to play. " << endl;
		cout << "\tGame Master: Good bye!" << endl;
		exit(0);
	}
	return;

}
/*********************************************************************
** Function: getUserMove
** Description: this function carries out the moves of the game.
** Parameters: a pointer to a room
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
void GameMaster::getUserMove(Room *currentRoom)
{
	while (!currentRoom->gameWon())
	{
		int userMove = 0;
		displayMenu(currentRoom);
		
		cout << "\n\tGame Master: What would you like to do? ";
		cin >> userMove;

		if (userMove == 0)/*Get Area Description*/
		{
			getWhereAmI(currentRoom);
		}
		else if (userMove == 1) /*Move North*/
		{
			moveNorth(currentRoom);
		}
		else if (userMove == 2) /*Move East*/
		{
			moveEast(currentRoom);
		}
		else if (userMove == 3) /*Move South*/
		{
			moveSouth(currentRoom);
		}
		else if (userMove == 4) /*Move West*/
		{
			moveWest(currentRoom);
		}
		else if (userMove == 5) /*View backpack contents*/
		{
			getBackpack(currentRoom);
		}

		if (currentRoom->canSearchRoom() && userMove == 6) /*Search the room*/
		{
			currentRoom->searchRoom(p);
			getUserMove(currentRoom);
		}

		if (currentRoom->canCollectItems() && userMove == 6) /*Drop off backpack contents*/
		{
			dropOffBackpack(currentRoom);
		}
		else if (currentRoom->canCheckProgress() && userMove == 7) /*Get Progress*/
		{
			playerProgress();
			getUserMove(currentRoom);
		}
		
		if (currentRoom->canGoUp() && userMove == 7)/*Take Elevator Upstairs*/
		{
			moveUp(currentRoom);
		}
		else if (currentRoom->canGoDown() && userMove == 7) /*Take Elevator downstairs*/
		{
			moveDown(currentRoom);
		}
	}

	if (currentRoom->gameWon())
	{
		gameWon();
		exit(0);
	}
	return;
}
/*********************************************************************
** Function: moveNorth
** Description: This function allows a player to travel North
** Parameters: a pointer to a room
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
void GameMaster::moveNorth(Room *currentRoom)
{
	Room * currRoom = currentRoom->getNorth();
	if (currRoom != NULL)
	{
		printAreaInfo(currRoom);
		getUserMove(currRoom);
	}
	else 
	{
		cout << "  Game Master: Try again you cannot go North from " << currentRoom->getName() << endl;
		getUserMove(currentRoom);
	}
	return;

}
/*********************************************************************
** Function: moveEast
** Description: This function allows a player to travel East
** Parameters: a pointer to a room
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
void GameMaster::moveEast(Room *currentRoom)
{
	Room * currRoom = currentRoom->getEast();
	if (currRoom != NULL)
	{
		printAreaInfo(currRoom);
		getUserMove(currRoom);
	}
	else
	{
		cout << "  Game Master: Try again you cannot go East from " << currentRoom->getName() << endl;
		getUserMove(currentRoom);
	}
	return;
}
/*********************************************************************
** Function: moveSouth
** Description: This function allows a player to travel South
** Parameters: a pointer to a room
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
void GameMaster::moveSouth(Room *currentRoom)
{
	Room * currRoom = currentRoom->getSouth();
	if (currRoom != NULL)
	{
		printAreaInfo(currRoom);
		getUserMove(currRoom);
	}
	else
	{
		cout << "  Game Master: Try again you cannot go South from " << currentRoom->getName() << endl;
		getUserMove(currentRoom);
	}
	return;
}
/*********************************************************************
** Function: moveWest
** Description: this function allows the player to travel west
** Parameters: a pointer to a room
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
void GameMaster::moveWest(Room *currentRoom)
{
	Room * currRoom = currentRoom->getWest();
	if (currRoom != NULL)
	{
		printAreaInfo(currRoom);
		getUserMove(currRoom);
	}
	else
	{
		cout << "  Game Master: Try again you cannot go West from " << currentRoom->getName() << endl;
		getUserMove(currentRoom);
	}
	return;
}
/*********************************************************************
** Function: getWhereAmI
** Description: This function prints to the console the description
**				of where the player is. 
** Parameters: a pointer to a room
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
void GameMaster::getWhereAmI(Room *currentRoom)
{
	string desc = currentRoom->location;
	cout << "\tGame Master: " << desc << endl;

	getUserMove(currentRoom);

	return;
}
/*********************************************************************
** Function: getBackpack
** Description: This function prints to the console the contents of
**				the player's backpack.
** Parameters: pointer to a room
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
void GameMaster::getBackpack(Room *currentRoom)
{
	if (p->backpack.isEmpty())
	{
		cout << "\tGame Master: Your backpack is empty." << endl;
	}
	else
	{
		unsigned int size = p->backpack.items.size();
		cout << "\tGame Master: You have the following " << size << " different items in your backpack: " << endl;
		p->backpack.printItems(); 
	}

	return;
}
/*********************************************************************
** Function:playerProgress
** Description: This function prints to the console the # of item
**				types the player has dropped off on lot 48.
** Parameters: n/a
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
void GameMaster::playerProgress()
{
	cout << "\tGame Master: You have dropped off " << p->getPoints() << " different types of items. " << endl;
	cout << "\tGame Master: You need to drop off all of 10 different items with various quantities to win. " << endl;

	return;
}
/*********************************************************************
** Function: dropOffBackpack
** Description: This function allows a user to dropoff backpack items.
** Parameters: n/a
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
void GameMaster::dropOffBackpack(Room *currentRoom)
{
	if (p->backpack.isEmpty())
	{
		cout << "\tGame Master: Your backpack is empty and there is nothing for you to drop off." << endl;
	}
	else
	{
		currentRoom->dropItems(p);
	}

	getUserMove(currentRoom);

	return;
}
/*********************************************************************
** Function: moveDown
** Description: This function allows a user to travel downstairs
** Parameters: takes a pointer to a room
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
void GameMaster::moveDown(Room *currentRoom)
{
	Room * currRoom = currentRoom->getDown();
	if (currRoom != NULL)
	{
		printAreaInfo(currRoom);
		getUserMove(currRoom);
	}
	return;
}
/*********************************************************************
** Function: moveUp
** Description: This function allows a user to travel upstairs
** Parameters: takes a pointer to a room
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
void GameMaster::moveUp(Room *currentRoom)
{
	Room * currRoom = currentRoom->getUp();
	if (currRoom != NULL)
	{
		printAreaInfo(currRoom);
		getUserMove(currRoom);
	}
	return;
}
/*********************************************************************
** Function: gameWon
** Description: function writes to console when game has been won
** Parameters: n/a
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
void GameMaster::gameWon()
{
	cout << "\tGame Master: Congratulations! You have won the game!" << endl;
	cout << "\tLot48 has now transformed into Pawnee Commons featuring a Wamapoke-themed playground, \n";
	cout << "\tand even a Li'l Sebastian tribute fountain!\n";
	return;
}
/*********************************************************************
** Function: printSeparator
** Description: function prints to console a line with *s
** Parameters: n/a
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
void GameMaster::printSeparator()
{
	cout << setfill('*') << setw(80) << "*" << endl;
	return;
}
/*********************************************************************
** Function: printGMSeparator
** Description: function prints to console a line with -s
** Parameters: n/a
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
void GameMaster::printGMSeparator()
{
	cout << setfill('-') << setw(84) << "-" << endl;
	return;
}
/*********************************************************************
** Function: printAreaInfo
** Description: Prints to the console info about map area
** Parameters: n/a
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
void GameMaster::printAreaInfo(Room *currRoom)
{
	cout << endl << endl;
	printSeparator();
	string desc = currRoom->getDesc();
	cout << "\t" << desc << endl << endl;
	printSeparator();
	return;
}
/*********************************************************************
** Function: printGameIntro
** Description: Prints to console introduction of game info
** Parameters: n/a
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
void GameMaster::printGameIntro()
{
	printGMSeparator();
	cout << endl << endl;
	cout << "  Welcome to the Parks and Recreation Console Game! \n";
	cout << "  This game has been programmed by Carol Toro. " << endl << endl;
	
	cout << "  Here are the rules of the game: \n";
	cout << "\tTo win the game, you must help Leslie Knope build the Pawnee Commons \n";
	cout << "\ton Lot 48 by collecting a series of items found all over Pawnee. You will \n";
	cout << "\thave 5 minutes to collect all items. Use your time wisely! " << endl << endl;
	cout << "\tBefore you start collecting items, you should know that you can only carry \n";
	cout << "\t5 different item types at a time.  When you have found 5 different item types, \n";
	cout << "\tyou will need to return to Lot 48 to drop off the items before you can carry \n";
	cout << "\tnew types of items." << endl << endl;
	cout << "\tApril Ludgate will randomly run into you with hints to make game play easier.  \n";
	cout << "\tCarefully read the hints as April doesn't like to repeat herself and she will only \n";
	cout << "\tshare each hint once." << endl;
	printGMSeparator();
	return;
}