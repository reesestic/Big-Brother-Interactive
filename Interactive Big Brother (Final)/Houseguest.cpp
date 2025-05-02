#include <string>
#include "Houseguest.h"
#include <iostream>

// int Houseguest::numOfPlayersInGame = 16;						// sets static member var's value at 16

int Houseguest::roundOfGame = 1;
string Houseguest::lastHOH = "";
string Houseguest::lastVetoWinner = "";
string Houseguest::lastVetoedPlayer = "";
string Houseguest::firstNom = "";
string Houseguest::secondNom = "";
string Houseguest::replacementNom = "";



bool Houseguest::vetoIsPlayed = false;

void Houseguest::setStrength() { strength = 0; }				// sets strength when user didnt specialize in it
void Houseguest::setStrength(int n) { strength = n; }			// sets strength when user specialized in it
int Houseguest::getStrength() { return strength; }
void Houseguest::increaseStrength(int n) { strength += n; }				// increase strength value


void Houseguest::setStrategy() { strength = 0; }				// sets strategy when user didnt specialize in it
void Houseguest::setStrategy(int n) { strategy = n; }			// sets strategy when user specialized in it
int Houseguest::getStrategy() { return strategy; }
void Houseguest::increaseStrategy(int n) { strategy += n; }				// increase strategy value

void Houseguest::setSocial() { social = 0; }					// sets social when user didnt specialize in it
void Houseguest::setSocial(int n) { social = n; }				// sets social when user specialized in it
int Houseguest::getSocial() { return social; }
void Houseguest::increaseSocial(int n) { social += n; }					// increase social value

void Houseguest::setTypeOfPlayer(int type)
{
	while (!(type == 1 || type == 2 || type == 3))
	{
		cout << "Type 1 to be a physical player, 2 to be a social player, and 3 to be a strategic player: ";
		cin >> type;
	}
	typeOfPlayer = type;
}

int Houseguest::getTypeOfPlayer()
{
	return typeOfPlayer;
}

void Houseguest::setName(string n)								// stores user's houseguest name
{
	name = n.substr(0, 14);
}

string Houseguest::getName() { return name; }					// returns user's houseguest name

// can be put itno the constructor
void Houseguest::setSizeAndValuesOfRelationships(int size)
{
	for (int i = 0; i < size; i++)
	{
		relationships.push_back(0);		// stores a 0 for each "size"
	}
}

void Houseguest::setRelationships(int indOfRelationAssignment, int valOfRelationAssignment)
{
	this->relationships.at(indOfRelationAssignment) += valOfRelationAssignment;
	if (this->relationships.at(indOfRelationAssignment) < -10)
	{
		this->relationships.at(indOfRelationAssignment) = -10;
	}
	else if (this->relationships.at(indOfRelationAssignment) > 10)
	{
		this->relationships.at(indOfRelationAssignment) = 10;
	}


}

vector<int>& Houseguest::getRelationships()
{
	return relationships;
}

int Houseguest::getSpecificRelationship(int index)
{
	return relationships[index];
}

string Houseguest::getBond(int index)
{
	string bond;
	if (this->relationships.at(index) < 0)
	{
		bond = "Abysmal";
	}
	else if (this->relationships.at(index) < 3)
	{
		bond = "Weak";
	}
	else if (this->relationships.at(index) < 6)
	{
		bond = "Friendly";
	}
	else if (this->relationships.at(index) < 10)
	{
		bond = "Close allies";
	}
	else // user has perfect 10 with person
	{
		bond = "Number one allies";
	}
	return bond;
}

int Houseguest::getIndexOfCPU(Houseguest& hg, vector<Houseguest>& CPU, size_t size)
{
	for (size_t index = 0; index < size; index++)
	{
		if (hg.getName() == CPU[index].getName())
		{
			return index;
		}
	}
	return 0;
}


void Houseguest::attemptToGetNominated(Houseguest& CPU, int indexOfCPU, vector<string> reasons)
{
	//Code to pick a reason for nomination
	int randomReason = rand() % reasons.size();
	int successfulAttempt;
	bool success = false;
	cout << "You attempt to pitch to the current Head of Household, " << Houseguest::getLastHOH() << ", to nominate ";
	cout << CPU.getName() << " because they're " << reasons.at(randomReason) << "." << endl;

	// Loop for nomination attempts based on strategy statistic
	for (int i = 0; i < this->getStrategy(); i++)
	{
		successfulAttempt = (rand() % 5 + 1);
		if (successfulAttempt == 1)
		{
			success = true;
		}
	}
	
	// Code to explain result and affect nominees/relationships
	if (success)
	{
		cout << Houseguest::getLastHOH() << " listened to you and agreed to nominate " << CPU.getName() << "!" << endl;
		cout << "Look at you go you talented manipulator ;)" << endl;
		CPU.setIsNominee(true);
		cout << CPU.getName() << "'s .getIsNominee() returns " << CPU.getIsNominee() << endl;
	}
	else
	{
		cout << "Well, that wasn't your best work!" << endl;
		cout << Houseguest::getLastHOH() << " told " << CPU.getName() << " that you came after them ";
		cout << "Now, " << CPU.getName() << " likes you less. Well done!" << endl;
		this->relationships.at(indexOfCPU) -= 2;
	}
	success = false;
}

void Houseguest::attemptToGetNominated(Houseguest& CPU1, int index1, Houseguest& CPU2, int index2, vector<string> reasons)
{
	vector<Houseguest> nominees;
	nominees.push_back(CPU1);
	nominees.push_back(CPU2);

	vector<int> indexesOfCPUs;
	indexesOfCPUs.push_back(index1);
	indexesOfCPUs.push_back(index2);

	int randomReason = rand() % reasons.size();
	int successfulAttempt;
	bool success = false;

	cout << "You attempt to pitch to the current Head of Household, " << Houseguest::getLastHOH() << ", to nominate ";
	cout << CPU1.getName() << " because they're " << reasons.at(randomReason) << "." << endl;

	cout << "You also asked " << Houseguest::getLastHOH() << " to nominate " << CPU2.getName();
	cout << " because they're " << reasons.at(rand() % reasons.size()) << "." << endl;

	for (int i = 0; i < 2; i++)
	{
		cout << endl;
		cout << "Result of your nomination pitch for " << nominees.at(i).getName() << ": " << endl;
		for (int i = 0; i < this->getStrategy(); i++)
		{
			successfulAttempt = (rand() % 5 + 1);
			if (successfulAttempt == 1)
			{
				success = true;
			}
		}

		// Code explaining success/failure of nominations and the effect
		if (success)
		{
			if (i == 1)
			{
				cout << "On your second pitch, ";
			}
			cout << Houseguest::getLastHOH() << " listened to you ";
			cout << "and agreed to nominate " << nominees.at(i).getName() << "!" << endl;
			cout << "Look at you go you talented manipulator ;)" << endl;
			nominees.at(i).setIsNominee(true);

		}
		else
		{
			cout << "Well, that wasn't your best work." << endl;
			cout << Houseguest::getLastHOH() << " told " << nominees.at(i).getName() << " that you came after them ";
			cout << "Now, " << nominees.at(i).getName() << " likes you less. Well done!" << endl;
			this->relationships.at(indexesOfCPUs.at(i)) -= 2;
		}
		success = false;
	}
	if (nominees.at(0).getIsNominee())
	{
		CPU1.setIsNominee(true);
	}
	if (nominees.at(1).getIsNominee())
	{
		CPU2.setIsNominee(true);
	}
}

void Houseguest::fillRemainingNominees(vector<Houseguest>& CPU)
{
	// Populate nominees, loop until =2
	int nomineeCount = 0;
	vector<Houseguest> currNominees;

	if (this->getIsNominee())
	{
		nomineeCount++;
		currNominees.push_back(*this);
	}
	for (int i = 0; i < CPU.size(); i++)
	{
		if (CPU.at(i).getIsNominee())
		{
			nomineeCount++;
			currNominees.push_back(CPU.at(i));
		}
	}

	while (nomineeCount != 2)
	{
		// veto is played
		if (Houseguest::getVetoIsPlayed())
		{
			// All checks needed for post-veto logic (if passes, you get nominated)
			if ((rand() % CPU.size() + 1) == 1 && !this->getIsNominee() && !this->getIsHOH()
				&& !this->getIsVetoWinner() && this->getName() != Houseguest::getLastVetoedPlayer() && !this->getIsEliminated())
			{
				this->setIsNominee(true);
				currNominees.push_back(*this);
				nomineeCount++;
			}
			else
			{
				// Logic for a CPU being nominated instead
				int randNomIndex = (rand() % CPU.size());

				// If passes, then CPU will be nominated
				if (!CPU[randNomIndex].getIsHOH() && !CPU.at(randNomIndex).getIsNominee() &&
					!CPU[randNomIndex].getIsVetoWinner() && CPU[randNomIndex].getName() != Houseguest::getLastVetoedPlayer())
				{
					CPU.at(randNomIndex).setIsNominee(true);
					currNominees.push_back(CPU.at(randNomIndex));
					nomineeCount++;
				}
			}
		}
		else // veto not played yet
		{
			// fillRemainingNominees runs only whne user != HOH, so dont need to check
			if ((rand() % CPU.size() + 1) == 1 && !this->getIsNominee() && !this->getIsEliminated())
			{
				this->setIsNominee(true);
				currNominees.push_back(*this);
				nomineeCount++;

			}
			else
			{
				// Same logic but for CPU
				int randNomIndex = (rand() % CPU.size());
				if (CPU.at(randNomIndex).getName() != Houseguest::getLastHOH() && !CPU.at(randNomIndex).getIsNominee())		// checks to make sure chosen CPU is not HOH
				{
					CPU.at(randNomIndex).setIsNominee(true);
					currNominees.push_back(CPU.at(randNomIndex));
					nomineeCount++;
				}
			}
		}
	}
	// Output final nominees
	cout << Houseguest::getLastHOH() << "'s nominees this week are " << currNominees.at(0).getName() << " and " << currNominees.at(1).getName() << "." << endl;

}

vector<Houseguest*> Houseguest::getNominees(vector<Houseguest>& CPU)
{
	// Getting references to the nominees
	vector<Houseguest*> nominees;

	if (this->getIsNominee())
	{
		nominees.push_back(this);
	}

	for (int i = 0; i < CPU.size(); i++)
	{
		if (CPU.at(i).getIsNominee())
		{
			nominees.push_back(&CPU.at(i));
		}
	}
	return nominees;
}

Houseguest Houseguest::pickVetoCompetitors(vector<Houseguest>& CPU, vector<Houseguest*>& nominees, vector<Houseguest>& vetoParticipants)
{

	// This loop iterates until player is not HOH, nominee or already selected in vetoParticipants
	bool validVetoPlayer = false;
	int randVetoPick = 0;
	while (!validVetoPlayer)
	{
		validVetoPlayer = true;
		randVetoPick = (rand() % CPU.size());

		if (CPU.at(randVetoPick).getName() == Houseguest::getLastHOH())
		{
			validVetoPlayer = false;		// if chosen CPU is HOH, false, will loop again with new value
		}
		if (CPU.at(randVetoPick).getName() == nominees.at(0)->getName())
		{
			validVetoPlayer = false;		// if chosen CPU is nominee, false, will loop again with new value
		}
		if (CPU.at(randVetoPick).getName() == nominees.at(1)->getName())
		{
			validVetoPlayer = false;		// if chosen CPU is nominee, false, will loop again with new value
		}
		for (Houseguest vetoPlayer : vetoParticipants)
		{
			// if chosen CPU is already picked for veto draw, false, will loop again with new value
			if (CPU.at(randVetoPick).getName() == vetoPlayer.getName())
			{
				validVetoPlayer = false;
			}
		}
	}
	return CPU.at(randVetoPick);
}


void Houseguest::conversationResult(Houseguest& player, int index, int value)
{
	// Stores result of a conversation and prints summary based on social points gained
	this->setRelationships(index, value);

	if (value == -2)
	{
		cout << player.getName() << " did not enjoy the conversation." << endl;
		cout << "... at all." << endl;
		cout << "Maybe reconsider your social game!" << endl;
		cout << player.getName() << " lost a lot of respect for you during this conversation. Nice work!" << endl;

	}
	else if (value == -1)
	{
		cout << player.getName() << " did not enjoy the conversation." << endl;
		cout << "Maybe reconsider your social game!" << endl;
		cout << player.getName() << " feels a little less comfortable around you now. Nice work!" << endl;
	}
	else if (value == 0)
	{
		cout << player.getName() << " didn't really listen to you." << endl;
		cout << "Try being more interesting maybe?" << endl;
		cout << player.getName() << " feels no differently towards you." << endl;
	}
	else if (value == 1)
	{
		cout << player.getName() << " surprisingly... enjoyed the conversation?" << endl;
		cout << "I guess " << player.getName() << " is into that sort of thing!" << endl;
		cout << player.getName() << " considers you more of a friend now." << endl;
	}
	else if (value == 2)
	{
		cout << player.getName() << " was ecstatic that someone finally mentioned their favorite hobby!" << endl;
		cout << "It seems like " << player.getName() << " is much more fond about you now!" << endl;
	}
}

void Houseguest::setIsEliminated(bool var) { eliminated = var; }			// set when player is voted out
bool Houseguest::getIsEliminated() { return eliminated; }

void Houseguest::setIsHOH(bool var) { isHOH = var; }					// set when player wins HOH
bool Houseguest::getIsHOH() { return isHOH; }

Houseguest Houseguest::findHOH(vector<Houseguest>& CPU)
{
	// Returns CPU HOH, returns user if not found (since user is HOH)
	for (Houseguest& houseguest : CPU)
	{
		if (houseguest.getIsHOH())
		{
			return houseguest;
		}
	}
	return *this;
}

Houseguest* Houseguest::findVetoWinner(vector<Houseguest>& CPU)
{
	// Same logic as findHOH, but for veto winner
	// HOWEVER: returns reference to houseguest instead of the value
	for (Houseguest& houseguest : CPU)
	{
		if (houseguest.getIsVetoWinner())
		{
			return &houseguest;
		}
	}
	return this;
}

// Simple setter/getters
void Houseguest::setIsNominee(bool var) { isNominee = var; }			
bool Houseguest::getIsNominee() { return isNominee; }

void Houseguest::setIsVetoWinner() { isVetoWinner = true; }		
bool Houseguest::getIsVetoWinner() { return isVetoWinner; }

// Cleanse user/CPU between cycles of main loop
void Houseguest::resetStatus()
{
	isHOH = false;
	isNominee = false;
	isVetoWinner = false;
}

// Display user/CPU's stats (CPU mainly for debugging, but we do display users sometimes)
void Houseguest::displayUserStats()
{
	cout << this->getName() << "'s strength stat: " << this->getStrength() << endl;
	cout << this->getName() << "'s social stat: " << this->getSocial() << endl;
	cout << this->getName() << "'s strategy stat: " << this->getStrategy() << endl;
}

// More simple setter/getter logic below
void Houseguest::setRoundOfGame(int round)
{
	roundOfGame += round;
}
int Houseguest::getRoundOfGame()
{
	return roundOfGame;
}

void Houseguest::setLastHOH(string n)
{
	lastHOH = n;
}

string Houseguest::getLastHOH()
{
	return lastHOH;
}

void Houseguest::setLastVetoWinner(string n)
{
	lastVetoWinner = n;
}

string Houseguest::getLastVetoWinner()
{
	return lastVetoWinner;
}

void Houseguest::setVetoIsPlayed(bool var)
{
	vetoIsPlayed = var;
}

bool Houseguest::getVetoIsPlayed()
{
	return vetoIsPlayed;
}

string Houseguest::getLastVetoedPlayer()
{
	return lastVetoedPlayer;
}
void Houseguest::setLastVetoedPlayer(string player)
{
	lastVetoedPlayer = player;
}

void Houseguest::setFirstNom(string fn)
{
	firstNom = fn;
}
string Houseguest::getFirstNom()
{
	return firstNom;
}

void Houseguest::setSecondNom(string sn)
{
	secondNom = sn;
}

string Houseguest::getSecondNom()
{
	return secondNom;
}

void Houseguest::setReplacementNom(string renom)
{
	replacementNom = renom;
}
string Houseguest::getReplacementNom()
{
	return replacementNom;
}

bool Houseguest::eligibleToVote()
{
	// return true if not HOH and not nominee
	return (!(this->getIsHOH() || this->getIsNominee() || this->getIsEliminated()));
}

void Houseguest::resizeCPU(vector<Houseguest>& CPU, int cpuSize, vector<Houseguest>& eliminated, int elimSize)
{
	// ensures only one person is removed and indices are not violated
	int maxChanges{ 0 };
	bool inElimVector{ false };
	for (auto& i : eliminated)
	{
		if (i.getName() == this->getName())
		{
			inElimVector = true;
		}
	}

	if (this->getIsEliminated() && !inElimVector)
	{
		eliminated.push_back(*this);
	}
	this->resetStatus();
	// User's elimination is taken care of

	// If user wasn't added, now add the CPU who is eliminated but still in vector
	// Other eliminated CPU are already gone
	for (int i = 0; i < CPU.size(); i++)
	{
		if (CPU[i].getIsEliminated())
		{
			// add them to eliminated vector
			eliminated.push_back(CPU[i]);

			CPU.erase(CPU.begin() + i);
			break;
		}
	}
	for (int i = 0; i < CPU.size(); i++)
	{
		// Set status for each CPU for nom/HOH/veto winner to false
		CPU[i].resetStatus();
	}
}


void Houseguest::setInFinalHOH(bool result)
{
	inFinalHOH = result;
}

bool Houseguest::getInFinalHOH()
{
	return inFinalHOH;
}

// Code for when user reaches finale!
string Houseguest::finalHOHWithUser(vector<Houseguest>& CPU, int cpuSize)
{
	vector<Houseguest> finalHOHCompetitors;
	int winner = rand() % 3;
	if (winner == 0)
	{
		cout << "Congratulations " << this->getName() << ", you are the winner of Part 1!" << endl;
		this->setInFinalHOH(true);
		finalHOHCompetitors.push_back(*this);
	}
	else
	{
		int randCPU = rand() % 2;
		cout << "Congratulations " << CPU[randCPU].getName() << ", you are the winner of Part 1!" << endl;
		CPU[randCPU].setInFinalHOH(true);
		finalHOHCompetitors.push_back(CPU[randCPU]);

	}
	// Sets HOH for top 3 (new code since top 3 is so different)
	if (!this->getInFinalHOH())
	{
		cout << "You now face off against ";
		for (int i = 0; i < CPU.size(); i++)
		{
			if (!CPU[i].getInFinalHOH())
			{
				cout << CPU[i].getName() << " in part two of the final HOH competition." << endl;
				int randWinner = rand() % 2;
				if (randWinner == 0)
				{
					cout << "Congratulations " << this->getName() << ", you are the winner of Part 2!" << endl;
					this->setInFinalHOH(true);
					finalHOHCompetitors.push_back(*this);

				}
				else
				{
					cout << "Congratulations " << CPU[i].getName() << ", you are the winner of Part 2!" << endl;
					CPU[i].setInFinalHOH(true);
					finalHOHCompetitors.push_back(CPU[i]);
				}
			}
		}
	}
	else
	{
		cout << "Now, " << CPU[0].getName() << " and " << CPU[1].getName() << " will face off in Part 2!" << endl;
		int randWinner = rand() % 2;

		cout << "Congratulations, " << CPU[randWinner].getName() << ", you are the winner of Part 2!" << endl;
		CPU[randWinner].setInFinalHOH(true);
		finalHOHCompetitors.push_back(CPU[randWinner]);
	}
	// Winners of first two challenges are decided
	cout << "The competitors in the final part of the final HOH are ";
	cout << finalHOHCompetitors[0].getName() << " and " << finalHOHCompetitors[1].getName() << "." << endl;

	winner = rand() % 2;
	finalHOHCompetitors[winner].setIsHOH(true);
	cout << "Congratulations " << finalHOHCompetitors[winner].getName() << ", you are the final HOH!";

	// Stores winner out of winners of P1 and P2, returns it.
	return finalHOHCompetitors[winner].getName();
}

// Same logic as above but without user and with one more CPU
string Houseguest::finalHOHWithoutUser(vector<Houseguest>& CPU, int cpuSize)
{
	vector<Houseguest> finalHOHCompetitors;

	int randCPU = rand() % 3;
	cout << "Congratulations " << CPU[randCPU].getName() << ", you are the winner of Part 1!" << endl;
	CPU[randCPU].setInFinalHOH(true);
	finalHOHCompetitors.push_back(CPU[randCPU]);

	vector<Houseguest*>partTwoCompetitors;
	for (auto& cpu : CPU)
	{
		if (!cpu.getInFinalHOH())
		{
			partTwoCompetitors.push_back(&cpu);
		}
	}

	randCPU = rand() % 2;
	cout << "Congratulations " << partTwoCompetitors[randCPU]->getName() << ", you are the winner of Part 2!" << endl;
	partTwoCompetitors[randCPU]->setInFinalHOH(true);
	finalHOHCompetitors.push_back(*partTwoCompetitors[randCPU]);

	cout << "The competitors in the final part of the final HOH are ";
	cout << finalHOHCompetitors[0].getName() << " and " << finalHOHCompetitors[1].getName() << "." << endl;

	randCPU = rand() % 2;
	cout << "Congratulations " << finalHOHCompetitors[randCPU].getName() << ", you are the final HOH!";

	return finalHOHCompetitors[randCPU].getName();

	// Winner of final HOH is established

}