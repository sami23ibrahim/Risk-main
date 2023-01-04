#include "GameEngine.h"
#include <sstream>


using namespace std;

GameEngine::GameEngine()
{
    state = State::null;
    this->map = new Map();
    vector<Player*> players;
    this->deck = new Deck();
    this->numberOfPlayers = 0;
    this->NumberOfTerritories = 0;

    CommandProcessor* commandProcessor = new CommandProcessor();
    processor = new CommandProcessor();
    processor->gameEnginePtr = this;
}

// Creates a deep copy of this GameEngine, and all of its components.
GameEngine::GameEngine(GameEngine& engine)
{
    state = State::null;
    this->map = new Map(*engine.map);
    // this->players;
    for (int i = 0; i < engine.players.size(); i++)
    {
        this->players.push_back(new Player(*engine.players.at(i)));
    }

    this->deck = new Deck(*engine.deck);
    this->numberOfPlayers = engine.numberOfPlayers;
    this->NumberOfTerritories = engine.NumberOfTerritories;
    this->state = engine.state;
    this->processor = engine.processor;
    processor->gameEnginePtr = this;
}

GameEngine::~GameEngine()
{

    delete this->map;
    map = NULL;
    delete this->deck;
    deck = NULL;
    

    delete this->processor->gameEnginePtr;
    processor->gameEnginePtr = NULL;

    delete this->processor;
    processor = NULL;

    for (int i = 0; i < this->players.size(); i++)
    {
        delete this->players[i];
        this->players.at(i) = nullptr;

    }
    this->players.clear();
}

State GameEngine::GetState()
{
    return state;
}

void GameEngine::SetState(State state)
{
    this->state = state;
}

int GameEngine::GetNumberOfPlayers()
{
    return numberOfPlayers;
}

void GameEngine::SetNumberOfPlayers(int nop)
{
    this->numberOfPlayers = nop;
}

// Returns all the players in the game in their order of play.
std::vector<Player*> GameEngine::GetPlayers() 
{ return players; }

std::vector<Player*>* GameEngine::GetPlayersAdress() 
{ return &players; }

// Returns a pointer to the game map.
Map* GameEngine::GetMap() 
{ return map; }

// Stream insertion operator
ostream& operator<<(ostream& os, const GameEngine& gameEngine) {
    string p = "";
    for (Player* i : gameEngine.players) {
        p += (*i).GetPlayerName() + " | ";
    }
  

    return os <<
        "GAME ENGINE: Current state: " << gameEngine.state <<
        "\n    Players (" << gameEngine.numberOfPlayers << "): " << endl << p.substr(0, p.length() - 2) << endl<<
        "\n    Number of Territories : (" << gameEngine.NumberOfTerritories << ")" << endl;
}


// Get String between two delimiter String
// https://stackoverflow.com/questions/18800796/c-get-string-between-two-delimiter-string
string GameEngine::get_str_between_two_str(const string& s,
                                              const string& start_delim,
                                                const string& stop_delim)
{
    unsigned first_delim_pos = s.find(start_delim);
    unsigned end_pos_of_first_delim = first_delim_pos + start_delim.length();
    unsigned last_delim_pos = s.find(stop_delim);

    return s.substr(end_pos_of_first_delim,
        last_delim_pos - end_pos_of_first_delim);
}


string GameEngine::SelectName(string command) {
    istringstream iss(command);
    string garbage = "";
    string name = "";
    iss >> garbage >> name;
    return name;
}

//=================== StartUp Phase===================

void GameEngine::StartupPhase() {
    cout << "Welcome to Warzone" << endl << endl;

    Start();

    //Q This sould be implemented throug a command 
    // "During the start game state, a new tournament command can be entered by the user, 
    // which triggers the Tournament Mode"

   string input = "";
   std::cout << "Do you want use the gamelog file to play the tournament? Enter (y/n): " << endl;
   do {
       std::cin >> input;
       if (input == "n") break;
   }  while (input != "y");
   if (input == "y") {
       processor->FileTournamentFunctionInput("tournamentLog.txt");
       tournamentMode = true;
       PlayTournament(nullptr);
   } else TakeInput(); 
}

void GameEngine::TakeInput() {
    do {
      
        commandEntered = processor->getCommand();
        
        if (processor->validate(commandEntered)) {
        
            if (commandEntered->type == Command::CommandType::tournament) {
                
                // tournament -M <listofmaps> -P <listofplayerstrategies> -G <numberofgames> -D <maxnumberofturns>\" to enter the parameters
                // tournament -M bigeurope sw_baltic bigeurope2 sw_baltic2 map5 -P Aggressive Benevolent Neutral Cheater -G 3 -D 50
                // tournament -M bigeurope bigeurope -P Aggressive Benevolent Neutral Cheater -G 3 -D 50
        
                tournamentMode = true;
                PlayTournament(commandEntered);
        
            }
            else if (commandEntered->type == Command::CommandType::loadmap) {
        
                LoadMap(commandEntered);
        
            }

            else if (commandEntered->type == Command::CommandType::validatemap) {

                ValidateMap();
            }

            else if (commandEntered->type == Command::CommandType::addplayer) {

                if (numberOfPlayers < 6) {

                    AddPlayer();

                }

                else {

                    cout << endl << "-< Maximum number of players is 6 >- " << endl;

                    cout << endl << "::Use the \"gamestart\" command to start the game play phase" << endl;
                }
               
            }
                   
            else if (commandEntered->type == Command::CommandType::gamestart) {

                for (auto player : players) {
                     player->setStrategy("Human", deck, players);
                }

                if (numberOfPlayers > 1) {

                    GameStart();
                    Player* winner = MainGameLoop();
                    cout << winner->GetPlayerName() << " has won (gg ez)" << endl;
                }

                else {

                    cout << endl << "::Use the \"addplayer <playername>\" command to enter players in the game (2-6 players)" << endl;

                }
                
            }

        }
    } while ((state != State::tournamentEnd && tournamentEnd) || (state != State::win && !tournamentMode) );
}


void GameEngine::Start() {
    state = State::start;   
    cout << "GamePhase: start" << endl << endl;
    Notify(this);

    // show a list of available maps
    std::cout << "Available maps:"
              << "\n---------------------------\n"
              << "bigeurope" << endl
              << "bigeurope2 (invalid)" << endl
              << "invalid (lol)" << endl
              << "sw_baltic"
              << "\n---------------------------\n"
              << endl; 
}


void GameEngine::LoadMap(Command* commandEntered) {

    string mapName;
    string fileName;

        mapName = SelectName(commandEntered->GetCommandName());

        fileName = "source_maps/" + mapName + ".map";

        map = new Map(fileName);

        if (map->isMapLoaded()) {
            state = State::maploaded;
            cout << endl << "GamePhase: map loaded" << endl << endl;
            Notify(this);
        }
}

void GameEngine::LoadMap() {
 
    string mapName;
    string fileName;

    do {
        cout << "::Use the \"loadmap <filename>\" command to select a map" << endl << endl;

        commandEntered = processor->getCommand();
            
        if (processor->validate(commandEntered)) {

            mapName = SelectName(commandEntered->GetCommandName());

            fileName = "source_maps/" + mapName + ".map";

            map = new Map(fileName);

            if (map->isMapLoaded()) {
                state = State::maploaded;
                cout << endl << "GamePhase: map loaded" << endl << endl;
                Notify(this);
            }
        }
    } while (map == nullptr || !map->isMapLoaded());
}

bool GameEngine::ValidateMapOld() {

    bool validMap = false;

    do {
        cout << "::Use the \"validatemap\" command to validate the map" << endl << endl;

        commandEntered = processor->getCommand();

        if (processor->validate(commandEntered)) {

            validMap = map->validate();

            if (!validMap)
            {
                cout << "Not a valid map, please try again\n";
                return false;
            }
            else
            {
                state = State::mapvalidated;
                cout << "GamePhase: map validated" << endl << endl;
                Notify(this);
                return true;
            }
        }
    } while (!validMap);
    return false;
}


void GameEngine::PlayTournament(Command* command = nullptr) {
    
 //   cout << *command;
 
  //  std::cout << "Use \"tournament -M <listofmaps> -P <listofplayerstrategies> -G <numberofgames> -D <maxnumberofturns>\" to enter the parameters" << std::endl;

    if (command != nullptr) {
        string commandText = command->getCommandText();
        processor->TournamentFunctionInput(commandText);
    }

    if (processor->TournamentValidation()) {
     // input is valid, so set the parameters
    initTournamentParams();
    state = State::tournamentStart;
    //Q why are we changing to tournamentStart state?
    Notify(this);
    for (int i = 0; i < tournamentNumOfGames; ++i) {

        //Q where are you initializing tournament maps?? found it in the initTournamentParams
        for (int j = 0; j < tournamentMaps.size(); ++j) {
            initTournament();
            map->load("source_maps/" + tournamentMaps[j] + ".map");
            // play game
            GameStart(); // is this the correct function? //Q yes

            Player* playerWon = MainGameLoop(); // get player who won //Q is this a place holder?
            tournamentPlayersWon.push_back(playerWon);
            reset();
        }
    }



    state = State::tournamentEnd;
    //Q should we sandwish the tournement with those states
    Notify(this);
    cout << "gg ez" << endl;
    }

    else
    {
        cout << endl << "Tournament validation failed";
    }
}


bool GameEngine::ValidateMap() {
    
    bool validMap = false;

    validMap = map->validate();

    if (!validMap)
    {
        cout << "Not a valid map, please try again\n" << endl << endl;
        return false;
    }
    else
    {
        state = State::mapvalidated;
        cout << "GamePhase: map validated" << endl << endl;
        Notify(this);
        return true;
    }        
}


void GameEngine::AddPlayers() {

    bool startGame = false;

    while (!startGame) {

        cout << "::Use the \"addplayer <playername>\" command to enter players in the game (2-6 players)" << endl;

        if (numberOfPlayers > 1 && numberOfPlayers < 7) {

            cout << "OR" << endl;
            cout << "::Use the \"gamestart\" to start the game play phase" << endl << endl;
        }

        commandEntered = processor->getCommand();

        if (processor->validate(commandEntered)) {

            if (commandEntered->type == Command::CommandType::addplayer) {

                if (numberOfPlayers < 6) {

                    AddPlayer();

                }

                else {

                    cout << endl << "-< Maximum number of players is 6 >- " << endl;

                    cout << endl << "::Use the \"gamestart\" command to start the game play phase" << endl;
                }
            }

            if (commandEntered->type == Command::CommandType::gamestart) {

                startGame = true;
            }
        }
    }

}

void GameEngine::AddPlayer() {

    string playerName;

    playerName = SelectName(commandEntered->GetCommandName());

    Player* player = new Player();

    player->setName(playerName);
    player->setPlayerID(numberOfPlayers);

    players.push_back(player);
    numberOfPlayers++;

    cout << "Player " << numberOfPlayers << ". " << playerName << " -> has been created. " << endl << endl;
    state = State::playersadded;
    cout << "GamePhase: players added" << endl << endl;
    Notify(this);    
}

void GameEngine::AddStrategyPlayer(string strategyName, int playerID) {

    string playerName = strategyName;

  //  playerName = SelectName(commandEntered->GetCommandName());

    Player* player = new Player();

    player->setName(playerName);
    player->setPlayerID(playerID);
    

    players.push_back(player);
    
    cout << "Player " << playerID << ". " << playerName << " -> has been created. " << endl << endl;
    state = State::playersadded;
    cout << "GamePhase: players added" << endl << endl;
    Notify(this);    
}


void GameEngine::DistributeTerritories() {

    mapTerritories = map->GetMapTerritories();
    NumberOfTerritories = map->GetMapTerritoriesNumber();

    int playerIndex = 0;

    for (int i = 0; i < NumberOfTerritories; i++) {

        // Player* tempPlayer = players.at(playerIndex);

        mapTerritories[i]->setOwnerId(players[playerIndex]->GetPlayerID());
        mapTerritories[i]->setPlayerOwner(players[playerIndex]);

        // Territory* tempTerritory = new Territory(*mapTerritories[i]);
        // just add the territory no need to copy

        players[playerIndex]->addTerritory(mapTerritories[i]);


        playerIndex++;

        if (playerIndex >= players.size()) {
            playerIndex = 0;
        }
    }

    for (auto p : players) p->toDefend();
    for (auto p : players) p->toAttack();
}

void GameEngine::ShufflePlayers() {

    // obtain a time-based seed:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);


    shuffle(begin(players), end(players), e);
}

void GameEngine::GameStart() {
    DistributeTerritories();
    ShufflePlayers();
    deck->create_deck();

    for (Player* player : players) {
        for (int i = 0; i < 2; i++) {
            player->AddCard(deck->draw());
        }
        player->addToReinforcePool(50);
    }

    cout << endl << "Players of this game ->" << endl;

    for (auto player : players)
        cout << endl << *player << endl;
 //  GameEngine::MainGameLoop(); // Here we start the main game loop

    // For debugging:
    
    // cout << endl << "Map info ->" << endl << endl <<*map;

    /*  for (Player* player : players) {
        cout << endl <<"Territories owned by " << player->GetPlayerName() << " -> " << endl << endl;
        for (Territory* territory : player->getTerritoryList()) {
          cout << *territory;
        } 
    }*/
}

// ==============================Play Phase===================================
Player* GameEngine::MainGameLoop() {

    cout << "Starting Main Game Loop..." << endl;
    ReinforcementPhase();
    while (true) {
        IssueOrdersPhase();
        ExecuteOrdersPhase();
        // Eliminate players who don't have any more territories.
        for (int i = players.size() - 1; i >= 0; i--) {
            if (tournamentMode) {
                cout << "A player has been eliminated." << endl;
                players.erase(players.begin() + i);
                if (players.size() == 1) {
                    cout << "There's only one player left." << endl;
                    state = State::win;
                    Notify(this);
                    Player* p = new Player(*players[0]);
                    return p;
                }
            }
            if (players[i]->getTerritoryList().size() == 0) {
                cout << "A player has been eliminated." << endl;
                players.erase(players.begin() + i);
                if (players.size() == 1) {
                    cout << "There's only one player left." << endl;
                    state = State::win;
                    Notify(this);
                    Player* p = new Player(*players[0]);
                    return p;
                }
            }
        }
        // If a player controls all territories, they win the game.
    }
}

// Adds troops to a player's reinforcement pool at the start of the turn.
void GameEngine::ReinforcementPhase() {
    state = State::reinforcementPhase;
    Notify(this);

    bool check = false;
    int temp = 0;
    double count = 0;

    cout << "Starting Reinforcement Phase..." << endl;
    for (int i = 0; i < players.size(); i++)
    {   //check if the player's terriotries
        temp = players[i]->getReinforcementPool();

        for (int j = 0; j < players[i]->getTerritoryList().size(); j++)
        {
            //count the terriorties number
            count++;
        }
        //calcuate the contient bouns
        check = (*players[i]).playerContinentBonus();

        if (check == true)
        {
            temp += 2 * (int)round(count / 3);
        }
        else
        {
            temp += (int)round(count / 3);
        }

        temp += 3;

        players[i]->setReinforcementPool(temp);
        check = false;
        temp = 0;
        count = 0;

    }

    cout << endl << "end of Reinforcement phase" << endl << endl;
    // IssueOrdersPhase(); //why????????????????????????????// this was from the first assignment, the logic changed sience
}


// Calls IssueOrder until all players have commited that they are done issuing orders.
void GameEngine::IssueOrdersPhase() {
    state = State::issueOrderPhase;
    Notify(this);

    cout << "Starting Issue Orders Phase..." << endl;


    auto m = map->GetMapTerritories();
    
    for (auto player : players)
    {
        if (tournamentMode) {
            player->issueOrder();
        }
        else player->issueOrder(); // what should be order?
    }
    cout << "end of issue orders phase" << endl << endl;

    // ExecuteOrdersPhase(); // why????
}


// Calls ExecuteOrder() until all players have no more orders in their orders list.
void GameEngine::ExecuteOrdersPhase() {
    state = State::executeOrderPhase;
    Notify(this);


    cout << "Starting Execute Orders Phase..." << endl;

    for (Player* player : players)
    {
        vector<Order*>& orderList = player->getOrdersList()->listOfOrders;
        for (int i = 0; i < orderList.size(); i++)
        {
            //execute and remove if its a deploy order
            if (orderList.at(i)->getName() == "Deploy")
            {
                //validate, excute and delete order
                // orderList.at(i)->validate();
                orderList.at(i)->execute();
                orderList.erase(orderList.begin() + i);
            }
        }
    }

    //there should be no more deploy orders

    for (Player* player : players)
    {
        vector<Order*>& orderList = player->getOrdersList()->listOfOrders;
        while (!orderList.empty())
        {
            //validate, excute and delete order
            // orderList.at(0)->validate();
            orderList.at(0)->execute();
            orderList.erase(orderList.begin());
        }
    } //order list should be empty


    cout << "end of execute orders phase" << endl;
}


void GameEngine::ExecuteOrders() {


    cout << "Enter \"win\" to win,  Enter \"executeOrder\" to repeat executeOrder Enter \"new\"to go back to issue new order" << endl;
    string a;
    cin >> a;
    if (a == "win") {
        state = State::win;
        Notify(this);
    }
    else if (a == "executeOrder") {
        ExecuteOrdersPhase();
        state = State::executeOrderPhase;
        Notify(this);

    }
    else if (a == "new") {
        state = State::issueOrderPhase;
        Notify(this);
        IssueOrdersPhase();
    }
}


void GameEngine::PlayerDrawCard(Player* player) {

}



std::string GameEngine::stringToLog() {
    std::string log = "";
    switch (state) {
        case State::start:
            log = "State: Starting game";
            break;
        case State::maploaded:
            log = "Phase: Map loaded";
            break;
        case State::mapvalidated:
            log = "Phase: Map validated";
            break;
        case State::playersadded:
            log = "Phase: Players added";
            break;
        case State::reinforcementPhase:
            log = "Phase: Reinforcement";
            break;
        case State::issueOrderPhase:
            log = "Phase: Issue order";
            break;
        case State::executeOrderPhase:
            log = "Phase: Execute order";
            break;
        case State::tournamentStart:
            log = "Phase: Starting tournament";
            break;
        case State::tournamentEnd:
            log = tournamentLog();
            break;
        default:
            log = "Unknown phase";
            break;
        
    }
    return log;
}

void GameEngine::AttachToOrdersList(LogObserver* observer) {
    for (Player* p : players) {
        p->Attach(observer);
    }
}

void GameEngine::AttachToProcessor(LogObserver* observer) {
    processor->Attach(observer);
    for (auto i : processor->commandObjects) {
        i->Attach(observer);
    }
}


string GameEngine::tournamentLog() {
    ostringstream oss;
    // logging tournament parameters
    oss << "Tournament mode:" << endl;
    oss << "M: ";
    for (auto map : tournamentMaps) oss << map << " ";
    oss << endl;
    oss << "P: ";
    // TODO: replace player->GetPlayerName() by getter for strategy name
    for (auto player : tournamentPlayers) oss << player << " ";
    oss << endl;
    oss << "G: " << tournamentNumOfGames << endl;
    oss << "D: " << tournamentMaxturns << endl;
    // logging results
    oss << "Results: " << endl;
    for (int i = 0; i < tournamentNumOfGames; ++i) {
        oss << "Game " << i + 1 << endl;
        for (int j = 0; j < tournamentMaps.size(); ++j) {
            auto player = tournamentPlayersWon[i * tournamentMaps.size() + j];
            oss << "\t" << "Map: " << tournamentMaps.at(j) << ", Won by " << player->GetPlayerName() << endl;
        }
    }
    return oss.str();

}
// returns true if the load was successfull, otherwise false
bool GameEngine::loadAnotherMap(string file) {
    map->load(file);
    return map->validate();
}

void GameEngine::reset() {
    // NOTE: All of these should be reloaded on each game in the tournament
    //Q Should not we call the destructors?
    for (auto p : players) delete p;
    players.clear();
    map->clear();

    mapTerritories.clear();
    state = State::start;
   
    delete deck;
    deck = new Deck();
    NumberOfTerritories = 0;
    numberOfPlayers = 0;
    cout << "Resetted state" << endl;
    // we should use delete to call the destructures 
    // would not calling delete gameEngine take care of some of the cleaning .. check ~GameEngine
}

void GameEngine::playTournamentOld() {
    
    while (true) {
        std::cout << "Use \"tournament -M <listofmaps> -P <listofplayerstrategies> -G <numberofgames> -D <maxnumberofturns>\" to enter the parameters" << std::endl;
        string input = "";
        std::cin >> input;
        processor->TournamentFunctionInput(input);
        if (processor->TournamentValidation()) break;
    } 

    // input is valid, so set the parameters
    initTournamentParams();
    state = State::tournamentStart;
    //Q why are we changing to tournamentStart state?
    Notify(this);
    for (int i = 0; i < tournamentNumOfGames; ++i) {
      
        for (int j = 0; j < tournamentMaps.size(); ++i) {
            reset();
           
            // initTournament();

            int playerID = 1;
            for (string& player : tournamentPlayers) // access by reference to avoid copying
            {
                AddStrategyPlayer(player, playerID);
                playerID++;
            }
            for (int i = 0; i < players.size(); ++i)
            {
                players[i]->setStrategy(tournamentPlayers[i], deck, players);
            }

            map->load("source_maps/" + tournamentMaps[i] + ".map");
           
            // play game
            GameStart(); // is this the correct function? //Q yes


            Player* playerWon = MainGameLoop(); // get player who won //Q is this a place holder?

            tournamentPlayersWon.push_back(playerWon);
        }
    }



  
    state = State::tournamentEnd;
    //Q should we sandwish the tournement with those states
    Notify(this);
}

void GameEngine::initTournamentParams() {
    tournamentMaps = processor->allMaps;
    tournamentPlayers = processor->allPlayerStrategies;
    tournamentNumOfGames = processor->numberOfGames;
    tournamentMaxturns = processor->maxNumberOfTurns;
}

void GameEngine::initTournament() {
    int playerID = 1;
    for (string& player : tournamentPlayers) // access by reference to avoid copying
    {
        AddStrategyPlayer(player, playerID);
        playerID++;
    }
    for (int i = 0; i < players.size(); ++i) {
        players[i]->setStrategy(tournamentPlayers[i], deck, players);
    }
    cout << "Tournament initialized" << endl;
}