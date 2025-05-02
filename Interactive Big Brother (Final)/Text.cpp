// This file will house the blocks of text for the game so that the code is less convulated in Season.cpp
// Author: Reese Stichter

#include <iostream>
#include "Text.h"
#include <string>
using namespace std;

void introductionText()
{
	cout << "Welcome to Big Brother Interactive!" << endl;
	cout << "This season you're here to compete against 15 other players." << endl;
	cout << "But before we meet your fellow houseguests, you need to meet yourself!" << endl;
}

void selectPlayerTypeText()
{
	cout << "Well now that we know your name, we want to learn more about you!" << endl;
	cout << "What type of player are you?" << endl;
	cout << "Do you most like to win competitions, be a social butterfly, or out-strategize your opponents?" << endl;
	cout << "NOTE: You won't be able to confirm or re-select your playstyle, so choose carefully! You get one chance." << endl;
}

string conversationVariationBlock()
{
	string convoString;
	int random = (rand() % 15 + 1);

	if (random == 1)
		convoString = "the wonders of ";
	else if (random == 2)
		convoString = "the enchantments and charms of ";
	else if (random == 3)
		convoString = "the beauty of ";
	else if (random == 4)
		convoString = "the sport of ";
	else if (random == 5)
		convoString = "RuPaul's worst nightmare, ";
	else if (random == 6)
		convoString = "something that would kill a founding father, ";
	else if (random == 7)
		convoString = "the stunning art of ";
	else if (random == 8)
		convoString = "the tackiest activity of all, ";
	else if (random == 9)
		convoString = "my favorite thing to do on a Friday night, ";
	else if (random == 10)
		convoString = "the pinnacle of human recreation, ";
	else if (random == 11)
		convoString = "the splendiferousness of ";
	else if (random == 12)
		convoString = "the glory and grandeur of ";
	else if (random == 13)
		convoString = "the loveliness and luster of ";
	else if (random == 14)
		convoString = "Oprah's greatest passion, ";
	else if (random == 15)
		convoString = "some random thing called ";
	return convoString;
}

void convinceHOHnomsText()
{
	cout << "Since you are not Head of Household, do you want to try and influence nominations?" << endl;
	cout << "If successful, you can place your enemies on the nomination block and attempt to send them home." << endl;
	cout << "If unsucessful, the Head of Household may decide to tell the player you threw under the bus about your scheme." << endl;
	cout << "Is the risk worth the reward?" << endl;
	cout << "Type 1 if you wish to attempt to get your enemies nominated using your strategy stat: " << endl;
}

/*
* Code blocks describing player's selection type
*/

void chosePhysical()
{
	cout << "You decided to be a physical player! You spent your lifting weights and listneing to TikTok fitness influencers." << endl;
	cout << "As a result, people will think you are attractive and you will perform better in challenges!" << endl;
	cout << "Your physical stat has increased by three points." << endl;
}

void choseSocial()
{
	cout << "You decided to be a social player! You spent your preparation time reading books about how to listen." << endl;
	cout << "As a result, people will think you like them when you talk to them!" << endl;
	cout << "Your social stat has increased by three points." << endl;
}

void choseStrategic()
{
	cout << "You decided to be a strategic player! You spent your preparation time grinding Chess.com in a dark room." << endl;
	cout << "As a result, you can convince people to listen to you when you tell them terrible plans!" << endl;
	cout << "Your strategy stat has increased by three points." << endl;
}