#ifndef HOUSEGUEST_H
#define HOUSEGUEST_H

#include <string>
#include <vector>
using namespace std;

class Houseguest
{
private:
	int strength{ 1 };	// User's likelihood to win comps (can increase over time)
	int strategy{ 1 };	// User's likelihood to have positive social interactions (can increase over time)
	int social{ 1 };		// User's likelihood to have positive strategic interactions (can increase over time)
	// int threat;		// threat level of player (makes more likely to be nominated/evicted (DNI yet)

	int typeOfPlayer;	// User's type of player (1 for physical, 2 for social, 3 for strategic

	string name;

	vector<int> relationships;						// holds each Houseguest's relationship with each other houseguest

	bool eliminated = false;						// checks if player is still in game after an eviction
	bool isHOH = false;								// checks if player is HOH (for noms/power plays)
	bool isNominee = false;							// checks if player isNominee (for power plays/voting)
	bool isVetoWinner = false;						// checks if veto winner (for power plays/choices)
	bool inFinalHOH = false;

	//static int numOfPlayersInGame;
	static int roundOfGame;
	static string lastHOH;
	static string lastVetoWinner;
	static string lastVetoedPlayer;
	static bool vetoIsPlayed;
	static string firstNom;
	static string secondNom;
	static string replacementNom;

public:
	Houseguest() = default;										// default constructor for initializing houseguests

	void setStrength();				// sets strength when user didnt specialize in it
	void setStrength(int n);		// sets strength when user specialized in it
	int getStrength();
	void increaseStrength(int);		// increase strength value


	void setStrategy();				// sets strategy when user didnt specialize in it
	void setStrategy(int n);		// sets strategy when user  specialized in it
	int getStrategy();
	void increaseStrategy(int);		// increase strategy value

	void setTypeOfPlayer(int type);			// sets users/CPUs types
	int getTypeOfPlayer();


	void setSocial();				// sets social when user didnt specialize in it
	void setSocial(int n);			// sets strength when user specialized in it 
	int getSocial();
	void increaseSocial(int);			// increase social value

	void setName(string n);			// stores user's houseguest name
	string getName();

	void setSizeAndValuesOfRelationships(int);	// sets size of relationship vector

	void setRelationships(int, int);
	vector<int>& getRelationships();			// function to access relationships vector
	int getSpecificRelationship(int);

	string getBond(int index);
	int getIndexOfCPU(Houseguest& hg, vector<Houseguest>&, size_t);
	void conversationResult(Houseguest&, int, int);			// for conversation sociali result incrementing

	void attemptToGetNominated(Houseguest&, int, vector<string>);			// function for when user suggests one nominee
	void attemptToGetNominated(Houseguest&, int, Houseguest&, int, vector<string>);		// function for whne user suggests two nominees
	void fillRemainingNominees(vector<Houseguest>&);

	vector<Houseguest*> getNominees(vector<Houseguest>&);

	Houseguest pickVetoCompetitors(vector<Houseguest>&, vector<Houseguest*>&, vector<Houseguest>&);

	void setIsEliminated(bool var);			// need a local variable in code "evicted"
	bool getIsEliminated();

	void setIsHOH(bool var);				// need a local variable in code "HOH"
	bool getIsHOH();
	Houseguest findHOH(vector<Houseguest>& CPU);
	Houseguest* findVetoWinner(vector<Houseguest>& CPU);


	void setIsNominee(bool var);			// need a local variable in code "nom"
	bool getIsNominee();

	void setIsVetoWinner();			// need a local variable in code "veto_holder"
	bool getIsVetoWinner();

	void setInFinalHOH(bool result);
	bool getInFinalHOH();

	void resetStatus();

	void displayUserStats();

	static int getRoundOfGame();
	static void setRoundOfGame(int round);

	static void setLastHOH(string);
	static string getLastHOH();

	static void setLastVetoWinner(string);
	static string getLastVetoWinner();

	static string getLastVetoedPlayer();
	static void setLastVetoedPlayer(string);

	static void setFirstNom(string);
	static string getFirstNom();

	static void setSecondNom(string);
	static string getSecondNom();

	static void setReplacementNom(string);
	static string getReplacementNom();

	static void setVetoIsPlayed(bool);
	static bool getVetoIsPlayed();

	friend void displayCPUvector(vector<Houseguest> CPU);

	bool eligibleToVote();
	void resizeCPU(vector<Houseguest>& CPU, int cpuSize, vector<Houseguest>& eliminated, int elimSize);

	string finalHOHWithUser(vector<Houseguest>& CPU, int cpuSize);
	string finalHOHWithoutUser(vector<Houseguest>& CPU, int cpuSize);

};
#endif

