#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "Cards.h" // Need for Deck
#include "Player.h"
#include "Map.h"
#include <set>
using namespace std;

class Territory;
class Player;
class Deck;

void PlayerStrategyDriver();

enum PlayerType
{
	Human,
	Aggressive,
	Benevolent,
	Neutral,
	Cheater
};

//abstract Playerstrategy class
class PlayerStrategy
{
public:
	//PlayerStrategy(Player*, Player*, Deck*);
	PlayerStrategy& operator = (const PlayerStrategy&); //added;
	//create method that will change strategy in the isuueing orders phase
	virtual int changeStrategy(string, int) = 0;
	//implement issueOrder(), toAttack(), toDefend() in different ConcreteStrategy 
	virtual void issueOrder() = 0;
	virtual vector<Territory*> toAttack() = 0;
	virtual vector<Territory*> toDefend() = 0;
	virtual void PrintStrategy() = 0;

	// Added by Justine & Jennifer

	void setDeck(Deck*);

	void updateAllPlayers(vector<Player*>);

protected:
	Player* player;		//Put in PlayerStrategy instead of in each subclass
	vector<Player*> allPlayers; // Added by Justine & jennifer, need it to keep track of all the players in
						// the game & use it in Blockade
	Deck* deck;
};

//ConcreteStrategy classes
class HumanPlayerStrategy : virtual public PlayerStrategy {
public:
	HumanPlayerStrategy(Player*, vector<Player*>, Deck*);
	~HumanPlayerStrategy();
	HumanPlayerStrategy(const HumanPlayerStrategy& p); //Copy constructor
	HumanPlayerStrategy& operator = (const HumanPlayerStrategy&); //added;
	int changeStrategy(string, int);
	//implement issueOrder(), toAttack(), toDefend() 
	void issueOrder();
	vector<Territory*> toAttack();
	vector<Territory*> toDefend();

private:
	void PrintStrategy();
	// Changes done by Justine
	int ChooseValidOption(int max);
	int GetArmyNumber(int army); //getArmies() in class Territory
	void ShowTerritory();
	vector<Territory*> GetEnemyTerritory();
	set<Player*> GetPlayers();
	//Player* player;
};

class AggressivePlayerStrategy : virtual public PlayerStrategy {
public:
	AggressivePlayerStrategy(Player* p, vector<Player*> all, Deck* d);
	~AggressivePlayerStrategy();
	AggressivePlayerStrategy(const AggressivePlayerStrategy& p); //Copy constructor
	AggressivePlayerStrategy& operator = (const AggressivePlayerStrategy&); //added;
	vector<Territory*> getTerritoryNeighbor(Territory*);
	int changeStrategy(string, int);
	//implement issueOrder(), toAttack(), toDefend() 
	void issueOrder();
	vector<Territory*> toAttack();
	vector<Territory*> toDefend();
	void PrintStrategy();

private:
	//Player* player;
};

class BenevolentPlayerStrategy : virtual public PlayerStrategy {
public:
	BenevolentPlayerStrategy(Player*, vector<Player*>, Deck*);
	~BenevolentPlayerStrategy();
	BenevolentPlayerStrategy(const BenevolentPlayerStrategy& p); //Copy constructor
	BenevolentPlayerStrategy& operator = (const BenevolentPlayerStrategy&); //added
	int changeStrategy(string, int);
	//implement issueOrder(), toAttack(), toDefend() 
	void issueOrder();
	vector<Territory*> toAttack();
	vector<Territory*> toDefend();
	void PrintStrategy();

private:
	//Player* player;
};

class NeutralPlayerStrategy : public PlayerStrategy {
public:
	NeutralPlayerStrategy(Player* player, vector<Player*> all, Deck* d);
	//create method that will change strategy in the isuueing orders phase
	virtual int changeStrategy(string, int);
	//implement issueOrder(), toAttack(), toDefend() in different ConcreteStrategy 
	virtual void issueOrder();
	virtual vector<Territory*> toAttack();
	virtual vector<Territory*> toDefend();
	void PrintStrategy();
	~NeutralPlayerStrategy();
private:
	//Player* player;
	int territoryCount;
	int armyCount;
};

class CheaterPlayerStrategy : public PlayerStrategy {
public:
	CheaterPlayerStrategy(Player* player, vector<Player*> all, Deck* d);
	//create method that will change strategy in the isuueing orders phase
	virtual int changeStrategy(string, int);
	//implement issueOrder(), toAttack(), toDefend() in different ConcreteStrategy 
	virtual void issueOrder();
	virtual vector<Territory*> toAttack();
	virtual vector<Territory*> toDefend();
	void PrintStrategy();
private:
	//Player* player;
};