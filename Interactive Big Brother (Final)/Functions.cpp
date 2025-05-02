#include "Functions.h"
#include "Houseguest.h"
#include <iostream>
#include <windows.h>
using namespace std;

void inputToLower(string& in)
{
	for (char& c : in)
	{
		c = (char)tolower(c);
	}
}

string validateYesNo(const string in, string n)
{
	string inputNew;
	cout << "You're supposed to type yes or no... Try again." << endl;
	Sleep(1500);
	cout << "You sure you want to go with " + n + "? You can't change this later. (Yes/No) " << endl;
	getline(cin, inputNew);
	inputToLower(inputNew);
	return inputNew;
}

bool validateName(string response, string n, bool newName)
{
	while (response != "yes" && response != "no")			// Input validation loop for user verifying name
	{
		response = validateYesNo(response, n);
	}

	if (response == "yes")
	{
		cout << "Welcome to Big Brother Interactive, " + n + "!" << endl;
		return false;
	}

	else
	{
		cout << "Okay, what will your new name be?" << endl;
		return true;
	}
}

void displayCPUvector(vector<Houseguest> CPU)
{
	for (int i = 0; i < CPU.size(); i++)	// loops values are diffferent to properly display vectors at index 1+
	{
		cout << i + 1 << ") " << CPU.at(i).getName() << endl;
	}
}




