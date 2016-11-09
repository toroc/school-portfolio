/*********************************************************************
** Program Filename: Game_No_Limit.cpp
** Author: Carol D. Toro
** Date:  06/08/2015
** Description: This program runs a game where a user must find all
**				items throughout a city and return them to the starting
**				lot so that they can win the game. No time limit implemented. 
** Input: n/a
** Output: n/a
*********************************************************************/
#include <iostream>
#include "../inc/GameMaster.hpp"
// #include "../inc/Timer.hpp"
using namespace std;

void gameTimeOutFunc();
int main()
{
	// Timer gameTimeOut;

	// gameTimeOut.start(chrono::minutes(5), gameTimeOutFunc);


	GameMaster game;
	game.begin();
	
	// gameTimeOut.stop();


	return 0;
}

void gameTimeOutFunc()
{
	cout << "Uh Oh! You ran out of time and lost the game!";
	exit(0);
}
