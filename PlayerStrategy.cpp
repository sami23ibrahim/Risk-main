#include "PlayerStrategy.h"
//#include "Orders.h"
//#include "Map.h"
//#include "Player.h"
#include <vector>
#include <set>
using namespace std;
class Orders;
class Player;
class Territory;

//--------------------------------------------------HUMAN PLAYER STRATEGY--------------------------------------------------
#pragma region Human Player Strategy

HumanPlayerStrategy::HumanPlayerStrategy(Player* p, vector<Player*> all, Deck* d) {
	this->player = p;
	this->allPlayers = all;
	this->deck = d;
}

// Why did this method like this? Why an int? why not using string order?
// NOT GOING TO USE THIS, WILL DO IT IN DRIVER (EASIER)
int HumanPlayerStrategy::changeStrategy(string order, int armies)
{
	cout << "HumanPlayerStrategy: Please choose a strategy from the following \n"
		"1 for issueOrder(), 2 for toAttack(), 3 for toDefend(). " << endl;			//Supposed to be strategies (Aggressive, Neutral...), not methods?
	int input;
	cin >> input;
	if (input == 1) {
		issueOrder();
		return armies - 1;
	}
	else if (input == 2) {
		toAttack();
		return armies - 2;
	}
	else if (input == 3) {
		toDefend();
		return armies;
	}
	else {
		cout << "Please input an correct integer from 1 to 3...\n" << endl;
		return 0;
	}
}

// The player issues deploy orders on its own territories that are in the list returned by toDefend(). As long as the player has armies still to deploy (see startup phase and reinforcement phase), 
// it will issue a deploy order and no other order. Once it has deployed all its available armies, it can proceed with other kinds of orders.
void HumanPlayerStrategy::issueOrder()
{
	int source;
	int target;
	int units;

	int defend;
	int attack;

	string input;
	// If reinforcement pool > 0, then do deploy orders
	while (player->getReinforcementPool() > 0)
	{
		cout << "Player " << player->GetPlayerName() << "'s reinforcement pool contains " << player->getReinforcementPool() << " armie units." << endl;

		player->printToDefend();

		cout << "\nTo which territory would you want to issue a deploy order? (Enter the number of the territory in your defendList)" << endl;
		cin >> target;

		cout << "How many units would you like to deploy?" << endl;
		cin >> units;

		Deploy* deployOrder = new Deploy(player, units, player->getListToDefend().at(target));
		player->setOrder(deployOrder);
	}

	cout << "Player " << player->GetPlayerName() << " has no more reinforcement units." << endl;

	cout << "Your hand contains the following cards: " << endl;
	player->getPlayerHand()->printHand();
	cout << "Advance" << endl;

	cout << "Which order would you wish to issue?" << endl;
	cin >> input;

	// If the player issues an order card that is in its hand
	if (input != "Advance") {
		for (int i = 0; i < player->getPlayerHand()->getHand().size(); i++) {
			if (*player->getPlayerHand()->getHand().at(i)->getCardType() == input) {
				if (input == "Airlift") {
					player->printToDefend();
					cout << "\nFrom which territory do you wish to airlift?" << endl;
					cin >> source;

					cout << "\nTo which territory do you wish to airlift?" << endl;
					cin >> target;

					cout << "\nHow many army units do you wish to airlift? You have " << player->getListToDefend().at(source)->getArmies() << " armie units at source territory " << player->getListToDefend().at(source)->getName() << endl;
					cin >> units;

					Airlift* airliftOrder = new Airlift(player, units, player->getListToDefend().at(source), player->getListToDefend().at(target));
					player->setOrder(airliftOrder);
				}
				else if (input == "Bomb") {
					player->printToAttack();

					cout << "\nWhich territory do you wish to bomb?" << endl;
					cin >> target;

					Bomb* bombOrder = new Bomb(player, player->getListToAttack().at(target));
					player->setOrder(bombOrder);
				}
				else if (input == "Blockade") {
					player->printToDefend();

					cout << "\nFor which territory do you wish to do a blockade?" << endl;
					cin >> target;

					Blockade* blockadeOrder = new Blockade(player, allPlayers, player->getListToDefend().at(target));
					player->setOrder(blockadeOrder);
				}
				else if (input == "Negotiate")
				{
					for (int i = 0; i < allPlayers.size(); i++) {
						if (allPlayers.at(i) == player) {
							cout << i << ". " << allPlayers.at(i)->GetPlayerName() << " (You)" << endl;
						}
						else {
							cout << i << ". " << allPlayers.at(i)->GetPlayerName() << endl;
						}
					}
					cout << "With which player do you wish to negociate?" << endl;
					cin >> target;

					Negotiate* negotitateOrder = new Negotiate(player, allPlayers.at(target));
					player->setOrder(negotitateOrder);
				}
			}
		}
	}
	else if (input == "Advance") {
		cout << "Do you wish to defend or attack? (write in lowercase defend or attack)" << endl;
		cin >> input;

		bool isValid = false;

		if (input == "defend")
		{
			player->printToDefend();

			// Make sure that player selects 2 territories that are adjacent
			while (isValid) {
				cout << "From which territory do you wish to send armie units?" << endl;
				cin >> source;

				cout << "Which territory do you wish to defend?" << endl;
				cin >> target;

				cout << "How many army units do you wish to send? You have " << player->getListToDefend().at(source)->getArmies() << " units at source territory " << player->getListToDefend().at(source)->getName() << endl;
				cin >> units;

				if (player->getListToDefend().at(source)->isAdjacentTo(player->getListToDefend().at(target)->getId()) == true) {
					Advance* advanceOrder = new Advance(player, units, player->getListToDefend().at(source), player->getListToDefend().at(target), deck);

					player->setOrder(advanceOrder);
				}
				else {
					cout << "The territories " << player->getListToDefend().at(source)->getName() << " and " << player->getListToDefend().at(target)->getName() << " are not adjacent territories. Please select adjacent territories." << endl;
				}
			}
		}
		else if (input == "attack")
		{
			// Make sure that player selects 2 territories that are adjacent
			while (isValid) {
				cout << "Your territories are:" << endl;
				for (int i = 0; player->getTerritoryList().size(); i++)
				{
					cout << i << ". " << player->getTerritoryList().at(i)->getName() << " with " << player->getTerritoryList().at(i)->getArmies() << endl;
				}

				cout << endl;
				player->printToAttack();

				cout << "From which territory do you wish to send armie units? (Please enter the number of a territory that belongs to you " << player->GetPlayerName() << ")" << endl;
				cin >> source;

				cout << "Which territory do you wish to attack from the attack list? (Please enter the number of the territory)" << endl;
				cin >> target;

				cout << "How many army units do you wish to send? You have " << player->getTerritoryList().at(source)->getArmies() << " units at source territory " << player->getTerritoryList().at(source)->getName() << endl;
				cin >> units;

				// Make sure that the target is adjacent to source territory
				if (player->getTerritoryList().at(source)->isAdjacentTo(player->getListToAttack().at(target)->getId()) == true) {
					Advance* advanceOrder = new Advance(player, units, player->getTerritoryList().at(source), player->getListToAttack().at(target), deck);

					player->setOrder(advanceOrder);
					isValid = true;
				}
				else {
					cout << "The territories " << player->getTerritoryList().at(source)->getName() << " and " << player->getListToAttack().at(target)->getName() << " are not adjacent territories. Please select adjacent territories." << endl;
					isValid = false;
				}
			}
		}
	}
	else if (input == "Exit")
	{
		cout << "Exiting" << endl;
	}
}

// From Assignment 2 (Orders Issuing phase): 
// The player decides which neighboring territories are to be attacked in priority (as a list return by the toAttack() method), and which of their own territories are to be defended 
// in priority (as a list returned by the toDefend() method).
vector<Territory*> HumanPlayerStrategy::toAttack()
{
	cout << "HumanPlayerStrategy: toAttack() " << endl;
	//vector<Territory*> attackList;

	// If attack list is not empty, then empty it
	if (!player->getListToAttack().empty())
		player->clearToAttack();

	string toAttack;
	string input;

	auto tList = player->getTerritoryList();

	for (int i = 0; i < tList.size(); i++)
	{
		auto adjT = tList.at(i)->getAdjTerritories();
		for (int j = 0; j < adjT.size(); j++) {
			// Make sure territory does not belong to player
			if (adjT.at(j)->getPlayerOwner()->GetPlayerID() != player->GetPlayerID()) {
				cout << "Adjacent territories of territory " << tList.at(i)->getName() << " are: " << endl;
				cout << "\t" << adjT.at(j)->getName() << " belonging to player " << adjT.at(j)->getPlayerOwner()->GetPlayerName()
					<< " and has " << adjT.at(j)->getArmies() << " armie units." << endl;
			}
		}
	}

	/// Do while loop so the player can add the territories they wish to attack to the attackList
	while (true)
	{
		cout << "Player: " << player->GetPlayerName() << " Which territory do you wish to attack? (Enter territory name)" << endl;
		cin >> toAttack;
		bool addedAttack = false;

		for (int i = 0; i < player->getTerritoryList().size(); i++)
		{
			if (addedAttack) break;
			for (int j = 0; j < player->getTerritoryList().at(i)->getAdjTerritories().size(); j++) {
				// If territory does not belong to the player, then it's a valid input.
				if (player->getTerritoryList().at(i)->getAdjTerritories().at(j)->getName() == toAttack && player->getTerritoryList().at(i)->getAdjTerritories().at(j)->getPlayerOwner()->GetPlayerID() != player->GetPlayerID()) {
					cout << "Territory " << player->getTerritoryList().at(i)->getAdjTerritories().at(j)->getName() << " is being added to attackList." << endl;
					player->addTerritoryToAttack(player->getTerritoryList().at(i)->getAdjTerritories().at(j));
					addedAttack = true;
					break;
				}
				// If territory belongs to the player, it's not a valid input
				else if (player->getTerritoryList().at(i)->getAdjTerritories().at(j)->getName() == toAttack && player->getTerritoryList().at(i)->getAdjTerritories().at(j)->getPlayerOwner() == player) {
					cout << "The territory " << player->getTerritoryList().at(i)->getAdjTerritories().at(j)->getName() << " belongs to you, you can't attack your own territory, please try again." << endl;
					continue;
				}
			}
		}

		cout << "Do you wish to add another territory to your attackList? (yes or no)" << endl;
		cin >> input;

		if (input == "yes") {
			continue;
		}
		else {
			break;
		}
	}

	return player->getListToAttack();
}

// From Assignment 2 (Orders Issuing phase): 
// The player decides which neighboring territories are to be attacked in priority (as a list return by the toAttack() method), and which of their own territories are to be defended 
// in priority (as a list returned by the toDefend() method).
vector<Territory*> HumanPlayerStrategy::toDefend()
{
	cout << "HumanPlayerStrategy: toDefend() " << endl;

	// Clear defend list if it's not empty
	if (!player->getListToDefend().empty())
		player->clearToDefend();

	int defend;
	string input;

	cout << "Your territories are: " << endl;
	for (int i = 0; i < player->getTerritoryList().size(); i++) {
		cout << "\t" << i << ". " << player->getTerritoryList().at(i)->getName() << " with armies: " << player->getTerritoryList().at(i)->getArmies() << endl;
	}

	// Do while loop so the player can add the territories they wish to defend to the defendList
	while (true)
	{
		cout << "Which territory do you wish to defend? (Enter territory name)" << endl;
		cin >> defend;

		player->addTerritoryToDefend(player->getTerritoryList().at(defend));
		player->printToDefend();

		cout << "Do you wish to add another territory to your defendList? (yes or no)" << endl;
		cin >> input;

		if (input == "yes") {
			continue;
		}
		else {
			break;
		}
	}

	return player->getListToDefend();
}

void HumanPlayerStrategy::PrintStrategy()
{
	std::cout << "Human Player Strategy\n";
}

#pragma region Helper Functions

int HumanPlayerStrategy::ChooseValidOption(int max)
{
	int region = -1;
	while (region >= max || region < 0) {
		cout << "choose a valid option: ";
		cin >> region;
	}

	return region;
}

int HumanPlayerStrategy::GetArmyNumber(int army)
{
	int rein = -1;
	while (rein > army || rein < 0) {
		cout << "choose a valid amount: ";
		cin >> rein;
		cout << endl;
	}

	return rein;
}

void HumanPlayerStrategy::ShowTerritory()
{
	cout << "These are the available to Choose from" << endl;
	for (int i = 0; i < player->getTerritoryList().size(); i++)
	{
		cout << i << ". " << player->getTerritoryList().at(i)->getName() << " (" << player->getTerritoryList().at(i)->getArmies() << ")" << endl;
	}
}

vector<Territory*> HumanPlayerStrategy::GetEnemyTerritory()
{
	vector<Territory*> nonPlayerTerritory;

	// Modified by Jennifer, changed from for each(Territory* territory in player->getMap()) to this bc the other one gave errors
	for (int i = 0; i < allPlayers.size(); i++) {
		for (int j = 0; j < allPlayers.at(i)->getTerritoryList().size(); j++) {
			if (allPlayers.at(i)->getTerritoryList().at(j)->getPlayerOwner() != player) {
				nonPlayerTerritory.push_back(allPlayers.at(i)->getTerritoryList().at(j));
			}
		}
	}

	return nonPlayerTerritory;
}

set<Player*> HumanPlayerStrategy::GetPlayers()
{
	set<Player*> players;

	// Changed by Jennifer
	for (Player* p : allPlayers) {
		if (p != player) {
			players.insert(p);
		}
	}
	return players;
}

#pragma endregion

#pragma endregion

//--------------------------------------------------AGGRESSIVE PLAYER STRATEGY--------------------------------------------------

#pragma region Aggressive Player Strategy

AggressivePlayerStrategy::AggressivePlayerStrategy(Player* p, vector<Player*> all, Deck* d) {
	this->player = p;
	this->allPlayers = all;
	this->deck = d;
}

int AggressivePlayerStrategy::changeStrategy(string order, int armies)
{
	cout << "AggressivePlayerStrategy: changeStrategy() " << endl;
	return 0;
}

//computer player that focuses on attack (deploys or advances armies on its strongest country)
//Then always advances to enemy territories until it cannot do so anymore). 
void AggressivePlayerStrategy::issueOrder()
{
	Territory* source;
	Territory* target;
	//Deploys or advances armies on its strongest territories
	//Find the strongest territory
	//for all territories in its list of territories, if territory at i has more armies, then strongest territory = territory at i
	//Create a Deploy or Advance (on its own territory) order, move all armies on the strongest country
	if (player->getReinforcementPool() > 0)
	{
		// If player has reinforcement units, then deploy
		target = player->getTerritoryList().at(0);
		// Find territory with largest number of units
		for (int i = 0; i < player->getTerritoryList().size(); i++) {
			if (target->getArmies() < player->getTerritoryList().at(i)->getArmies()) {
				target = player->getTerritoryList().at(i);
			}
		}
		Deploy* deployOrder = new Deploy(player, player->getReinforcementPool(), target);

		player->setOrder(deployOrder);
	} 
	// Else, advance
	//Always advances to enemy territories until it cannot do so anymore
	//Create Advance order on enemy territory
	source = player->getTerritoryList().at(0);
	// While the player still has armie units, do advance
	if (source->getArmies() > 0)
	{
		// Find territory with largest number of units; Going to be the source territory
		for (int i = 0; i < player->getTerritoryList().size(); i++) {
			if (source->getArmies() < player->getTerritoryList().at(i)->getArmies()) {
				source = player->getTerritoryList().at(i);
			}
		}

		// Go through all the territories
		for (int i = 0; i < allPlayers.size(); i++) {
			target = allPlayers.at(i)->getTerritoryList().at(0);
			for (int j = 0; j < allPlayers.at(i)->getTerritoryList().size(); j++) {
				if (allPlayers.at(i)->getTerritoryList().at(j)->getPlayerOwner() != player) {
					// If the enemie territory has lowest amount of armie units and is adjacent to the source territory, then it's going to be the target
					if (target->getArmies() > allPlayers.at(i)->getTerritoryList().at(j)->getArmies() && source->isAdjacentTo(allPlayers.at(i)->getTerritoryList().at(j)->getId()))
						target = allPlayers.at(i)->getTerritoryList().at(j);
				}
			}
		}
		Advance* advanceOrder = new Advance(player, source->getArmies(), source, target, deck);

		player->setOrder(advanceOrder);
	}

	//cout << "\nAggressive player deployed/Advanced all its armies and reinforcements until it no longer can." << endl;
}

vector<Territory*> AggressivePlayerStrategy::toAttack()
{
	// Empty toDefend list
	if (!player->getListToAttack().empty())
		player->clearToAttack();

	// Adding territory to defend by the least number of units
	for (int i = 0; i < player->getTerritoryList().size(); i++) {
		for (int j = 0; j < player->getTerritoryList().at(i)->getAdjTerritories().size(); j++) {
			if (player->getTerritoryList().at(i)->getAdjTerritories().at(j)->getPlayerOwner() != player) {
				player->addTerritoryToAttack(player->getTerritoryList().at(i)->getAdjTerritories().at(j));
			}
		}
	}

	return player->getListToAttack();
}

vector<Territory*> AggressivePlayerStrategy::toDefend()
{
	// Empty toDefend list
	if (!player->getListToDefend().empty())
		player->clearToDefend();

	// Adding territory to defend by the least number of units
	//player->sortLeastToGreatestUnits();
	for (int i = 0; i < player->getTerritoryList().size(); i++)
	{
		player->addTerritoryToDefend(player->getTerritoryList().at(i));
	}

	player->printToDefend();

	return player->getListToDefend();
}

void AggressivePlayerStrategy::PrintStrategy()
{
	std::cout << "Aggressive Player Strategy\n";
}

#pragma endregion

//--------------------------------------------------BEENEVOLENT PLAYER STRATEGY--------------------------------------------------

#pragma region Benevolent Player Strategy

BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player* p, vector<Player*> all, Deck* d) {
	this->player = p;
	this->allPlayers = all;
	this->deck = d;
}

int BenevolentPlayerStrategy::changeStrategy(string order, int numArmy)
{
	cout << "BenevolentPlayerStrategy: changeStrategy() " << endl;

	return 0;

}

//computer player that focuses on protecting its weak countries (deploys or advances armies on its weakest countries, 
//never advances to enemy territories).
void BenevolentPlayerStrategy::issueOrder()
{
	//Deploys its weakest territories
	//Find the weakest territory
	//set weakest territory to index 0
	//for all territories in its list of territories, if territory at i has less armies, then weakest territory = territory at i
	//Create a Deploy on the weakest country
	Territory* source;
	Territory* target;
	if (player->getReinforcementPool() > 0)
	{
		// If player has reinforcement units, then deploy
		source = player->getTerritoryList().at(0);
		// Find territory with largest number of units
		for (int i = 0; i < player->getTerritoryList().size(); i++) {
			if (source->getArmies() > player->getTerritoryList().at(i)->getArmies()) {
				source = player->getTerritoryList().at(i);
			}
		}
		Deploy* deployOrder = new Deploy(player, player->getReinforcementPool(), source);

		player->setOrder(deployOrder);
	}
	//Check adjacent countries that has less armies then the others and move armies to the weaker countries (Advance)
	source = player->getTerritoryList().at(0);
	if (source->getArmies() > 0)
	{
		// Find territory with largest number of units; Going to be the source territory
		for (int i = 0; i < player->getTerritoryList().size(); i++) {
			if (source->getArmies() < player->getTerritoryList().at(i)->getArmies()) {
				source = player->getTerritoryList().at(i);
			}
		}

		// Get target territory which is adjacent to the source
		target = player->getTerritoryList().at(0);
		for (int i = 0; i < player->getTerritoryList().size(); i++) {
			if (source->isAdjacentTo(player->getTerritoryList().at(i)->getId()) && target->getArmies() > player->getTerritoryList().at(i)->getArmies()) {
				target = player->getTerritoryList().at(i);
			}
		}
		Advance* advanceOrder = new Advance(player, source->getArmies(), source, target, deck);
		player->setOrder(advanceOrder);
	}

	//Cannot advance on enemy territories
}

vector<Territory*> BenevolentPlayerStrategy::toAttack()
{
	cout << "Player " << player->GetPlayerName() << " is a benevolent player and a benevolent player does not attack any territories." << endl;
	return player->getListToAttack();
}

vector<Territory*> BenevolentPlayerStrategy::toDefend()
{
	// Empty toDefend list
	if (!player->getListToDefend().empty())
		player->clearToDefend();

	// Adding territory to defend by the least number of units
	//player->sortLeastToGreatestUnits();
	for (int i = 0; i < player->getTerritoryList().size(); i++)
	{
		player->addTerritoryToDefend(player->getTerritoryList().at(i));
	}

	player->printToDefend();

	return player->getListToDefend();
}

void BenevolentPlayerStrategy::PrintStrategy()
{
	std::cout << "Benevolent Player Strategy\n";
}

#pragma endregion

//--------------------------------------------------NEUTRAL PLAYER STRATEGY--------------------------------------------------

#pragma region Neutral Player Strategy

//sets player
NeutralPlayerStrategy::NeutralPlayerStrategy(Player* player, vector<Player*> all, Deck* d) {
	this->player = player;
	this->allPlayers = all;
	this->deck = d;

}

int NeutralPlayerStrategy::changeStrategy(string, int)
{
	return 0;
}

//computer player that never issues any order. If a Neutral player is attacked, it becomes an Aggressive player.
void NeutralPlayerStrategy::issueOrder()
{
	//Cannot issue order unless attcacked

	int territoryAmount = 0;
	int armyAmount = 0;

	//if army is attacked
	if (territoryAmount < territoryCount || armyAmount < armyCount) {

		cout << "Neutral player is attacked, now it is aggressive" << endl;

		//change player strategy
		//player->setPlayerStrategy(Aggressive); // have tocreate setPlayerStrategy
		//player->issueOrder();

		//not sure if this is allowed, remove if necessary
		this->~NeutralPlayerStrategy();
	}
	else {
		territoryCount = territoryAmount;
		armyCount = armyAmount;
	}
}

//never needs to attack
vector<Territory*> NeutralPlayerStrategy::toAttack()
{
	vector<Territory*> DefendList;
	cout << "Neutral does not attack" << endl;
	return DefendList;
}

//never needs to defend
vector<Territory*> NeutralPlayerStrategy::toDefend()
{
	vector<Territory*> DefendList;
	cout << "Neutral does not defend" << endl;
	return DefendList;
}

void NeutralPlayerStrategy::PrintStrategy()
{
	std::cout << "Neutral Player Strategy\n";
}

NeutralPlayerStrategy::~NeutralPlayerStrategy()
{

}

#pragma endregion


//--------------------------------------------------CHEATER PLAYER STRATEGY--------------------------------------------------

#pragma region Cheater Player Strategy

CheaterPlayerStrategy::CheaterPlayerStrategy(Player* player, vector<Player*> all, Deck* d)
{
	this->player = player;
	this->allPlayers = all;
	this->deck = d;
}

int CheaterPlayerStrategy::changeStrategy(string, int)
{
	return 0;
}

//computer player that automatically conquers all territories that are adjacent to its own territories(only once per turn)
void CheaterPlayerStrategy::issueOrder()
{
	Territory* source;
	Territory* target;

	// Go through list of cheater player territories
	for (int i = 0; i < player->getTerritoryList().size(); i++)
	{
		source = player->getTerritoryList().at(i);

		// Conquer all adjacent territories that are not his
		for (int j = 0; j < source->getAdjTerritories().size(); j++) {
			//target = source->getAdjTerritories().at(i);
			
			target = source->getAdjTerritories().at(j);

			if (target->getPlayerOwner() != player) {
				target->setPlayerOwner(player);
			}
		}
	}
}

vector<Territory*> CheaterPlayerStrategy::toAttack()
{
	vector<Territory*> AttackList;

	return AttackList;
}

//cheater never needs to defend
vector<Territory*> CheaterPlayerStrategy::toDefend()
{
	vector<Territory*> DefendList;
	return DefendList;
}

void CheaterPlayerStrategy::PrintStrategy()
{
	std::cout << "Cheater Player Strategy\n";
}

#pragma endregion

void PlayerStrategy::setDeck(Deck* d)
{
	this->deck = d;
}

void PlayerStrategy::updateAllPlayers(vector<Player*> all)
{
	allPlayers = all;
}
