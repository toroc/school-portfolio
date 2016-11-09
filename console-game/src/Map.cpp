/*********************************************************************
** Program Filename: Map.cpp
** Author: Carol D. Toro
** Date:  06/08/2015
** Description: This is the Map class implementation file containing
**				the definitions for the Map class.
** Input: n/a
** Output: n/a
*********************************************************************/
#include "../inc/Map.hpp"
#include "../inc/Room.hpp"
#include "../inc/DropRoom.hpp"
#include "../inc/EmptyRoom.hpp"
#include "../inc/DownStairsRoom.hpp"
#include "../inc/UpStairsRoom.hpp"

Map::Map()
{
	/*Items*/
	pony = new Item("Li'l Sebastian Commemorative Statue", "  This statue of Li'l Sebastian, an animal, a legend, \n  a friend and a beacon of light, inspires Pawnee everyday.");
	hoop = new Item("Basketball Hoop", "  One that basketballs can actually fit through.");
	playground = new Item("Playground Set", "  A Wamapoke-themed playground that is also tornado proof.");
	seesaw = new Item("Seesaw", "  One that doesn't have gum and has not been repaired with glue.");
	swing = new Item("Tire Swing", "  One that isn't made from old car tires.");
	sign = new Item("Welcome to Pawnee Sign", "  First in Friendship, Fourth in Obesity");
	fountain = new Item("Li'l Sebastian Tribute Fountain", "");
	plans = new Item("Architect's Blueprints for Pawnee Commons", "  Wreston St. James designed these plans for the park");
	petition = new Item("Pawnee Commons Petition", "  1000 Signatures in favor of Pawnee Commons");
	permit = new Item("Park Building Permit", "  Issued by the Pawnee City Council");


	lot48 = new DropRoom("Lot 48");
	lot48->setDescription("Lot 48 also known as the Sullivan Street Pit is where your adventure begins and ends. \n ");
	lotSouth = new EmptyRoom();

	jjDiner = new Room("JJ's Diner");
	jjDiner->setDescription("JJ's Diner has been a staple for the citizens of Pawnee since its opening in 1976. \n  The specialty here at JJ's are our world-famous waffles.");

	dinerEast =  new EmptyRoom();

	cityHall = new Room("Pawnee City Hall");
	cityHall->setDescription("Pawnee City Hall is the home of most government functions in Pawnee.");

	parksOffice = new DownStairsRoom("Park's Depart. Office");
	parksOffice->setDescription("The Parks and Recreation Department of Pawnee \n  oversees parks and recreational public works, \n  and is supervised by Director Ron Swanson");

	fourthFloor = new UpStairsRoom("Fourth Floor of City Hall");
	fourthFloor->setDescription("Proceed to your intended department. Do not make eye contact with anyone. \n  Do not speak to anyone. Do not accept suspicious packages from anyone.");

	officeEast = new EmptyRoom();
	officeSouth = new EmptyRoom();

	ronsCabin = new Room("Ron Swanson's Cabin");
	ronsCabin->setDescription("Ron Swanson's Journal: I am in my cabin in the woods. It's good. \n  No one else is around for 2 miles in any direction. \n  I have some water and whiskey and my shotgun. I am going to go kill my dinner. \n  Okay I am back and I have my dinner, a deer. \n  I shot this deer and now I will cook it.");

	rentSwag = new Room("Rent-A-Swag");
	rentSwag->setDescription("Rent kids clothing from a huge wardrobe of the dopest shirts, \n  the swankiest jackets, the slickest cardigans, the flashiest fedoras, \n  the hottest ties, the snazziest canes and more!");

	sandwich = new Room("Ray's Sandwich Place");
	sandwich->setDescription("  Home of the famous fried chicken skin pizza, everything at Ray's has gravy on it. \n  If there's no gravy on whatever you ordered, you get your money back!");

	sandwichEast = new EmptyRoom();

	sweetums = new Room("Sweetums Candy Factory");
	sweetums->setDescription("Sweetums is Pawnee's leading sweetums treat manufacturer \n  and has been for over 80 years. \n  Did you know that the pollution from the factory results in beautiful sunsets?");

	sweetEast = new EmptyRoom();

	festival = new Room("The Pawnee Harvest Festival");
	festival->setDescription("  The Harvest Festival is a week-long festival in Pawnee, Indiana. ");

	ferrisWheel = new Room("The Harvest Festival Ferris Wheel");
	ferrisWheel->setDescription("Be careful not to stay here for too long as you may get stranded if the power goes out. ");

	tomBistro = new Room("Tom's Bistro");
	tomBistro->setDescription("Tom's Bistro is one of the most successful businesses in Pawnee.");

	unityConcert = new Room("The Pawnee Unity Concert");
	unityConcert->setDescription("The Unity Concert is a massive music and arts festival to celebrate the merger and \n  rally together the Pawnee and Eagleton communities! It's like if the United Nations put on Woodstock in a park!!");
}

/*********************************************************************
** Function: createMap
** Description: function creates the map of pawnee
** Parameters: n/a
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
Room* Map::createMap()
{
	/*Lot 48*/
	lot48->setEast(jjDiner);
	lot48->setSouth(lotSouth);

	/*Lot South*/
	lotSouth->setNorth(lot48);
	lotSouth->setEast(cityHall);

	/*JJ's Diner*/
	jjDiner->setWest(lot48);
	jjDiner->setSouth(cityHall);
	jjDiner->setEast(dinerEast);

	/*City Hall*/
	cityHall->setNorth(jjDiner);
	cityHall->setWest(lotSouth);
	cityHall->setSouth(parksOffice);
	
	/*Parks' Office*/
	parksOffice->setNorth(cityHall);
	parksOffice->setUp(fourthFloor);
	parksOffice->setEast(officeEast);
	parksOffice->setSouth(officeSouth);

	/*Fourth Floor*/
	fourthFloor->setDown(parksOffice);

	/*Office South*/
	officeSouth->setNorth(parksOffice);
	officeSouth->setSouth(ronsCabin);
	
	/*Ron's Cabin*/
	ronsCabin->setNorth(officeSouth);
	
	/*Office East*/
	officeEast->setNorth(rentSwag);
	officeEast->setEast(sandwich);
	officeEast->setWest(parksOffice);

	/*Rent-Swag*/
	rentSwag->setNorth(dinerEast);
	rentSwag->setSouth(officeEast);
	rentSwag->setEast(sweetums);

	/*DinerEast*/
	dinerEast->setWest(jjDiner);
	dinerEast->setSouth(rentSwag);

	/*Sandwichhouse*/
	sandwich->setWest(officeEast);
	sandwich->setSouth(festival);
	sandwich->setEast(sandwichEast);
	
	/*Harvest*/
	festival->setWest(ferrisWheel);
	festival->setNorth(sandwich);

	/*Ferris*/
	ferrisWheel->setEast(festival);

	/*Sweetums*/
	sweetums->setWest(rentSwag);
	sweetums->setEast(sweetEast);

	/*Sandwich East*/
	sandwichEast->setWest(sandwich);
	sandwichEast->setNorth(sweetEast);

	/*Sweet East*/
	sweetEast->setWest(sweetums);
	sweetEast->setEast(tomBistro);
	sweetEast->setSouth(sandwichEast);

	/*Tom's Bistro*/
	tomBistro->setWest(sweetEast);
	tomBistro->setNorth(unityConcert);

	/*Unity Concert*/
	unityConcert->setSouth(tomBistro);

	loadItems();
	loadLocations();
	loadFeedback();
	loadInventory();
	return lot48;
}
/*********************************************************************
** Function: loadItems
** Description: function loads the items into the various areas of Pawnee
** Parameters: n/a
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
void Map::loadItems()
{
	/*Lot South*/
	lotSouth->items->addItem(swing, 1);

	/*parksOffice*/
	parksOffice->items->addItem(petition, 1);

	/*parksOffice East*/
	officeEast->items->addItem(seesaw,1);

	/*parksOffice South*/
	officeSouth->items->addItem(playground, 1);

	/*cityHall*/
	cityHall->items->addItem(permit, 1);

	/*Ron's Cabin*/
	ronsCabin->items->addItem(sign, 1);

	/*Unity*/
	unityConcert->items->addItem(fountain, 1);

	/*Tom's Bistro*/
	tomBistro->items->addItem(hoop, 1);

	/*Rent-A-Swag*/
	rentSwag->items->addItem(hoop, 1);

	/*Ferris Wheel*/
	ferrisWheel->items->addItem(pony, 1);

	/*harvest*/
	festival->items->addItem(permit, 1);
	
	/*4th Floor*/
	fourthFloor->items->addItem(plans, 1);

	/*Sandwich*/
	sandwich->items->addItem(swing,1);

	/*Sweetums*/
	sweetums->items->addItem(seesaw, 1);

	/*Sweetums East*/
	sweetEast->items->addItem(seesaw, 1);
	
	/*SandwichEast*/
	sandwichEast->items->addItem(swing, 1);
	
}
/*********************************************************************
** Function: loadFeedback
** Description: function loads the clues April Ludgate will give
** Parameters: n/a
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
void Map::loadFeedback()
{
	/*Lot 48*/
	lot48->feedback = "\n\n  April Ludgate: Go on next door to the diner, \n  have a jjDiner and find out what items you need to collect.\n";

	/*JJ's Diner */
	jjDiner->feedback = "\n\n  April Ludgate: You need to collect the following 10 different types of items \n to help build Pawnee Commons on Lot 48:		\n  1 park statue, 3 Seesaws, 3 swings, \n  1 park blueprint, 1 fountain, 2 basketball hoops, \n  1 playground, 1 park petition, \n  2 park permits, and 1 welcome sign. \n";

	/*DinerEast*/
	dinerEast->feedback = "\n\n  April Ludgate: Tom has been trying to impress the kids on the basketball team \n  and he might be able to help you find some hoops.\n";
	
	/*officeSouth*/
	officeSouth->feedback = "\n\n  April Ludgate: If you still need a permit, I heard some City Council officials \n  were headed to the Harvest Festival. \n";

	/*Sandwich East*/
	sandwichEast->feedback = "\n\n  April Ludgate: I hear you may able to find the blue prints on the 4th floor of \n  City Hall above the Park's Office.";

	/*Ferris Wheel*/
	ferrisWheel->feedback = "\n\n  April Ludgate: Jerry and Tom have lost Li'l Sebastian! \n  From up top of this ferris wheel you spot him in the corn maze! \n  Go search the area!\n";

	/*sweetEast*/
	sweetEast->feedback = "\n\n  April Ludgate: If you need help with permits, try going to City Hall and looking around.\n";

}
/*********************************************************************
** Function: loadLocations
** Description: loads decriptions to the various map locations
** Parameters: n/a
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
void Map::loadLocations()
{
	/*Lot 48*/
	lot48->location = "  You are in " + lot48->getName() + " which is the west of JJ's diner.";

	/*JJ's Diner */
	jjDiner->location = "  You are in " + jjDiner->getName() + " which is the east of Lot48.";

	/*Diner East*/
	dinerEast->location = "  You are on " + dinerEast->getName() + " which is east of JJ's diner.";

	/*Rent-A-Swag*/
	rentSwag->location = "  You are in " + rentSwag->getName() + " which is west of the Sweetums Factory";

	/*Sweetums*/
	sweetums->location = "  You are in " + sweetums->getName() + " which is east of Rent-A-Swag";

	/*lot South*/
	lotSouth->location = "  You are in " + lotSouth->getName() + " which is the west of City Hall.";

	/*cityHall*/
	cityHall->location = "  You are in " + cityHall->getName() + " which is the south of JJ's diner.";

	/*parksOffice*/
	parksOffice->location = "  You are in " + parksOffice->getName() + " which is the south of City Hall.";

	/*fourthFloor*/
	fourthFloor->location = "  You are in " + fourthFloor->getName() + " which is upstairs of the Park's Dept. Office.";

	/*officeSouth*/
	officeSouth->location = "  You are on " + officeSouth->getName() + " which is south of the Park's Dept. Office.";

	/*officeEast*/
	officeEast->location = "  You are on " + officeEast->getName() + " which is the west of Ray's Sandwich Place.";

	/*ronsCabin*/
	ronsCabin->location = "  You are in " + ronsCabin->getName() + " which is the south of everything in Pawnee.";

	/*Harvest Festival*/
	festival->location = "  You are in " + festival->getName() + " which is the south of Ray's Sandwich Place.";

	/*ferris wheel*/
	ferrisWheel->location = "  You are in " + ferrisWheel->getName() + " which is west of the Harvest Festival.";

	/*Ray's Sandwich*/
	sandwich->location = "  You are in " + sandwich->getName() + " which is north of the Harvest Festival.";

	/*sandwichEast*/
	sandwichEast->location = "  You are on " + sandwichEast->getName() + " which is the east of Ray's Sandwich Place.";

	/*sweetEast*/
	sweetEast->location = "  You are on " + sweetEast->getName() + " which is east of the Sweetums Factory.";
	
	/*tom's Bistro*/
	tomBistro->location = "  You are in " + tomBistro->getName() + " which is south of the Unity Concert.";

	/*unity unityConcert*/
	unityConcert->location = "  You are in " + unityConcert->getName() + " which is north of Tom's Bistro.";
}
/*********************************************************************
** Function: loadInventory
** Description: loadsInventory to the game map
** Parameters: n/a
** Pre-Conditions:  n/a
** Post-Conditions:  n/a
*********************************************************************/
void Map::loadInventory()
{
	lot48->needed->addItem(pony, 1);
	lot48->needed->addItem(seesaw, 3);
	lot48->needed->addItem(swing, 3);
	lot48->needed->addItem(plans, 1);
	lot48->needed->addItem(fountain, 1);
	lot48->needed->addItem(hoop, 2);
	lot48->needed->addItem(petition, 1);
	lot48->needed->addItem(permit, 2);
	lot48->needed->addItem(sign, 1);
	lot48->needed->addItem(playground, 1);
}
