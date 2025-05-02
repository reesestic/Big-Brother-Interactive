#include "Houseguest.h"
#include "Functions.h"
#include "Text.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>

using std::string;
const int NUM_OF_CONVOS_PER_SOCIAL = 2;
bool DEBUG = false;

int main()
{
	srand(time(0));					// Applies randomization to rand()
	Houseguest user;				// Object that user plays as
	string name;					// Storage for user's name (before being stored in user.name)
	int chooseToThrow = 0;
	bool keepPlaying{ true };
	bool wonHOHCompetition = false;
	bool wonVetoCompetition = false;

	introductionText();				// Text block explaining introduction to game

	bool enterNewName = false;		// Used to decide if loop will ask for name again (ONLY USED IN THIS DO-WHILE)
	do
	{
		string doWhileInput;

		// Store user's input in name variable
		cout << "So, what's your name? (Limit: 15 characters, extra chars will be cut off) : ";
		getline(cin, name);			
		user.setName(name);

		/*
		* Verify name input
		* User response --> doWhileInput
		* Set user input to lowercase
		*/
		cout << "You sure you want to go with " + name + "? You can't change this later. (Yes/No) " << endl;
		getline(cin, doWhileInput);
		inputToLower(doWhileInput);

		/*
		* Put in loop of ensuring input == Yes or No
		*/
		enterNewName = validateName(doWhileInput, name, enterNewName);
	} while (enterNewName);

	// Do-while loop to properly select player type
	int typeOfPlayer;
	do
	{
		cout << "Type 1 to be a physical player, 2 to be a social player, and 3 to be a strategic player: ";
		cin >> typeOfPlayer;
	} while (!(typeOfPlayer == 1 || typeOfPlayer == 2 || typeOfPlayer == 3));
	user.setTypeOfPlayer(typeOfPlayer);


	if (typeOfPlayer == 1)
	{
		// Text block describing player's selection
		chosePhysical();

		// Updating user initial stats and displaying new values
		user.increaseStrength(2);
		user.displayUserStats();
	}

	else if (typeOfPlayer == 2)
	{
		// Text block describing player's selection
		choseSocial();

		// Updating user initial stats and displaying new values
		user.increaseSocial(2);
		user.displayUserStats();
	}

	else
	{
		// Text block describing player's selection
		choseStrategic();

		// Updating user initial stats and displaying new values
		user.increaseStrategy(2);
		user.displayUserStats();
	}

	cout << "So " + name + ", now we know what kind of player you are." << endl;
	cout << "Next, we need to meet your fellow housemates and start the game!" << endl;

	// Declare size of CPU vector, establish eliminated vector
	int numOfOpponents = 15;
	const int STARTING_NUMBER_OF_PLAYERS = numOfOpponents+1;
	vector<Houseguest> CPU(numOfOpponents);	
	vector<Houseguest> eliminated;

	//COME BACK TO
	user.setSizeAndValuesOfRelationships(numOfOpponents);

	/* Vector declarations for:
	* 1) Things to talk about in conversations
	* 2) Names for CPUs
	* 3) Reasons people are nominated
	* These are just to store inFile read-ins
	*/

	vector<string> interests;								/* 1 */
	vector<string> possibleNames;							/* 2 */
	vector<string> nominationReasons;						/* 3 */

	// Declare ifstream object "inFile" to act as an input file stream for all .txt files
	ifstream inFile;

	inFile.open("CPUnames.txt");		
	string inFileCurrLine;				// makes a string to hold each line input of inFile

	// Populate possibleNames vector with (CPU) differentiation if needed
	while (getline(inFile, inFileCurrLine))	
	{
		if (inFileCurrLine == user.getName())	
		{
			inFileCurrLine += " (CPU)";
		}
		possibleNames.push_back(inFileCurrLine);
	}

	// Close and clear inFile to use for next file read-in (interests.txt)
	inFile.close();									
	inFile.clear();	

	inFile.open("Interests.txt");				// opens inFile for new file (Interests)

	while (getline(inFile, inFileCurrLine))
	{
		interests.push_back(inFileCurrLine);
	}

	// Close and clear inFile to use for next file read-in (nominationReasons.txt)
	inFile.close();
	inFile.clear();

	inFile.open("NominationReasons.txt");		// opens inFile for new file (nominationReasons)
	while (getline(inFile, inFileCurrLine))
	{
		nominationReasons.push_back(inFileCurrLine);
	}

	/*
	* Loop of code to select CPU names.
	* Once a name is selected, set line to empty string.
	* If it is selected again (duplicate), retry a new line.
	*/
	for (int i = 0; i < numOfOpponents; i++)
	{
		int randIndex = rand() % possibleNames.size();
		if (possibleNames.at(randIndex) != "")
		{
			CPU.at(i).setName(possibleNames.at(randIndex));
			possibleNames.at(randIndex) = ""; // sets names that now have been used to hold "", will be ignored in future now

		}
		else
		{
			i--;	// goes back to the previous index so that no index gets a name of ""
		}

		CPU.at(i).setSizeAndValuesOfRelationships(numOfOpponents);
	}

	cout << "Will you outsmart your opponents? Or will you flame out?" << endl;
	cout << "Find out this season on Big Brother Interactive!" << endl;

	/*
	* --------------------------------------------------------------------------------------
	* | This is the main block of code looped over for every round of the game!			   |
	* | The loop breaks once one of these conditions is fulfilled:						   |
	* |		1) The player is eliminated and chooses to not spectate (keepPlaying == false) |
	* |		2) There are less than four players in the game (3 CPUs or 2 CPUs and user)    |
	* --------------------------------------------------------------------------------------
	*/
	// change to using eliminated vectro size
	while (keepPlaying && STARTING_NUMBER_OF_PLAYERS - eliminated.size() >= 4)
	{
		Houseguest::setLastVetoedPlayer(""); // setting this here becuase it needs to restart each loop unlike other statics

		cout << "It's time to crown a new Head of Household!" << endl;

		/*
		* Code loop to determine if user becomes Head of Household this round.
		* If user is not outgoing Head of Household and still in the game:
		*	If user chooses to throw, skip to letting a CPU win.
		*	Else, give a algorithmically-determined number of attempts for user to win HOH.
		* 
		* If user fails to win, choose a random CPU to win
		*/
		if (user.getName() != Houseguest::getLastHOH() && !user.getIsEliminated())
		{
			cout << "Do you want to throw the upcoming HOH? Type 1 to throw the competition: " << endl;
			cin >> chooseToThrow;
			if (chooseToThrow == 1)
			{
				cout << "You chose to throw the HOH competition!" << endl;
			}
			else
			{
				cout << "Okay girl, guess you're going for it! Good luck!" << endl;
				for (int i = 0; i < ((user.getStrength() / 2) + 1); i++)
				{
					// Code like this only displays if ran on DEBUG mode (debug=true, cannot be set by user)
					if (DEBUG)
					{
						cout << "Odds of you winning HOH: 1 in " << numOfOpponents << endl;
						cout << "Attempt number " << i + 1 << endl;
					}
					int random = 1 + (rand() % numOfOpponents);		// stores a number between 1 and num of CPUs in game
					if (random == 1)									// arbitrary number chosen to represent winning (1)
					{
						wonHOHCompetition = true;							// bool to represent that user won the competition
					}
				}
			}
		}
		else if (user.getName() == Houseguest::getLastHOH())
		{
			cout << "As outgoing Head of Household, you are ineligible to compete." << endl;
		}
		//Either user has now won, lost or couldn't play.

		if (wonHOHCompetition)											
		{
			cout << "Congratulations, you are the new Head of Household!" << endl;
			user.setIsHOH(true);										// sets isHOH to true
			Houseguest::setLastHOH(user.getName());					// sets static value lastHOH to be user's name
		}
		else
		{
			cout << "Sorry, you lost!" << endl;

			// Select random index to win
			int winner = rand() % CPU.size();

			// If winner is last HOH
			if (CPU[winner].getName() == Houseguest::getLastHOH())

				// Code to pick a new winner by picking an adjacent CPU in the vector
				if (winner == CPU.size() -1)
				{
					winner -= 1;
				}
				else
				{
					winner += 1;
				}
		
			// Set CPU at this index as the winner and put their name as lastHOH static string.
			CPU.at(winner).setIsHOH(true);	
			Houseguest::setLastHOH(CPU.at(winner).getName());
			cout << CPU.at(winner).getName() << " is the new Head of Household!" << endl;
		}

		// Reset wonHOHCompetition so each loop will have this value be false 
		wonHOHCompetition = false;
		
		/*		HOH HAS BEEN SET FOR THE ROUND		*/

		if (!user.getIsEliminated())
		{
			cout << "Now that ";
			if (user.getIsHOH() == true)
			{
				cout << "you are Head of Household, it's time to socialize!" << endl;
				cout << "Use this time to set up your social relationships for next week!" << endl;
			}
			else
			{
				cout << Houseguest::getLastHOH() << " is Head of Household, it's time to socialize!" << endl;
				cout << "Will you suck up to the HOH, or will you try and make other friends? The choice is yours." << endl;
			}

			int socialChoice = 0;
			cout << "Who do you want to socialize with this week? Enter the houseguest's number." << endl;

			for (int i = 0; i < NUM_OF_CONVOS_PER_SOCIAL; i++)
			{
				displayCPUvector(CPU);
				cout << (CPU.size() + 1) << ") No more talking! I hate them all!" << endl;

				cin >> socialChoice;
				while (socialChoice < 1 || socialChoice > CPU.size() + 1)
				{
					cout << "You didn't type a valid Houseguest number." << endl;
					cout << "Try typing a valid houseguest's number in again: " << endl;
					cin >> socialChoice;
				}

				// Social choice has been set
				if (socialChoice == CPU.size() + 1)
				{
					cout << "You chose to be an introvert and not talk to anyone!" << endl;
					// Increment i so the loop ends immediately
					i++;
				}


				else
				{
					cout << "You chose to have a conversation with " << CPU.at(socialChoice - 1).getName() << "!" << endl;

					//Code to make random conversation 
					int randomConvoTopic = rand() % interests.size();
					string convoVariety = conversationVariationBlock();

					cout << "You talked to " << CPU.at(socialChoice - 1).getName() << " about " << convoVariety << interests.at(randomConvoTopic) << "!" << endl;

					// Code to determine how well the conversation went ( more social points=  more chances for better result)
					int randomConvoSuccess = rand() % 5 + -2;
					for (int i = 1; i < user.getSocial(); i++)
					{
						int tempNewValue = rand() % 5 + -2;
						if (randomConvoSuccess < tempNewValue)
						{
							randomConvoSuccess = tempNewValue;
						}
						// If new value is greater, store it in success for each trial (the higher social, the more trials)
					}

					user.conversationResult(CPU.at(socialChoice - 1), socialChoice - 1, randomConvoSuccess);
					// Offset's user bond with CPU at index socialChoice - 1 by (randomConvoSuccess)
					// Outputs text describing how the conversation went too

				}

			}
		}
		
		//Code loop for all CPUs to now have bonds (needed for logic in game)
		// If i = j (both are same indices), then it affects CPU's bond with USER, not other CPU's
		for (int i = 0; i < CPU.size(); i++)
		{
			for (int j = 0; j < CPU.size(); j++)
			{
				int randBond = rand() % 5 - 2;
				CPU[i].setRelationships(j, randBond);
			}
		}

		// Debugging: Printing relationships to track veto and nomination logic
		if (DEBUG)
		{
			for (int i = 0; i < CPU.size(); i++)
			{
				vector <int> currVector = CPU[i].getRelationships();
				for (int j = 0; j < CPU.size(); j++)
				{
					if (CPU[i].getName() == CPU[j].getName())
					{
						cout << CPU[i].getName() << "'s bond with " << user.getName() << " is " << currVector[j] << endl;
					}
					else
					{
						cout << CPU[i].getName() << "'s bond with " << CPU[j].getName() << " is " << currVector[j] << endl;
					}
				}
			}
		}

		// Loop if user is still in (spectator mode complicates code but I really wanted it)
		if (!user.getIsEliminated())
		{

			if (user.getIsHOH() == false)			// Code for user to influence HOH noms (run only when user not HOH)
			{
				// if this int = 1, user is trying to influence HOH's nominations
				int manipulateHOHnominations;

				convinceHOHnomsText();			// calls block of text from text.cpp describing what the user can do here

				cin >> manipulateHOHnominations;		// user inputs choice (1 if want to sway, any other input if not)
				if (manipulateHOHnominations != 1)
				{
					cout << "Boring!" << endl;
					cout << "Well okay. Time for nominations!" << endl;			// Cuts right to nominations
				}
				else	// user is attempting to sway HOH's decision
				{
					cout << "You've decided to try and influence the Head of Household's nominations!" << endl;
					cout << "Shady, shady, shady..." << endl;
					cout << "Well, who do you want to get nominated?" << endl;
					cout << "NOTE: Players who are less bonded toward you are more likely to nominate you and vote to evict you." << endl;

					// Code to display all options for nominations 
					for (int i = 0; i < CPU.size(); i++)
					{

						cout << i + 1 << ") " << setw(15) << left << CPU.at(i).getName() << " Bond: ";
						cout << user.getBond(i) << endl;

						//code to display their relationship value using the following:
						// Abysmal: relationship is negative
						// Weak: relationship = 0, 1, 2, 3
						// Friendly: relationship = 4, 5, 6
						// Close allies: relationship = 7, 8, 9
						// Number one allies: relationship = 10 (MAX)
					}
					cout << CPU.size() + 1 << ") No one, I changed my mind!" << endl;

					cout << "Note: You cannot try to have " << Houseguest::getLastHOH() << " nominated, as they are Head of Household." << endl;
					cout << "Entering the HOH's number will prompt you to be asked again." << endl;

					int potentialNominee1 = 0, potentialNominee2 = 0;
					bool properIndexCheck = false, check2 = false;

					// Code block to validate nominee 1
					while (!(properIndexCheck && check2))
					{
						// Loop to ensure first nominee is in vector (since only 1 nomination is valid)
						do {
							cout << "Who is the first player you want to have nominated? Type their player number here: ";
							cin >> potentialNominee1;
						} while (!(potentialNominee1 > 0 && potentialNominee1 <= CPU.size() + 1));

						properIndexCheck = true;		// number is going to be a proper index

						if (potentialNominee1 == CPU.size() + 1)
						{
							check2 = true;		// is the quit number so will be true, also protects from accessing invalid subscript
						}
						else if (CPU.at(potentialNominee1 - 1).getName() != Houseguest::getLastHOH())
						{
							check2 = true;		// selected HG is not HOH
						}
					}
					// We know nominee is not HOH and in bounds or that are we quitting, 1 is valid

					if (potentialNominee1 == CPU.size() + 1)
					{
						//User does not want to influence (changed mind)
						cout << "Well okay! Time for the HOH to make their nominations!" << endl;
					}
					else
					{
						properIndexCheck = false, check2 = false;
						while (!(properIndexCheck && check2))
						{
							// Loop to ensure second nominee is in vector
							do {
								cout << "Who is the second player you want to have nominated? Type their player number here: ";
								cin >> potentialNominee2;
							} while (!(potentialNominee2 > 0 && potentialNominee2 <= CPU.size() + 1));

							properIndexCheck = true;		// number is going to be a proper index

							if (potentialNominee2 == CPU.size() + 1)
							{
								check2 = true;		// is the quit number so will be true, also protects from accessing invalid subscript
							}
							else if (CPU.at(potentialNominee2 - 1).getName() != Houseguest::getLastHOH())
							{
								check2 = true;		// selected HG is not HOH
							}
						}
						cout << endl;
						if (potentialNominee2 == CPU.size() + 1)	
						{
							// Use overloaded "attemptToGetNominated" function to nominate one player (not 2 nom version)
							user.attemptToGetNominated(CPU.at(potentialNominee1 - 1), potentialNominee1 - 1, nominationReasons);
						}
						else
						{
							// Use overloaded "attemptToGetNominated" function to nominate two player (2 nom version)
							user.attemptToGetNominated(CPU.at(potentialNominee1 - 1), potentialNominee1 - 1, CPU.at(potentialNominee2 - 1), potentialNominee2 - 1, nominationReasons);

						}
					}
				}

			}
			else	// Still inside branch for user is not eliminated	
			{
				// Code block for if user is Head of Household

				int nominee1 = 0, nominee2 = 0;
				cout << "As Head of Household, you have the responsibility of nominating two houseguests for eviction." << endl;
				cout << "Keep in mind: You will lose social bond with whoever you nominate!" << endl;
				cout << "So, who do you want to nominate? You MUST choose two players." << endl;

				// List out options
				displayCPUvector(CPU);

				// Two do while loops to validate both nominations
				do
				{
					cout << "Enter your first nominee: ";
					cin >> nominee1;
				} while (nominee1 < 1 || nominee1 > CPU.size());
				CPU.at(nominee1 - 1).setIsNominee(true);
				do
				{
					cout << "Enter your second nominee: ";
					cin >> nominee2;
				} while (nominee2 < 1 || nominee2 > CPU.size() || nominee2 == nominee1);
				CPU.at(nominee2 - 1).setIsNominee(true);

			}
		}
		
		// If user wasn't HOH and couldn't convince, let CPU pick until there are 2 nominations
		user.fillRemainingNominees(CPU);


		/*	|---------------------|
			|	NOMINATIONS DONE! |
			|---------------------|	*/

		cout << "Now that the nominees are set, it's time to pick players for the veto competition!" << endl;
		vector<Houseguest*> nominees = user.getNominees(CPU);		// stores the nominees in nominees vector

		// Debugging: display nominations
		if (DEBUG)
		{
			for (Houseguest*& i : nominees)
			{
				cout << i->getName() << "is a nominee in the vector" << endl;
			}
		}

		// Set veto Participants vector with HOH and two nominees
		vector<Houseguest> vetoParticipants;
		vetoParticipants.push_back(user.findHOH(CPU));	
		vetoParticipants.push_back(*nominees.at(0));
		Houseguest::setFirstNom(nominees[0]->getName());
		vetoParticipants.push_back(*nominees.at(1));	
		Houseguest::setSecondNom(nominees[1]->getName());
		// Veto vector is populated

		//Text block declaring who is playing
		if (user.getIsHOH())
		{
			cout << "You are Head of Household, so you will compete." << endl;
		}
		else
		{
			cout << Houseguest::getLastHOH() << " is Head of Household, so they will compete." << endl;
		}
		cout << "Also, this week's nominees, " << nominees[0]->getName() << " and " << nominees[0]->getName() << ", will compete." << endl;
		cout << "Each of these houseguests will now draw another houseguest's veto chip to also compete in the veto." << endl;
		cout << "The Head of Household will draw first." << endl;

		// Normal format: 6 veto players! (Happens as long as 6 players are in the game)
		if (CPU.size() > 5)
		{
			vetoParticipants.push_back(user.pickVetoCompetitors(CPU, nominees, vetoParticipants));


			cout << Houseguest::getLastHOH() << " has picked " << vetoParticipants.at(3).getName() << "'s chip!" << endl;

			vetoParticipants.push_back(user.pickVetoCompetitors(CPU, nominees, vetoParticipants));
			cout << nominees.at(0)->getName() << " has picked " << vetoParticipants.at(4).getName() << "'s chip!" << endl;

			vetoParticipants.push_back(user.pickVetoCompetitors(CPU, nominees, vetoParticipants));
			cout << nominees.at(1)->getName() << " has picked " << vetoParticipants.at(5).getName() << "'s chip!" << endl;
		}
		else
		{
			if (!user.getIsEliminated() && !user.getIsNominee() && !user.getIsHOH())
			{
				vetoParticipants.push_back(user);
			}
			for (auto& cpu : CPU)
			{
				if (!cpu.getIsEliminated() && !cpu.getIsNominee() && !cpu.getIsHOH())
				{
					vetoParticipants.push_back(cpu);
				}
			}
		}

		/*	|---------------------|
			|	  VETO DRAW DONE! |
			|---------------------|	*/
		cout << "The competitors in this week's veto are: " << endl;
		for (Houseguest i : vetoParticipants)
		{
			cout << i.getName() << endl;
		}

		cout << "Now that we have our competitors, it's time for the veto competition!" << endl;
		cout << "The Power of Veto can be used to remove either " << nominees.at(0)->getName() << " or " << nominees.at(1)->getName() << " from the block." << endl;
		cout << "Also, the winner of the Power of Veto can be used to keep the nominees the same." << endl;


		// sets the POV winner
		int randVetoWinnerIndex;

		// Creates rand number between 0 and (# of veto participants) - 1
		randVetoWinnerIndex = rand() % vetoParticipants.size();

		// This person wins veto!
		vetoParticipants.at(randVetoWinnerIndex).setIsVetoWinner();
		bool CPUwonVeto = false;
		for (int i = 0; i < CPU.size(); i++)
		{
			// Code to see if CPU won, set true if one did
			if (CPU.at(i).getName() == vetoParticipants.at(randVetoWinnerIndex).getName())		
			{

				CPU.at(i).setIsVetoWinner();
				cout << CPU.at(i).getName() << " is the winner of the Power of Veto!" << endl;
				CPUwonVeto = true;
				Houseguest::setLastVetoWinner(CPU.at(i).getName());
			}
		}
		// If in the game, user wins veto by default
		if (!CPUwonVeto && !user.getIsEliminated())
		{
			user.setIsVetoWinner();
			cout << "Congratulations, " << user.getName() << ", you are the winner of the Power of Veto!" << endl;
			Houseguest::setLastVetoWinner(user.getName());
		}

		/*	|--------------------------|
			|	  VETO CHALLENGE DONE! |
			|--------------------------|	*/
		Houseguest::setVetoIsPlayed(true);

		// Code for if the user is the veto winner
		if (user.getIsVetoWinner())
		{
			int backdoorChoice = 0;
			// Text to store user's veto decision
			cout << "Do you want to use the Power of Veto?" << endl;
			do
			{
				cout << "Type 0 to save neither nominee, 1 to save ";
				if (nominees.at(0)->getName() == user.getName())
				{
					cout << "yourself, or 2 to save ";
				}
				else
				{
					cout << nominees.at(0)->getName() << ", or 2 to save ";
				}
				if (nominees.at(1)->getName() == user.getName())
				{
					cout << "yourself: " << endl;
				}
				else
				{
					cout << nominees.at(1)->getName() << ": " << endl;
				}
				cin >> backdoorChoice;
			} while (!(backdoorChoice == 0 || backdoorChoice == 1 || backdoorChoice == 2));

			if (backdoorChoice == 0)
			{
				cout << "You boringly decide to discard the power of veto." << endl;
				cout << "As a result, the nominations stay the same, and the eviction carries on as planned." << endl;
			}
			else if (backdoorChoice == 1)
			{
				// If vetoed self, remove self from nominees
				if (user.getName() == nominees[0]->getName())
				{
					user.setIsNominee(false);
					Houseguest::setLastVetoedPlayer(user.getName());
					nominees.erase(nominees.begin() + 0);
				}

				else
				{
					for (size_t i = 0; i < CPU.size(); i++)
					{
						// Loop all CPU to find the one being vetoeed
						if (CPU[i].getName() == nominees[0]->getName())
						{
							// Remove from nomines, set statics and user data
							CPU[i].setIsNominee(false);
							Houseguest::setLastVetoedPlayer(CPU[i].getName());
							nominees.erase(nominees.begin() + 0);

							// Break since removing more is illogical and not desired
							break;

						}
					}
				}
			}
			else	// means backdoorChoice == 2
			{
				// If vetoed self, remove self from nominees
				if (user.getName() == nominees[1]->getName())
				{
					user.setIsNominee(false);
					Houseguest::setLastVetoedPlayer(user.getName());
					nominees.erase(nominees.begin() + 1);
				}

				else
				{
					for (size_t i = 0; i < CPU.size(); i++)
					{
						// Loop all CPU to find the one being vetoeed
						if (CPU[i].getName() == nominees[1]->getName())
						{
							// Remove from nomines, set statics and user data
							CPU[i].setIsNominee(false);
							Houseguest::setLastVetoedPlayer(CPU[i].getName());
							nominees.erase(nominees.begin() + 1);

							// Break since removing more is illogical and not desired
							break;

						}
					}
				}
			}

			// THIS IS ALL DEBUGGING CODE!!
			// vvvvvvvvvvvvvvvvvvvvvvvvvvvv
			if (DEBUG)
			{
				cout << "At the end of user veto abilities, nominees vector is: ";
				for (auto& i : nominees)
				{
					cout << i->getName() << " ";
				}
				cout << endl;

				cout << "At the end of the user veto abilities, all people with nominated = true are: " << endl;
				if (user.getIsNominee())
				{
					cout << user.getName() << " ";
				}
				cout << endl;
				for (auto& i : CPU)
				{
					if (i.getIsNominee())
					{
						cout << i.getName() << " ";
					}
				}
			}

			cout << endl;

		}
		// USER IS NOT VETO WINNER
		else
		{
			// Veto winner not nominated:
			if (Houseguest::getLastVetoWinner() != nominees[0]->getName() && Houseguest::getLastVetoWinner() != nominees[1]->getName())
			{
				// Explanation of CPU veto system:
				//  1) Veto winner finds the nominee they are closer to
				//  2) Each bond = 1/10 chance independently to save them (Max odds expected to save)
				Houseguest* vetoWinner = user.findVetoWinner(CPU);

				// Debugging: checking relationship validation
				if (DEBUG)
				{
					cout << "Using getRelationships: " << endl << endl;
					vector<int> currRelationships = vetoWinner->getRelationships();
					for (int i = 0; i < CPU.size(); i++)
					{
						if (vetoWinner->getName() == CPU[i].getName())
						{
							cout << vetoWinner->getName() << "'s bond with " << user.getName() << ": " << currRelationships[i] << endl;
						}
						else
						{
							cout << vetoWinner->getName() << "'s bond with " << CPU[i].getName() << ": " << currRelationships[i] << endl;
						}
					}

					cout << "Using getSpecificRelationship:" << endl;
					for (int i = 0; i < CPU.size(); i++)
					{
						if (vetoWinner->getName() == CPU[i].getName())
						{
							cout << vetoWinner->getName() << "'s bond with " << user.getName() << ": " << vetoWinner->getSpecificRelationship(i) << endl;
						}
						else
						{
							cout << vetoWinner->getName() << "'s bond with " << CPU[i].getName() << ": " << vetoWinner->getSpecificRelationship(i) << endl;
						}
					}
				}


				// Get veto winner's bonds
				int nominee0Bond = vetoWinner->getSpecificRelationship(user.getIndexOfCPU(*nominees[0], CPU, CPU.size()));
				int nominee1Bond = vetoWinner->getSpecificRelationship(user.getIndexOfCPU(*nominees[1], CPU, CPU.size()));
				int higherBond = nominee0Bond;
				int indexOfHigherBond{ 0 };

				if (DEBUG)
				{
					cout << Houseguest::getLastVetoWinner() << "'s bond with " << nominees[0]->getName() << " is " << nominee0Bond << endl;
					cout << Houseguest::getLastVetoWinner() << "'s bond with " << nominees[1]->getName() << " is " << nominee1Bond << endl;
				}

				// Set higher bond and bond index correctly
				if (nominee1Bond > nominee0Bond)
				{
					higherBond = nominee1Bond;
					indexOfHigherBond = 1;
				}
				else if (nominee1Bond == nominee0Bond)
				{
					indexOfHigherBond = (rand() % 10 == 0) ? 0 : 1;
					// Random choice if both bonds are equal
				}

				// If both bonds are neagitve, no attempt to veto (loop will iterate from 0 to 0)
				if (higherBond < 0)
				{
					higherBond = 0;
				}

				// Bonds are set
				// Now let CPU make veto attempts
				for (int i = 0; i < higherBond; i++)
				{
					if (rand() % 10 == 0) // 1 in 10 chance of success
					{
						// nominee is vetoed
						nominees[indexOfHigherBond]->setIsNominee(false);
						Houseguest::setLastVetoedPlayer(nominees[indexOfHigherBond]->getName());
						cout << vetoWinner->getName() << " has used the Power of Veto on " << nominees[indexOfHigherBond]->getName() << "!" << endl;
						nominees.erase(nominees.begin() + indexOfHigherBond);

						// Veto is used, break loops
						break;
					}
				}
			}

			// see if the veto winner is nominated (if so must veto themself)
			else
			{
				for (size_t i = 0; i < CPU.size(); i++)
				{
					if (CPU[i].getIsVetoWinner() && CPU[i].getIsNominee())
					{
						// Veto winner is nominee, they remove themself:

						cout << "Since " << CPU[i].getName() << " is nominated and won the veto, they will automatically veto themselves." << endl;
						CPU[i].setIsNominee(false);
						Houseguest::setLastVetoedPlayer(CPU[i].getName());

						for (size_t nomIndex = 0; nomIndex < nominees.size(); nomIndex++)
						{
							// Code to loop until they are removed from nominees vector
							if (nominees[nomIndex]->getName() == CPU[i].getName() && !CPU[i].getIsNominee())
							{
								nominees.erase(nominees.begin() + nomIndex);
								break;								
							}
						}
					}
				}
			}
		}

		/*	|--------------------------|
			|	  VETO USAGE DONE!     |
			|--------------------------|	*/
		// Now there are either 1 OR 2 Nominees (veto was used or discarded)


		// If someone was vetoed
		if (nominees.size() == 1)
		{
			if (user.getIsHOH())
			{
				// User chooses new nominee
				// Lists people that cannot be nominated
				cout << "Who do you name as the replacement nominee?" << endl;
				cout << "You can choose anyone besides the current nominee, " << nominees[0]->getName();
				if (!user.getIsVetoWinner())
				{
					cout << ", and you may not enter the Power of Veto Winner," << Houseguest::getLastVetoWinner();
				}
				if (Houseguest::getLastVetoWinner() != Houseguest::getLastVetoedPlayer())
				{
					cout << ", and you may not enter the vetoed nominee, " << Houseguest::getLastVetoedPlayer();
				}
				cout << "." << endl;

				int newNominee{ 0 };
				for (int i = 0; i < CPU.size(); i++)
				{
					// List CPUs and bond words
					cout << i + 1 << ") " << setw(15) << left << CPU.at(i).getName() << " Bond: ";
					cout << user.getBond(i) << endl;
				}

				cout << "Who is the player you want to have nominated? Type their player number here: ";
				cin >> newNominee;
				bool keepLooping{ true };
				while (keepLooping)
				{
					if (newNominee > 0 && newNominee < CPU.size())
					{

						if (CPU[newNominee - 1].getName() != Houseguest::getLastVetoedPlayer()
							&& CPU[newNominee - 1].getName() != Houseguest::getLastVetoWinner() && CPU[newNominee - 1].getName() != nominees[0]->getName())
						{
							// Nominee choice is valid, set as nominee
							CPU[newNominee - 1].setIsNominee(true);
							keepLooping = false;
						}

						// Invalid nomination indices with explanations:
						else if (CPU[newNominee - 1].getName() == Houseguest::getLastVetoWinner())
						{
							cout << "You cannot try to nominate the Power of Veto holder. Enter a new houseguest." << endl;
						}
						else if (CPU[newNominee - 1].getName() == Houseguest::getLastVetoedPlayer())
						{
							cout << "You cannot try to nominate the player that was just vetoed. Enter a new houseguest." << endl;
						}
						else if (CPU[newNominee - 1].getName() == nominees[0]->getName())
						{
							cout << "You cannot target a player that is already nominated. Enter a new houseguest." << endl;
						}
					}
					if (keepLooping)
					{
						// Invalid input
						cout << "Please enter a valid player number." << endl;
						cin >> newNominee;
					}
				}

			}
			else if (!user.getIsHOH() && !user.getIsEliminated())
			{
				// Give user a chance to influence re-nomination
				int newNominee{ 0 };
				cout << "Since the veto was used, you can try and sway the HOH using your strategy stat!" << endl;
				cout << "Do you want to try and target another player for nomination?" << endl;
				cout << "If you succeed, your chosen player will be the replacement nominee." << endl;
				cout << "If you fail, you will lose bond with the selected player. " << endl;
				cout << "Type 1 to attempt to convince the veto holder to name a new nominee: ";
				cin >> newNominee;

				if (newNominee != 1)
				{
					// Jumps to veto ceremony
					cout << "Boring!" << endl;
					cout << "Well okay!" << endl;
				}
				else
				{
					// Print out nomination options
					cout << "Well, who do you want to get nominated?" << endl;
					for (int i = 0; i < CPU.size(); i++)
					{

						cout << i + 1 << ") " << setw(15) << left << CPU.at(i).getName() << " Bond: ";
						cout << user.getBond(i) << endl;
					}
					cout << CPU.size() + 1 << ") No one, I changed my mind!" << endl;

					// List who you cannot suggest
					cout << "Note: You cannot try to have " << Houseguest::getLastHOH() << " nominated, as they are Head of Household." << endl;
					cout << "You also may not enter the current nominee, " << nominees[0]->getName();
					if (Houseguest::getLastVetoWinner() != nominees[0]->getName() && Houseguest::getLastVetoWinner() != Houseguest::getLastHOH())
					{
						cout << ", and you also may not enter the veto winner, " << Houseguest::getLastVetoWinner();
					}
					if (Houseguest::getLastVetoedPlayer() != "" && Houseguest::getLastVetoedPlayer() != Houseguest::getLastVetoWinner())
					{
						cout << ", and you also may not enter the veteod nominee, " << Houseguest::getLastVetoedPlayer();
					}
					cout << "." << endl;

					bool nominate{ false };
					bool dontNominate{ false };
					// if entered index is above 0 and below cpu.size OR if its equal to the quit command, check to ensure
					// it is not equal to veto winner or vetoed player or hoh or nominee
					while (!(nominate || dontNominate))
					{
						cout << "Who is the player you want to have nominated? Type their player number here: ";
						cin >> newNominee;
						if (newNominee > 0 && newNominee < CPU.size() + 1)
						{

							if (CPU[newNominee - 1].getName() != Houseguest::getLastHOH() && CPU[newNominee - 1].getName() != Houseguest::getLastVetoedPlayer()
								&& CPU[newNominee - 1].getName() != Houseguest::getLastVetoWinner() && CPU[newNominee - 1].getName() != nominees[0]->getName())
							{
								// User wants to nominate still, set nominate bool = true
								nominate = true;
							}

							// Invalid indices with explanations
							else if (CPU[newNominee - 1].getName() == Houseguest::getLastHOH())
							{
								cout << "You cannot try to nominate the Head of Household. Enter a new houseguest." << endl;
							}
							else if (CPU[newNominee - 1].getName() == Houseguest::getLastVetoWinner())
							{
								cout << "You cannot try to nominate the Power of Veto holder. Enter a new houseguest." << endl;
							}
							else if (CPU[newNominee - 1].getName() == Houseguest::getLastVetoedPlayer())
							{
								cout << "You cannot try to nominate the player that was just vetoed. Enter a new houseguest." << endl;
							}
							else if (CPU[newNominee - 1].getName() == nominees[0]->getName())
							{
								cout << "You cannot target a player that is already nominated. Enter a new houseguest." << endl;
							}
						}
						else if (newNominee == CPU.size() + 1)
						{
							cout << "Okay thanks for wasting my time! Moving on!" << endl;
							dontNominate = true;
						}
					}
					// If trying to nominate, try to nominate the specified user
					if (nominate)
						user.attemptToGetNominated(CPU.at(newNominee - 1), newNominee - 1, nominationReasons);
					// Logic on this function:
						// FILL IN
				}
			}
		}

		// If suggestions were not taken, add new nominee and then update nominee vector
		user.fillRemainingNominees(CPU);
		nominees = user.getNominees(CPU);
		
		/*	|--------------------------|
			|	 NOMINEES ARE FINAL!   |
			|--------------------------|	*/

		cout << endl;
		cout << "The final nominees are " << nominees[0]->getName() << " and " << nominees[1]->getName() << endl;

		// Loop to set the replacement nominee
		for (auto& nom : nominees)
		{
			if (nom->getName() != Houseguest::getFirstNom() && nom->getName() != Houseguest::getSecondNom())
			{
				Houseguest::setReplacementNom(nom->getName());
			}
		}

		// Debugging: Display who can vote
		if (DEBUG)
		{
			cout << "Voting: ";
			for (int i = 0; i < CPU.size(); i++)
			{
				if (CPU.at(i).getName() != Houseguest::getLastHOH() && !CPU.at(i).getIsNominee())
				{
					cout << CPU.at(i).getName() << " is voting. " << endl;
				}
			}
			if (user.getName() != Houseguest::getLastHOH() && !user.getIsNominee())
			{
				cout << "You are also voting" << endl;
			}
		}

		/*	|--------------------------|
			|	   EVICTION BEGINS!    |
			|--------------------------|	*/

		int nom1VotesAgainst{ 0 }, nom2VotesAgainst{ 0 };
		cout << "Now it's time for the live eviction! Before votes are cast, both nominees have a chance to speak." << endl;
		cout << "Wow, what great speeches..... anyway!" << endl;

		cout << "It's time to cast your votes to evict. As Head of Household, " << Houseguest::getLastHOH();
		cout << ", you will only vote in the event of a tie." << endl;

		cout << "As nominees, " << nominees[0]->getName() << " and " << nominees[1]->getName() << " also are not eligible to vote." << endl;

		// Code for user to cast eviction vote
		if (user.eligibleToVote())
		{
			string evictChoice = "";
			do
			{
				cout << "Who do you vote to evict? " << nominees[0]->getName() << " or " << nominees[1]->getName() << ":" << endl;
				cin >> evictChoice;
			} while (!(evictChoice == nominees[0]->getName() || evictChoice == nominees[1]->getName()));

			// Validated input, increment respective nominee's votes
			if (evictChoice == nominees[0]->getName())
			{
				nom1VotesAgainst++;
				cout << "You casted your vote to evict " << nominees[0]->getName() << "." << endl;
			}
			else
			{
				nom2VotesAgainst++;
				cout << "You casted your vote to evict " << nominees[1]->getName() << "." << endl;
			}
		}
		else
		{
			cout << "You watch anxiously as the other houseguests cast their votes..." << endl;
		}

		// Code for each CPU to vote (it is random)
		for (auto& cpu : CPU)
		{
			if (cpu.eligibleToVote())
			{
				cout << cpu.getName() << " casts their vote to evict ";
				if (rand() % 2 == 0)
				{
					cout << nominees[0]->getName() << endl;
					nom1VotesAgainst++;
				}
				else
				{
					cout << nominees[1]->getName() << endl;
					nom2VotesAgainst++;
				}
			}

		}
		if (DEBUG)
		{
			cout << "Expected votes: " << CPU.size() - 2 << endl;
			cout << "Votes for " << nominees[0]->getName() << ": " << nom1VotesAgainst << endl;
			cout << "Votes for " << nominees[1]->getName() << ": " << nom2VotesAgainst << endl;
		}

		if (nom1VotesAgainst > nom2VotesAgainst)
		{
			cout << "By a vote of " << nom1VotesAgainst << " to " << nom2VotesAgainst;
			cout << ", " << nominees[0]->getName() << ", you are evicted from the Big Brother House." << endl;

			// Set evicted person's status
			for (size_t i = 0; i < CPU.size(); i++)
			{
				if (CPU[i].getName() == nominees[0]->getName())
				{
					CPU[i].setIsEliminated(true);
				}
			}

			// Check if user was eliminated (and was nominee[0])
			if (user.getName() == nominees[0]->getName())
			{
				user.setIsEliminated(true);
			}
		}
		else if (nom2VotesAgainst > nom1VotesAgainst)
		{
			cout << "By a vote of " << nom2VotesAgainst << " to " << nom1VotesAgainst;
			cout << ", " << nominees[1]->getName() << ", you are evicted from the Big Brother House." << endl;

			// Set evicted person as evicted
			for (size_t i = 0; i < CPU.size(); i++)
			{
				if (CPU[i].getName() == nominees[1]->getName())
				{
					CPU[i].setIsEliminated(true);
				}
			}

			// Check if user was evicted (and was nominee[1])
			if (user.getName() == nominees[1]->getName())
			{
				user.setIsEliminated(true);
			}
		}
		else
		{
			cout << "The votes are in, and we have a tie! As Head of Household, " << Houseguest::getLastHOH() << " must break the tie." << endl;
			if (user.getName() == Houseguest::getLastHOH())
			{
				cout << "Who do you vote to evict? Whoever you choose WILL go home." << endl;

				string evictChoice = "";
				do
				{
					cout << "Who do you vote to evict? " << nominees[0]->getName() << " or " << nominees[1]->getName() << ":" << endl;
					cin >> evictChoice;
				} while (!(evictChoice == nominees[0]->getName() || evictChoice == nominees[1]->getName()));

				if (evictChoice == nominees[0]->getName())
				{
					cout << "You casted your vote to evict " << nominees[0]->getName() << "." << endl;
					cout << "As a result, " << nominees[0]->getName() << ", you are evicted from the Big Brother House." << endl;
					// set evicted person as evicted
					for (size_t i = 0; i < CPU.size(); i++)
					{
						if (CPU[i].getName() == nominees[0]->getName())
						{
							CPU[i].setIsEliminated(true);
							//cout << CPU[i].getName() << "is eliminated." << endl;
						}
					}
					// remove person from game
				}
				else
				{
					cout << "You casted your vote to evict " << nominees[1]->getName() << "." << endl;
					cout << "As a result, " << nominees[1]->getName() << ", you are evicted from the Big Brother House." << endl;
					// set evicted person as evicted
					for (size_t i = 0; i < CPU.size(); i++)
					{
						if (CPU[i].getName() == nominees[1]->getName())
						{
							CPU[i].setIsEliminated(true);
						}
					}
					// remove person from game
				}
			}
			// User has cast their tie-breaking vote
			// Now let CPU cast tie-breaker vote
			else
			{
				cout << Houseguest::getLastHOH() << " casts the tie-breaking vote to evict ";
				if (rand() % 2 == 0)
				{
					cout << nominees[0]->getName() << endl;
					nominees[0]->setIsEliminated(true);
					nom1VotesAgainst++;
				}
				else
				{
					cout << nominees[1]->getName() << endl;
					nominees[1]->setIsEliminated(true);
					nom2VotesAgainst++;
				}
			}
		}
		/*	|--------------------------|
			|	  TIE BREAKER DONE!    |
			|--------------------------|	*/
		// Now resize CPU (removes the eliminated CPU)
		user.resizeCPU(CPU, CPU.size(), eliminated, eliminated.size());
		// CPU vector has now been cleansed (each index has reset values)


		// Code below: Recap of events of week
		cout << "A recap of Week" << Houseguest::getRoundOfGame() << ":" << endl;
		cout << Houseguest::getLastHOH() << " initially nominated " << Houseguest::getFirstNom() << " and " << Houseguest::getSecondNom() << "." << endl;
		cout << Houseguest::getLastVetoWinner() << " won the veto and decided to ";
		if (Houseguest::getLastVetoedPlayer() != "")
		{
			cout << "use the power of veto on ";
			if (Houseguest::getLastVetoedPlayer() == Houseguest::getLastVetoWinner())
			{
				cout << "themself";
			}
			else
			{
				cout << Houseguest::getLastVetoedPlayer();
			}
			cout << ", and " << Houseguest::getLastHOH() << " nominated ";
			cout << Houseguest::getReplacementNom() << " as a replacement nominee." << endl;
		}
		else
		{
			cout << "keep the nominations the same." << endl;
		}

		cout << eliminated[eliminated.size() - 1].getName() << " was then evicted by a vote of ";
		if (nom1VotesAgainst > nom2VotesAgainst)
		{
			cout << nom1VotesAgainst << " - " << nom2VotesAgainst << "." << endl;
		}
		else if (nom2VotesAgainst > nom1VotesAgainst)
		{
			cout << nom2VotesAgainst << " - " << nom1VotesAgainst << "." << endl;
		}
		else
		{
			cout << nom2VotesAgainst << " - " << nom1VotesAgainst << ", with the tie broken by " << Houseguest::getLastHOH() << "." << endl;
		}
		
		// Code to display eliminated players
		if (DEBUG)
		{
			cout << "Eliminated vector: " << endl;
			for (auto& elim : eliminated)
			{
				cout << elim.getName();
			}
		}

		cout << endl;
		cout << "With that, week " << Houseguest::getRoundOfGame() << " of the game is complete! " << CPU.size() << " other players remain!" << endl;
		int choice{ 0 };

		// If user was eliminated:
		if (user.getIsEliminated())
		{
			do
			{
				cout << "You have been eliminated. Do you want to spectate the game or quit?" << endl;
				cout << "Type 1 to quit and 0 to continue: ";
				cin >> choice;
			} while (choice != 1 && choice != 0);

			if (choice == 1)
			{
				cout << "You chose to quit watching." << endl;
				keepPlaying = false;
			}
			else
			{
				cout << "You have decided to spectate. Type ctrl+c any time to quit." << endl;
			}
		}
		if (keepPlaying)
		{
			cout << "Next round begins now!" << endl;
		}
		numOfOpponents--;
		Houseguest::setRoundOfGame(1);
	}

	// Debugging: Check if both conditions are true
	if (DEBUG)
	{
		if (keepPlaying)
		{
			cout << "Keep playing is still true";
		}
		if (numOfOpponents + 1 - eliminated.size() >= 4)
		{
			cout << "Other part is true";
		}
		if (keepPlaying && (numOfOpponents + 1 - eliminated.size() >= 4))
		{
			cout << "Both are true";
		}
	}

	/*	|--------------------------|
		|   FINALE IS BEGINNING!   |
		|--------------------------|	*/
	if (keepPlaying)
	{
		// List the top three players
		cout << "We have made it to the finale! Our top three players are: ";
		if (!user.getIsEliminated())
		{
			cout << user.getName() << ", ";
			for (int i = 0; i < CPU.size(); i++)
			{
				if (i == 1)
				{
					cout << " , and ";
				}
				cout << CPU[i].getName();
			}
			cout << "!" << endl;
		}
		else
		{
			for (int i = 0; i < CPU.size(); i++)
			{
				if (i == 2)
				{
					cout << " , and ";
				}
				else if (i == 1)
				{
					cout << ", ";
				}
				cout << CPU[i].getName();
			}
			cout << "!" << endl;
		}


		cout << "The final three will compete in a three-part HOH competition, where the winners of Part 1 and 2 compete in Part 3." << endl;
		cout << "First, we need to play Part 1." << endl;

		// Code to decide final HOH (options based on if user is still in the game)
		string finalHOH;
		if (!user.getIsEliminated())
		{
			finalHOH = user.finalHOHWithUser(CPU, CPU.size());
		}
		else
		{
			finalHOH = user.finalHOHWithoutUser(CPU, CPU.size());
		}
		Houseguest::setLastHOH(finalHOH);

		for (int i = 0; i < CPU.size(); i++)
		{
			if (CPU[i].getName() == finalHOH)
			{
				CPU[i].setIsHOH(true);
			}
		}

		if (user.getName() == finalHOH)
		{
			int evictChoice{ 0 };

			user.setIsHOH(true);
			cout << "You have the final decision to decide what player to eliminate!" << endl;
			cout << "Whoever you keep in the game will sit next to you when the jury votes!" << endl;
			displayCPUvector(CPU);

			do
			{
				cout << "So, who do you vote to evict?" << endl;
				cout << "Type the NUMBER of the player: ";
				cin >> evictChoice;
			} while (evictChoice != 1 && evictChoice != 2);

			cout << "You have chosen to evict " << CPU[evictChoice - 1].getName();

			CPU[evictChoice-1].setIsEliminated(true);
			eliminated.push_back(CPU[evictChoice - 1]);
			CPU.erase(CPU.begin() + evictChoice - 1);

			// User has officially evicted one of the other two players
		}

		else if (user.getName() != finalHOH && !user.getIsEliminated())
		{
			int randElim = rand() % 2;
			if (randElim == 0)	// user loses
			{
				cout << finalHOH << " has decided to evict you, meaning you are the final person evicted from the Big Brother House. " << endl;
				user.setIsEliminated(true);
				eliminated.push_back(user);
			}
			else
			{
				int maxChanges{ 0 };
				for (int i = 0; i < CPU.size(); i++)
				{
					if (CPU[i].getName() != finalHOH)
					{
						cout << finalHOH << " has decided to evict " << CPU[i].getName();
						cout << ", meaning they are the final person evicted from the Big Brother House. " << endl;
						CPU[i].setIsEliminated(true);
						eliminated.push_back(CPU[i]);
						break;
					}
				}
			}

		}

		// Runs if current player is eliminated
		else
		{
			for (int i = 0; i < CPU.size(); i++)
			{
				if (CPU[i].getName() != finalHOH)
				{
					cout << finalHOH << " has decided to evict " << CPU[i].getName();
					cout << ", meaning they are the final person evicted from the Big Brother House. " << endl;
					CPU[i].setIsEliminated(true);
					eliminated.push_back(CPU[i]);
					break;
				}
			}
		}
		// Declare reference vector for final two
		vector<Houseguest*> finalTwo;
		if (!user.getIsEliminated())
		{
			cout << "You are in the final two with " << CPU[0].getName() << "!" << endl;
			finalTwo.push_back(&user);
			finalTwo.push_back(&CPU[0]);
		}
		else
		{
			cout << "The final two are " << CPU[0].getName() << " and " << CPU[1].getName() << "!" << endl;
			finalTwo.push_back(&CPU[0]);
			finalTwo.push_back(&CPU[1]);
		}

		cout << "The winner will be decided by the jury. Here are our nine jurors!" << endl;

		// Eliminated should have 14 entries with indices 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13.
		// We want to ignore the first five indices (BB Format declares them as prejury)
		for (int i = 5; i < eliminated.size(); i++)
		{
			cout << "Juror #" << i - 4 << ": " << eliminated[i].getName() << endl;
		}

		cout << "Now let's see who wins!" << endl;
		int finalist1Votes{ 0 }, finalist2Votes{ 0 };

		// Show all juror votes
		for (int i = 5; i < eliminated.size(); i++)
		{
			int randVote = rand() % 2;
			cout << "Juror #" << i - 4 << ": " << eliminated[i].getName() << " has voted for " << finalTwo[randVote]->getName() << " to win!" << endl;
			if (randVote == 0)
			{
				finalist1Votes++;
			}
			else
			{
				finalist2Votes++;
			}
		}
		if (DEBUG)
		{
			cout << "Expected total: 9" << endl;
			cout << "finalist1Votes: " << finalist1Votes << endl;
			cout << "finalist2Votes: " << finalist2Votes << endl;
		}

		if (finalist1Votes > finalist2Votes)
		{
			cout << "The Winner of Big Brother Interactive is.... " << finalTwo[0]->getName() << "!!" << endl;
		}
		else
		{
			cout << "The Winner of Big Brother Interactive is.... " << finalTwo[1]->getName() << "!!" << endl;
		}
	}
	return 0;
}
// End of simulation!
