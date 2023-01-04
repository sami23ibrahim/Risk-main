#pragma once

#include "Player.h"
#include "Cards.h"
#include "Orders.h"
#include "Map.h"
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <chrono>       // std::chrono::system_clock
//#include "Deck.h"
//#include "MapLoader.h"
#include "LoggingObserver.h"
#include "CommandProcessing.h"

using namespace std;
//
//class CommandProcessor;//((target))
//class Command;


void EngineDriver();



class Deck;

// enum Phase {StartUp, Play };

enum  State{
	null, start, maploaded, mapvalidated, playersadded,
	reinforcementPhase, issueOrderPhase, executeOrderPhase, win, tournamentEnd, tournamentStart
};

class GameEngine: public Subject, public ILoggable {

public:
	
	// constructors
	GameEngine();
	GameEngine(GameEngine& engine);
	//Destructor:
	~GameEngine();


	//Assignment operator
	GameEngine& operator =(const GameEngine& other);
	//Stream insertion
	friend ostream& operator << (ostream& out, const GameEngine& g);

	// Mutators
	void SetState(State);
	void ExecuteOrders();
	State GetState();
	Map* GetMap();
	Deck* GetDeck();
	void AddPlayers();
	int GetNumberOfPlayers();
	vector<Player*> GetPlayers();
	vector<Player*>* GetPlayersAdress();
	void SetNumberOfPlayers(int);

	void AddPlayer();
	void AddStrategyPlayer(string playerName, int playerID);
	// void Init();
	void Start();

	void LoadMap();
	void LoadMap(Command* commandEntered);
	bool ValidateMap();
	bool ValidateMapOld();
	
	void IssueOrdersPhase();
	//void IssueOrdersPhase(Player* player);

	Player* MainGameLoop();
	void StartupPhase();
	void ReinforcementPhase();

	void ExecuteOrdersPhase();
	//void ExecuteOrdersPhase(Player* player);


	void PlayerDrawCard(Player* player);
	
	string stringToLog();

	string SelectName(string command);
	

	void GameStart();
	CommandProcessor* processor;
	Command* commandEntered;

	void TakeInput();

	// fairly distribute all the territories to the players
	void DistributeTerritories();

	// randomize the order of play of the players in the game
	void ShufflePlayers();

	void AttachToOrdersList(LogObserver* observer);
	void AttachToProcessor(LogObserver* observer);

	



private:

	bool tournamentMode = false;
	State state;
	Map* map;
	MapLoader* mapLoader; //for reinforcement phase
	vector<Player*> players;
	Deck* deck;
	int numberOfPlayers;
	int NumberOfTerritories;

	// tournament mode
	// all of these should be reset for each game
	vector<string> tournamentMaps; // vector of map names
	vector<string> tournamentPlayers; // vector of player strategies, used to initialize players
	int tournamentNumOfGames;
	int tournamentMaxturns;
	vector<Player*> tournamentPlayersWon; // push_back winning player after each game
	
	

	vector<Territory*> mapTerritories;

	string get_str_between_two_str(const string& s, const string& start_delim, const string& stop_delim);
	string tournamentLog();
	bool loadAnotherMap(string file);
	void reset();
	
	void playTournamentOld();
	void PlayTournament(Command* command);

	void initTournamentParams();
	void initTournament(); // @Bero do this part
};