#include "Orders.h"
//--------------------------ORDER---------------------------
//Default constructor
Order::Order() {
	this->name = "Order";
	this->player = NULL;
}

//Destructor
Order:: ~Order() {

}

//Copy constructor
Order::Order(Order& anotherOrder) {
	name = anotherOrder.name;
}

//Assignment operator
Order& Order::operator=(const Order& anOrder) {
	name = anOrder.name;
	return *this;
}


istream& operator>>(istream& ins, Order& order) {
	cout << "Enter order's name: ";
	ins >> order.name;
	return ins;
}

ostream& operator<<(ostream& outs, const Order& order) {
	outs << "Order's name: " << order.name << endl;
	return outs;
}

string const Order::getName() {
	return name;
}

void Order::setName(string orderName) {
	name = orderName;
}

void Order::addPlayer(Player* p)
{
	listOfPlayers.push_back(p);
}

vector<Player*> Order::getListOfPlayers()
{
	return listOfPlayers;
}

string Order::stringToLog() {
	return "Order " + name + " executed";
}

//Remove validate and execute from Order (pure virtual)

//--------------------------DEPLOY---------------------------
//Default constructor
Deploy::Deploy() {
	name = "Deploy";
	player = NULL;
	armies = 0;
	target = NULL;
}

//A deploy order tells a certain number of armies taken from the reinforcement pool to deploy to a target territory owned 
//by the player issuing this order.
Deploy::Deploy(Player* currentPlayer, int numberOfArmies, Territory* targetTerritory) {
	this->name = "Deploy";
	this->player = currentPlayer;
	this->armies = numberOfArmies;
	this->target = targetTerritory;
}

//Destructor
Deploy::~Deploy() {
	delete target;
	target = nullptr;
}

//Copy constructor
Deploy::Deploy(Deploy& anotherDeploy) : Order(anotherDeploy) {
	this->name = anotherDeploy.name;
	this->player = anotherDeploy.player;
	this->armies = anotherDeploy.armies;
	this->target = anotherDeploy.target;
}

//Assignment operator
Deploy& Deploy::operator=(const Deploy& aDeploy) {
	Order::operator=(aDeploy);
	this->name = aDeploy.name;
	this->player = aDeploy.player;
	this->armies = aDeploy.armies;
	this->target = aDeploy.target;
	return *this;
}

ostream& operator<<(ostream& outs, const Deploy& deploy) {
	outs << "Order's name: " << deploy.name << endl;
	outs << "Player: " << *(deploy.player) << endl;
	outs << "Number of armies: " << deploy.armies << endl;
	outs << "Target territory: " << *(deploy.target) << endl;
	return outs;
}

string const Deploy::getName() {
	return name;
}

void Deploy::setSelfPlayers(Player* p)
{
	this->player = p;
}

void Deploy::setTargetTerritory(Territory* t)
{
	this->target = t;
}

void Deploy::setNumArmy(int n)
{
	this->armies = n;
}

vector<Player*> Deploy::updateListOfPlayers()
{
	return listOfPlayers;
}

//If the target territory does not belong to the player that issued the order, the order is invalid.
bool Deploy::validate() {
	bool isValid = false;
	cout << target << endl;
	cout << player << endl;
	if (target->getPlayerOwner()->GetPlayerID() == player->GetPlayerID()) {
		cout << "Deploy order is valid." << endl;
		isValid = true;
	}
	else {
		cout << "Deploy order is invalid" << endl;
	}
	return isValid;
}

//The selected number of armies is added to the number of armies on that territory.
void Deploy::execute() {
	bool isValid = validate();
	if (isValid) {
		cout << "Executing deploy..." << endl;
		target->addArmies(armies);
		cout << "Deploying " << armies << " units to territory " << target->getName() << endl;
		player->removeFromReinforcePool(armies);
		cout << "Remaining reinforcement units: " << player->getReinforcementPool() << endl;
	}
	else {
		cout << "Cannot execute deploy." << endl;
	}
	//player->getOrdersList()->remove(this);
}

string Deploy::stringToLog()
{
	return "Order: " + player->GetPlayerName() + " deployed " + to_string(armies) + " units to " + target->getName();
}

//--------------------------ADVANCE---------------------------
//Default constructor
Advance::Advance() {
	name = "Advance";
	player = NULL;
	armies = 0;
	source = NULL;
	target = NULL;
}

// An advance order tells a certain number of army units to move from a source territory to a target adjacent territory.
Advance::Advance(Player* currentPlayer, int numberOfArmies, Territory* sourceTerritory, Territory* targetTerritory, Deck* pDeck) {
	this->name = "Advance";
	this->player = currentPlayer;
	this->armies = numberOfArmies;
	this->source = sourceTerritory;
	this->target = targetTerritory;
	//this->playerHand = pHand;
	this->deck = pDeck;
}

//Destructor
Advance::~Advance() {
	delete source;
	source = nullptr;
	delete target;
	target = nullptr;
}

//Copy constructor
Advance::Advance(Advance& anotherAdvance) : Order(anotherAdvance) {
	this->name = anotherAdvance.name;
	this->player = anotherAdvance.player;
	this->armies = anotherAdvance.armies;
	this->source = anotherAdvance.source;
	this->target = anotherAdvance.target;
	//this->playerHand = anotherAdvance.playerHand;
	this->deck = anotherAdvance.deck;
}

//Assignment operator
Advance& Advance::operator=(const Advance& anAdvance) {
	Order::operator= (anAdvance);
	this->name = anAdvance.name;
	this->player = anAdvance.player;
	this->armies = anAdvance.armies;
	this->source = anAdvance.source;
	this->target = anAdvance.target;
	//this->playerHand = anAdvance.playerHand;
	this->deck = anAdvance.deck;
	return *this;
}

ostream& operator<<(ostream& outs, const Advance& advance) {
	outs << "Order's name: " << advance.name << endl;
	outs << "Player: " << advance.player << endl;
	outs << "Number of armies:" << advance.armies << endl;
	outs << "Source territory: " << advance.source << endl;
	outs << "Target territory: " << advance.target << endl;
	return outs;
}

string const Advance::getName() {
	return name;
}

void Advance::setSelfPlayers(Player* p)
{
	this->player = p;
}

void Advance::setSourceTerritory(Territory* s)
{
	this->source = s;
}

void Advance::setTargetTerritory(Territory* t)
{
	this->target = t;
}

void Advance::setArmyUnits(int n)
{
	this->armies = n;
}

vector<Player*> Advance::updateListOfPlayers()
{
	return listOfPlayers;
}

//If the source territory does not belong to the player that issued the order, the order is invalid.
//If the target territory is not adjacent to the source territory, the order is invalid.
bool Advance::validate() {
	bool isValid = false;
	if (source->getPlayerOwner()->GetPlayerID() == player->GetPlayerID() && target->isAdjacentTo(source->getId()) && !player->isNegociating(this->target->getPlayerOwner())) {
		cout << "Advance order is valid" << endl;
		isValid = true;
	}
	else {
		if (player->isNegociating(this->target->getPlayerOwner()))
			cout << "Players " << player->GetPlayerName() << " and " << target->getPlayerOwner()->GetPlayerName() << " are negociating. Advance order invalid." << endl;
		cout << "Advance order is invalid" << endl;
	}
	return isValid;
}



void Advance::execute() {
	bool isValid = validate();
	if (isValid) {
		cout << "Executing advance..." << endl;
		//If the source and target territory both belong to the player that issued the order, the army units are moved from the source to the target territory.
		if (source->getPlayerOwner() == player && target->getPlayerOwner() == player) {
			cout << "The target territory belongs to player " << player->GetPlayerName() << endl;
			source->removeArmies(armies);
			cout << "Amount of units remaining in source territory " << source->getName() << " is " << source->getArmies() << endl;
			target->addArmies(armies);
			cout << "Amount of units in target territory " << target->getName() << " is " << target->getArmies() << endl;
		}
		//If the target territory belongs to another player than the player that issued the advance order, an attack is
		//simulated when the order is executed.An attack is simulated by the following battle simulation mechanism :
		else if (target->getPlayerOwner() != player) {

			bool noMoreArmies = false;
			cout << "source armies " << armies << endl;
			cout << "Target armies " << target->getArmies() << endl;
			// Each attacking army unit involved has 60 % chances of killing one defending army.
			while (!noMoreArmies) {
				int attChances = rand() % 100; // Generate random number between 0 & 99
				int defChances = rand() % 100; // Generate random number between 0 & 99

				if (attChances >= 0 && attChances <= 59) {
					target->removeArmies(1);
					cout << "Player " << player->GetPlayerName() << " has eliminated 1 unit of player " << target->getPlayerOwner()->GetPlayerName() << endl;
				}
				// At the same time, each defending army unit has 70 % chances of killing one attacking army unit.
				if (defChances >= 0 && defChances <= 69) {
					armies -= 1;
					source->removeArmies(1);
					cout << "Player " << target->getPlayerOwner()->GetPlayerName() << " has eliminated 1 unit of player " << player->GetPlayerName() << endl;
				}
				if (armies == 0 || target->getArmies() == 0) {
					noMoreArmies = true;
				}

			}

			// If all the defender's armies are eliminated, the attacker captures the territory. 
			// The attacking army units that survived the battle then occupy the conquered territory.
			if (target->getArmies() == 0) {
				cout << "Player " << target->getPlayerOwner()->GetPlayerName() << " has lost territory " << target->getName() << endl;
				target->setPlayerOwner(source->getPlayerOwner());
				cout << "The new owner of territory " << target->getName() << " is " << target->getPlayerOwner()->GetPlayerName() << endl;
				target->addArmies(armies);
				cout << "The number of units in territory " << target->getName() << " is " << target->getArmies() << endl;
				Cards* drawnCard = deck->draw();
				player->getPlayerHand()->setHand(drawnCard); // Player receives card if they succesfully conquered at least one territory during their turn
				cout << "Since player " << target->getPlayerOwner()->GetPlayerName() << " has successfully captured the territory, they have received the card " << *drawnCard->getCardType() << endl;
			}
			else if (armies == 0) {
				cout << "Player " << player->GetPlayerName() << " has not been able to capture territory " << target->getName() << " and there remains " << target->getArmies() << " unit at that territory." << endl;
			}
		}
	}
	else {
		cout << "Cannot execute advance." << endl;
	}
}

string Advance::stringToLog()
{
	return "Order: " + player->GetPlayerName() + " advanced " + to_string(armies) + " from " + source->getName() + " to " + target->getName();
}

//--------------------------BOMB---------------------------
//Default constructor
Bomb::Bomb() {
	name = "Bomb";
	player = NULL;
	target = NULL;
}

//A bomb order targets a territory owned by another player than the one issuing the order. Its result is
//to remove half of the armies from this territory.The bomb order can only be created by playing the bomb card.
Bomb::Bomb(Player* currentPlayer, Territory* targetTerritory) {
	this->name = "Bomb";
	this->player = currentPlayer;
	this->target = targetTerritory;
}

//Destructor
Bomb::~Bomb() {
	delete target;
	target = nullptr;
}

//Copy constructor
Bomb::Bomb(Bomb& anotherBomb) : Order(anotherBomb) {
	this->name = anotherBomb.name;
	this->player = anotherBomb.player;
	this->target = anotherBomb.target;
}

//Assignment operator
Bomb& Bomb::operator=(const Bomb& aBomb) {
	Order::operator= (aBomb);
	this->name = aBomb.name;
	this->player = aBomb.player;
	this->target = aBomb.target;
	return *this;
}

ostream& operator<<(ostream& outs, const Bomb& bomb) {
	outs << "Order's name: " << bomb.name << endl;
	outs << "Player: " << *(bomb.player) << endl;
	outs << "Target territory: " << *(bomb.target) << endl;
	return outs;
}

string const Bomb::getName() {
	return name;
}

void Bomb::setSelfPlayers(Player* p)
{
	this->player = p;
}

void Bomb::setTargetTerritory(Territory* t)
{
	this->target = t;
}

vector<Player*> Bomb::updateListOfPlayers()
{
	return listOfPlayers;
}

//If the target belongs to the player that issued the order, the order is invalid.
//If the target territory is not adjacent to one of the territory owned by the player issuing the order, then the order is invalid.
bool Bomb::validate() {
	bool isValid = false;
	bool isAdjacent = false;
	for (int i = 0; i < player->getTerritoryList().size(); i++) {
		if (player->getTerritoryList().at(i)->isAdjacentTo(target->getId())) {
			isAdjacent = true;
		}
	}
	if (target->getPlayerOwner() != player && isAdjacent && !player->isNegociating(this->target->getPlayerOwner())) {
		cout << "Bomb order is valid" << endl;
		isValid = true;
	}
	else {
		if (player->isNegociating(this->target->getPlayerOwner()))
			cout << "Players " << player->GetPlayerName() << " and " << target->getPlayerOwner()->GetPlayerName() << " are negociating. Bomb order invalid." << endl;
		cout << "Bomb order is invalid" << endl;
	}
	return isValid;
}

//If the target belongs to an enemy player, half of the armies are removed from this territory.
void Bomb::execute() {
	bool isValid = validate();
	if (isValid) {
		cout << "Executing bomb..." << endl;
		int enemyArmies = target->getArmies();
		if (enemyArmies > 1) {
			target->removeArmies(enemyArmies / 2);
		}
		else {
			target->removeArmies(enemyArmies);
		}
		cout << "There remains " << target->getArmies() << " units at target territory " << target->getName() << endl;
	}
	else {
		cout << "Cannot execute bomb." << endl;
	}
}

string Bomb::stringToLog()
{
	return "Order: " + player->GetPlayerName() + " bombed " + target->getName();
}

//--------------------------BLOCKADE---------------------------
//Default constructor
Blockade::Blockade() {
	name = "Blockade";
	player = NULL;
	target = NULL;
}

//A blockade order targets a territory that belongs to the player issuing the order. Its effect is to
//double the number of armies on the territory and to transfer the ownership of the territory to the Neutral player.
//The blockade order can only be created by playing the blockade card.
Blockade::Blockade(Player* currentPlayer, vector<Player*> all, Territory* targetTerritory) {
	this->name = "Blockade";
	this->player = currentPlayer;
	this->listOfPlayers = all;
	this->target = targetTerritory;
	/*this->listOfPlayers = list;*/
	//this->engine = e;
}

//Destructor
Blockade::~Blockade() {
	delete target;
	target = nullptr;
	delete neutralPlayer;
	neutralPlayer = nullptr;
	/*allPlayers->getListOfPlayers().clear();*/
}

//Copy constructor
Blockade::Blockade(Blockade& anotherBlockade) : Order(anotherBlockade) {
	this->name = anotherBlockade.name;
	this->player = anotherBlockade.player;
	//this->allPlayers = anotherBlockade.allPlayers;
	this->target = anotherBlockade.target;
	this->listOfPlayers = anotherBlockade.listOfPlayers;
	//this->engine = anotherBlockade.engine;
}

//Assignment operator
Blockade& Blockade::operator=(const Blockade& aBlockade) {
	Order::operator= (aBlockade);
	this->name = aBlockade.name;
	this->player = aBlockade.player;
	//this->allPlayers = aBlockade.allPlayers;
	this->target = aBlockade.target;
	this->listOfPlayers = aBlockade.listOfPlayers;
	//this->engine = aBlockade.engine;
	return *this;
}

ostream& operator<<(ostream& outs, const Blockade& blockade) {
	outs << "Order's name: " << blockade.name << endl;
	outs << "Player: " << *(blockade.player) << endl;
	outs << "Target territory: " << blockade.target << endl;
	return outs;
}

string const Blockade::getName() {
	return name;
}

void Blockade::setSelfPlayers(Player* p)
{
	this->player = p;
}

void Blockade::setNeutralPlayer(Player* n)
{
	this->neutralPlayer = n;
}

void Blockade::setTargetTerritory(Territory* t)
{
	this->target = t;
}

vector<Player*> Blockade::updateListOfPlayers()
{
	return listOfPlayers;
}

//If the target territory belongs to an enemy player, the order is declared invalid.
bool Blockade::validate() {
	bool isValid = false;
	if (target->getPlayerOwner() == player && !player->isNegociating(this->target->getPlayerOwner())) {
		cout << "Blockade order is valid" << endl;
		isValid = true;
	}
	else {
		if (player->isNegociating(this->target->getPlayerOwner()))
			cout << "Players " << player->GetPlayerName() << " and " << target->getPlayerOwner()->GetPlayerName() << " are negociating. Blockade order invalid." << endl;
		cout << "Blockade order is invalid" << endl;
	}
	return isValid;
}

//If the target territory belongs to the player issuing the order, the number of armies on the territory is doubled 
//And the ownership of the territory is transferred to the Neutral player, which must be created if it does not already exist.
void Blockade::execute() {
	bool isValid = validate();
	if (isValid) {
		cout << "Executing blockade..." << endl;
		target->addArmies(target->getArmies());
		cout << "Number of armies added to target: " << target->getArmies() << endl;

		for (int i = 0; i < listOfPlayers.size(); i++) {
			if (listOfPlayers.at(i)->GetPlayerName() == "Neutral Player") { // Verify if player in game engine list of players has a neutral player
				neutralPlayer = listOfPlayers.at(i);
				neutralPlayer->addTerritory(target);
				listOfPlayers.at(i)->getTerritoryList().push_back(target); // Transfer target ownership

				cout << "The target territory " << target->getName() << "'s ownership has been transfered to " << listOfPlayers.at(i)->GetPlayerName() << endl;

				cout << "Neutral Player's territories are: " << endl;
				for (int j = 0; j < neutralPlayer->getTerritoryList().size(); j++) {
					cout << neutralPlayer->getTerritoryList().at(j)->getName() << endl;
				}
				/*for (int j = 0; j < allPlayers->getListOfPlayers().at(i)->getTerritoryList().size(); j++) {
					cout << allPlayers->getListOfPlayers().at(j)->getTerritoryList().at(j)->getName() << endl;
				}*/
				break;
			}
			// if reach end of for loop and have not found a neutral player, then create a neutral player
			else if (listOfPlayers.at(listOfPlayers.size() - 1)->GetPlayerName() != "Neutral Player") {

				// Create variables for neutral player
				int* neutralID = new int(0);
				int* neutralReinforcement = new int(0);
				string* neutralName = new string("Neutral Player");
				vector<Territory*> neutralTerritoryList;
				Hand* neutralHand = new Hand();
				OrdersList* neutralOrdersList = new OrdersList();

				neutralTerritoryList.push_back(target);
				// Create neutral player
				neutralPlayer = new Player(neutralID, neutralReinforcement, neutralName, neutralTerritoryList, neutralHand, neutralOrdersList);
				listOfPlayers.push_back(neutralPlayer); // Add neutral player to list of players in game engine
				target->setPlayerOwner(neutralPlayer);
				cout << "A neutral player has been created." << endl;
				cout << "The target territory " << target->getName() << "'s ownership has been transferred to " << neutralPlayer->GetPlayerName() << endl;
				cout << "Neutral Player's territories are: " << endl;
				for (int j = 0; j < neutralPlayer->getTerritoryList().size(); j++) {
					cout << neutralPlayer->getTerritoryList().at(j)->getName() << " army count " << neutralPlayer->getTerritoryList().at(i)->getArmies() << endl;
				}

				// Remove territory from players territory list
				for (int i = 0; i < player->getTerritoryList().size(); i++) {
					if (player->getTerritoryList().at(i) == target) { // Remove territory that has been transferred to neutral player from player issuing order
						player->removeTerritory(i);
						cout << "Player " << player->GetPlayerName() << " no longer has ownership of territory " << target->getName() << endl;
						cout << "Player " << player->GetPlayerName() << "'s territories are: " << endl;
						for (int j = 0; j < player->getTerritoryList().size(); j++) {
							cout << player->getTerritoryList().at(j)->getName() << endl;
							if (player->getTerritoryList().size() == 0) {
								cout << "Player " << player->GetPlayerName() << " has no territories left." << endl;
							}
						}
						if (player->getTerritoryList().size() == 0) {
							cout << "Player " << player->GetPlayerName() << " has no territories left." << endl;
						}
						//break;
					}
				}
				//for (int j = 0; j < allPlayers->getListOfPlayers().at(allPlayers->getListOfPlayers().size()-1)->getTerritoryList().size(); j++) {
				//    cout << allPlayers->getListOfPlayers().at(j)->getTerritoryList().at(j)->getName() << endl;
				//}

				//cout << allPlayers->getListOfPlayers().size() << endl;
				break;
			}
		}
	}
	else {
		cout << "Cannot execute blockade." << endl;
	}
}

string Blockade::stringToLog()
{
	return "Order: " + player->GetPlayerName() + " blocked " + target->getName();
}

//--------------------------AIRLIFT---------------------------
//Default constructor
Airlift::Airlift() {
	name = "Airlift";
	player = NULL;
	armies = 0;
	source = NULL;
	target = NULL;
}

//An airlift order tells a certain number of armies taken from a source territory to be moved to a target
//territory, the source and the target territory being owned by the player issuing the order.The airlift order can only
//be created by playing the airlift card
Airlift::Airlift(Player* currentPlayer, int armies, Territory* sourceTerritory, Territory* targetTerritory) {
	this->name = "Airlift";
	this->player = currentPlayer;
	this->armies = armies; // Added this & int armies in ()
	this->source = sourceTerritory;
	this->target = targetTerritory;
}

//Destructor
Airlift::~Airlift() {
	/*delete source;
	source = nullptr;
	delete target;
	target = nullptr;*/
}

//Copy constructor
Airlift::Airlift(Airlift& anotherAirlift) : Order(anotherAirlift) {
	this->name = anotherAirlift.name;
	this->player = anotherAirlift.player;
	this->armies = anotherAirlift.armies;
	this->source = anotherAirlift.source;
	this->target = anotherAirlift.target;
}

//Assignment operator
Airlift& Airlift::operator=(const Airlift& anAirlift) {
	Order::operator= (anAirlift);
	this->name = anAirlift.name;
	this->player = anAirlift.player;
	this->source = anAirlift.source;
	this->target = anAirlift.target;
	return *this;
}

ostream& operator<<(ostream& outs, const Airlift& airlift) {
	outs << "Order's name: " << airlift.name << endl;
	outs << "Player: " << airlift.player << endl;
	outs << "Source territory: " << airlift.source << endl;
	outs << "Target territory: " << airlift.target << endl;
	return outs;
}

string const Airlift::getName() {
	return name;
}

void Airlift::setSelfPlayers(Player* p)
{
	this->player = p;
}

void Airlift::setSourceTerritory(Territory* s)
{
	this->source = s;
}

void Airlift::setTargetTerritory(Territory* t)
{
	this->target = t;
}

void Airlift::setNumArmy(int n)
{
	this->armies = n;
}

vector<Player*> Airlift::updateListOfPlayers()
{
	return listOfPlayers;
}

//The target territory does not need to be adjacent to the source territory.
//If the source or target does not belong to the player that issued the order, the order is invalid.
bool Airlift::validate() {
	bool isValid = false;
	if (source->getPlayerOwner()->GetPlayerID() == player->GetPlayerID() && target->getPlayerOwner()->GetPlayerID() == player->GetPlayerID()) {
		cout << "Airlift order is valid" << endl;
		isValid = true;
	}
	else {
		cout << "Airlift order is invalid" << endl;
	}

	return isValid;
}

//If both the source and target territories belong to the player that issue the airlift order, then the selected
//number of armies is moved from the source to the target territory.
void Airlift::execute() {
	bool isValid = validate();
	if (isValid) {
		cout << "Executing airlift..." << endl;
		cout << source->getArmies() << endl;
		source->removeArmies(armies);
		cout << "Source territory " << source->getName() << " has " << to_string(source->getArmies()) << " units left." << endl;
		target->addArmies(armies);
		cout << "Target territory " << target->getName() << " has " << armies << " units added. The total number of units is " << to_string(target->getArmies()) << endl;
	}
	else {
		cout << "Cannot execute airlift." << endl;
	}
}

string Airlift::stringToLog()
{
	return "Order: " + player->GetPlayerName() + " airlifted " + to_string(armies) + " from " + source->getName() + " to " + target->getName();
}

//--------------------------NEGOTIATE---------------------------
//Default constructor
Negotiate::Negotiate() {
	name = "Negotiate";
	player = NULL;
	enemyPlayer = NULL;
}

//A negotiate order targets an enemy player. It results in the target player and the player issuing
//the order to not be able to successfully attack each others� territories for the remainder of the turn.The negotiate
//order can only be created by playing the diplomacy card.
Negotiate::Negotiate(Player* currentPlayer, Player* enemyPlayer) {
	this->name = "Negotiate";
	this->player = currentPlayer;
	this->enemyPlayer = enemyPlayer;
}

//Destructor
Negotiate::~Negotiate() {

}

//Copy constructor
Negotiate::Negotiate(Negotiate& anotherNegotiate) : Order(anotherNegotiate) {
	this->name = anotherNegotiate.name;
	this->player = anotherNegotiate.player;
	this->enemyPlayer = anotherNegotiate.enemyPlayer;
}

//Assignment operator
Negotiate& Negotiate::operator=(const Negotiate& aNegotiate) {
	this->name = aNegotiate.name;
	this->player = aNegotiate.player;
	this->enemyPlayer = aNegotiate.enemyPlayer;
	return *this;
}

ostream& operator<<(ostream& outs, const Negotiate& negotiate) {
	outs << "Order's name: " << negotiate.name << endl;
	outs << "Player: " << *(negotiate.player) << endl;
	outs << "Enemy player: " << *(negotiate.enemyPlayer) << endl;
	return outs;
}

string const Negotiate::getName() {
	return name;
}

void Negotiate::setSelfPlayers(Player* p)
{
	this->player = p;
}

void Negotiate::setPeacePlayer(Player* n)
{
	this->enemyPlayer = n;
}

vector<Player*> Negotiate::updateListOfPlayers()
{
	return listOfPlayers;
}

//If the target is the player issuing the order, then the order is invalid.
bool Negotiate::validate() {
	bool isValid = false;
	if (enemyPlayer != player) {
		cout << "Negotiate order is valid" << endl;
		isValid = true;
	}
	else {
		cout << "Negotiate order is invalid" << endl;
	}
	return isValid;
}

//If the target is an enemy player, then the effect is that any attack that may be declared between territories
//of the player issuing the negotiate order and the target player will result in an invalid order.
void Negotiate::execute() {
	bool isValid = validate();
	if (isValid) {
		cout << "Executing negotiate..." << endl;
		player->addNegociate(enemyPlayer);
		enemyPlayer->addNegociate(player);
		cout << "Players " << player->GetPlayerName() << " and " << enemyPlayer->GetPlayerName() << " are negociating. No attacks can be done. " << endl;
	}
	else {
		cout << "Cannot execute negotiate." << endl;
	}
}

string Negotiate::stringToLog()
{
	return "Order: " + player->GetPlayerName() + " is negotiating with " + enemyPlayer->GetPlayerName();
}

////--------------------------ORDERSLIST---------------------------
//Default constructor
OrdersList::OrdersList() {
	
}

//Constructor
OrdersList::OrdersList(vector<Order*> anOrdersList) {
	listOfOrders = anOrdersList;
}

//Destructor
OrdersList::~OrdersList() {
	for (auto l : listOfOrders) {
		delete l;               //listOfOrders[i] is a pointer to an Order object
	}
	listOfOrders.clear();
}

//Copy constructor
OrdersList::OrdersList(OrdersList& anotherOrdersList) {
	this->listOfOrders = anotherOrdersList.listOfOrders;
}

//Assignment operator
OrdersList& OrdersList::operator=(const OrdersList& anOrdersList) {
	listOfOrders = anOrdersList.listOfOrders;
	return *this;
}

ostream& operator<<(ostream& outs, const OrdersList& anOrdersList) {
	cout << "List of orders:" << endl;
	for (int i = 0; i < anOrdersList.listOfOrders.size(); i++) {
		cout << i << ":" << (anOrdersList.listOfOrders[i])->getName() << endl;
	}
	return outs;
}

void OrdersList::addToListOfOrders(Order* order) {
	listOfOrders.push_back(order);                //Push order at the end of the list
	notifyAddOrder(order);
	Notify(this);
}

void OrdersList::move(int fromIndex, int toIndex) {
	if ((fromIndex >= 0 && fromIndex <= listOfOrders.size() - 1) && (toIndex >= 0 && toIndex <= listOfOrders.size() - 1)) {
		listOfOrders.insert(listOfOrders.begin() + toIndex, listOfOrders.at(fromIndex));
		listOfOrders.erase(listOfOrders.begin() + fromIndex);
	}
	else {
		cout << "Cannot move: invalid index" << endl;
	}
}

void OrdersList::remove(int index) {
	if (index >= 0 && index <= listOfOrders.size() - 1) {
		listOfOrders.erase(listOfOrders.begin() + index);
	}
	else {
		cout << "Cannot remove: invalid index" << endl;
	}
}

string OrdersList::stringToLog() {
	Order* latestOrder = listOfOrders.back();
	return "Order " + latestOrder->getName() + " added";
}

void OrdersList::notifyAddOrder(Order* order) {
	for (auto observer : *observers) {
		order->Attach(observer);
	}
}