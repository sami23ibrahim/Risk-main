#include <iostream>
#include <string>

//#include "Cards.h"
//#include "GameEngine.h"
#include "Orders.h"
//#include "Player.h"

using namespace std;

void OrdersDriver()
{
	cout << "--Constructors--" << endl;

	int* id1 = new int(10);
	int* id2 = new int(20);

	int* reinforcement1 = new int(20);
	int* reinforcement2 = new int(20);

	int reinforce1 = 20;
	int reinforce2 = 20;

	string* name1 = new string("Justine");
	string* name2 = new string("Jennifer");

	Territory* t1 = new Territory(1, "t1", "Turkey");
	t1->addArmies(10);
	Territory* t2 = new Territory(2, "t2", "Turkey");
	t2->addArmies(10);
	Territory* t3 = new Territory(3, "t3", "Turkey");
	t3->addArmies(10);
	Territory* t4 = new Territory(4, "t4", "Turkey");
	t4->addArmies(10);
	Territory* t5 = new Territory(5, "t5", "Turkey");
	t5->addArmies(10);

	t1->addAdjTerritory(t2);
	t2->addAdjTerritory(t1);
	t2->addAdjTerritory(t3);
	t3->addAdjTerritory(t2);
	t3->addAdjTerritory(t4);
	t3->addAdjTerritory(t5);
	t4->addAdjTerritory(t3);
	t4->addAdjTerritory(t5);
	t5->addAdjTerritory(t3);
	t5->addAdjTerritory(t4);

	vector<Territory*> list1 = { t1,t2, t4 };
	vector<Territory*> list2 = { t3, t5 };

	Deck* deck = new Deck;
	deck->create_deck();
	Hand* hand1 = new Hand;
	Hand* hand2 = new Hand;

	// Set 2 hands of cards
	for (int i = 0; i < 6; i++) {
		hand1->setHand(deck->draw());
	}
	for (int j = 0; j < 6; j++) {
		hand2->setHand(deck->draw());
	}

	OrdersList* ordersList1 = new OrdersList();
	OrdersList* ordersList2 = new OrdersList();

	Deploy* deploy = new Deploy();
	Advance* advance = new Advance();
	Bomb* bomb = new Bomb();
	Blockade* blockade = new Blockade();
	Airlift* airlift = new Airlift();
	Negotiate* negotiate = new Negotiate();

	cout << "Adding orders to list of orders" << endl;

	ordersList1->addToListOfOrders(deploy);
	ordersList2->addToListOfOrders(deploy);
	ordersList1->addToListOfOrders(advance);
	ordersList2->addToListOfOrders(advance);
	ordersList1->addToListOfOrders(bomb);
	ordersList2->addToListOfOrders(bomb);
	ordersList1->addToListOfOrders(blockade);
	ordersList2->addToListOfOrders(blockade);
	ordersList1->addToListOfOrders(airlift);
	ordersList2->addToListOfOrders(airlift);
	ordersList1->addToListOfOrders(negotiate);
	ordersList2->addToListOfOrders(negotiate);

	cout << endl;
	cout << "Creating players" << endl;

	Player* p1 = new Player(id1, reinforcement1, name1, list1, hand1, ordersList1);
	Player* p2 = new Player(id2, reinforcement2, name2, list2, hand2, ordersList2);

	cout << "Players created." << endl << endl;

	cout << "Setting territory owners." << endl;
	for (int i = 0; i < list1.size(); i++) {
		list1.at(i)->setPlayerOwner(p1);
	}
	for (int i = 0; i < list2.size(); i++) {
		list2.at(i)->setPlayerOwner(p2);
	}
	cout << "Finished assigning territory owners." << endl << endl;

	cout << "Creating list of all players" << endl;
	vector<Player*> allPlayers;
	allPlayers.push_back(p1);
	allPlayers.push_back(p2);

	/*Player* allPlayers = new Player();
	allPlayers->addPlayer(p1);
	allPlayers->addPlayer(p2);*/

	cout << "Players are: " << endl;
	for (int i = 0; i < allPlayers.size(); i++) {
		cout << allPlayers.at(i)->GetPlayerName() << " and their hand is " << endl;
		allPlayers.at(i)->getPlayerHand()->printHand();
		cout << endl;
	}

	//cout << "getTerritoryList:" << endl;
	//cout << allPlayers->getTerritoryList().size() << endl;
	//for (int i = 0; i < allPlayers->getTerritoryList().size(); i++) {
	//	cout << "Territory " << allPlayers->getTerritoryList().at(i)->getName() << " belongs to " << allPlayers->getTerritoryList().at(i)->getPlayerOwner()->GetPlayerName() << endl;
	//}

	string sourceTerritory;
	string targetTerritory;

	int nbArmies;

	string negociate;

	// Deploy
	cout << "\n--------------Deploy Execution with Player 1 ---------------" << endl;

	cout << "Your territories are: " << endl;
	for (int i = 0; i < p1->getTerritoryList().size(); i++) {
		cout << "\t" << p1->getTerritoryList().at(i)->getName() << endl;
	}

	do {
		cout << "Which territory do you wish to deploy to?" << endl;
		cin >> targetTerritory;
		cout << "How many units from reinforcement pool do you wish to deploy?" << endl;
		cin >> nbArmies;

		for (int i = 0; i < allPlayers.size(); i++) {
			for (int j = 0; j < allPlayers.at(i)->getTerritoryList().size(); j++)
			{
				if (allPlayers.at(i)->getTerritoryList().at(j)->getName() == targetTerritory) {
					deploy = new Deploy(p1, nbArmies, allPlayers.at(i)->getTerritoryList().at(j));
				}
			}
		}
	} while (deploy->validate() == false);
	deploy->execute();

	cout << "\n--------------Advance Execution with Player 2 ---------------" << endl;

	cout << "Your territories are " << endl;
	for (int i = 0; i < p2->getTerritoryList().size(); i++) {
		cout << "\t" << p2->getTerritoryList().at(i)->getName() << endl;
	}

	cout << "Your enemies territories are " << endl;
	for (int i = 0; i < allPlayers.size(); i++) {
		if (allPlayers.at(i) != p2) {
			cout << "\t" << allPlayers.at(i)->GetPlayerName() << "'s territories are: " << endl;
			for (int j = 0; j < allPlayers.at(i)->getTerritoryList().size(); j++) {
				cout << "\t\t" << allPlayers.at(i)->getTerritoryList().at(j)->getName() << endl;
			}
		}
	}

	do {
		cout << "From which territory do you wish to advance your army?" << endl;
		cin >> sourceTerritory;
		cout << "To which territory do you wish to advance your army?" << endl;
		cin >> targetTerritory;


		for (int i = 0; i < allPlayers.size(); i++) {
			for (int j = 0; j < allPlayers.at(i)->getTerritoryList().size(); j++) {
				// Search for territory having the name of the source territory
				if (allPlayers.at(i)->getTerritoryList().at(j)->getName() == sourceTerritory) {

					for (int k = 0; k < allPlayers.size(); k++) {
						for (int l = 0; l < allPlayers.at(k)->getTerritoryList().size(); l++) {
							// Search for territory having the name of the target territory
							if (allPlayers.at(k)->getTerritoryList().at(l)->getName() == targetTerritory) {
								cout << "How many units do you wish to move? You have " << allPlayers.at(i)->getTerritoryList().at(j)->getArmies()
									<< " units in " << allPlayers.at(i)->getTerritoryList().at(j)->getName() << endl;
								cin >> nbArmies;

								advance = new Advance(p2, nbArmies, allPlayers.at(i)->getTerritoryList().at(j), allPlayers.at(k)->getTerritoryList().at(l), deck);
							}
						}
					}
				}
			}
		}
	} while (advance->validate() == false);
	advance->execute();

	/*cout << "\n--------------Bomb Execution with Player 1 ---------------" << endl;

	cout << "Your enemy territories are " << endl;
	for (int i = 0; i < allPlayers.size(); i++) {
		if (allPlayers.at(i) != p1) {
			cout << "\t" << allPlayers.at(i)->GetPlayerName() << "'s territories are " << endl;
			for (int j = 0; j < allPlayers.at(i)->getTerritoryList().size(); j++) {
				cout << "\t\t" << allPlayers.at(i)->getTerritoryList().at(j)->getName() << endl;
			}
		}
	}

	do {
		cout << "Which territory do you wish to bomb? " << endl;
		cin >> targetTerritory;

		for (int i = 0; i < allPlayers.size(); i++) {
			for (int j = 0; j < allPlayers.at(i)->getTerritoryList().size(); j++) {
				if (allPlayers.at(i)->getTerritoryList().at(j)->getName() == targetTerritory) {
					bomb = new Bomb(p1, allPlayers.at(i)->getTerritoryList().at(j));
				}
			}
		}
	} while (bomb->validate() == false);
	bomb->execute();*/


	cout << "\n--------------blockade execution with player 2 ---------------" << endl;

	cout << "Your territories are: " << endl;
	for (int i = 0; i < p2->getTerritoryList().size(); i++) {
		cout << "\t" << p2->getTerritoryList().at(i)->getName() << endl;
	}

	do {
		cout << "Which territory do you wish to block?" << endl;
		cin >> targetTerritory;

		for (int i = 0; i < allPlayers.size(); i++) {
			for (int j = 0; j < allPlayers.at(i)->getTerritoryList().size(); j++) {
				if (allPlayers.at(i)->getTerritoryList().at(j)->getName() == targetTerritory) {
					blockade = new Blockade(p2, allPlayers, allPlayers.at(i)->getTerritoryList().at(j));
				}
			}
		}
	} while (blockade->validate() == false);
	blockade->execute();
	allPlayers = blockade->updateListOfPlayers();

	//cout << "\n--------------blockade execution with player 2 ---------------" << endl;

	//cout << "Your territories are: " << endl;
	//for (int i = 0; i < p2->getTerritoryList().size(); i++) {
	//	cout << "\t" << p2->getTerritoryList().at(i)->getName() << endl;
	//}

	//do {
	//	cout << "Which territory do you wish to block?" << endl;
	//	cin >> targetTerritory;

	//	for (int i = 0; i < allPlayers->getListOfPlayers().size(); i++) {
	//		for (int j = 0; j < allPlayers->getListOfPlayers().at(i)->getTerritoryList().size(); j++) {
	//			if (allPlayers->getListOfPlayers().at(i)->getTerritoryList().at(j)->getName() == targetTerritory) {
	//				blockade = new Blockade(p2, allPlayers, allPlayers->getListOfPlayers().at(i)->getTerritoryList().at(j));
	//			}
	//		}

	//	}
	//} while (blockade->validate() == false);
	//blockade->execute();

	cout << "\n--------------Airlift Execution with Player 1 ---------------" << endl;
	cout << "Your territories are: " << endl;
	for (int i = 0; i < p1->getTerritoryList().size(); i++) {
		cout << "\t" << p1->getTerritoryList().at(i)->getName() << endl;
	}

	do {
		cout << "From which territory do you wish to airlift?" << endl;
		cin >> sourceTerritory;
		cout << "To which territory do you wish to airlift?" << endl;
		cin >> targetTerritory;
		for (int i = 0; i < allPlayers.size(); i++) {
			for (int j = 0; j < allPlayers.at(i)->getTerritoryList().size(); j++) {
				// Search for territory having the name of the source territory
				if (allPlayers.at(i)->getTerritoryList().at(j)->getName() == sourceTerritory) {
					for (int k = 0; k < allPlayers.size(); k++) {
						for (int l = 0; l < allPlayers.at(k)->getTerritoryList().size(); l++) {
							// Search for territory having the name of the target territory
							if (allPlayers.at(k)->getTerritoryList().at(l)->getName() == targetTerritory) {
								cout << "How many units do you wish to move? You have " << allPlayers.at(i)->getTerritoryList().at(j)->getArmies()
									<< " units in " << allPlayers.at(i)->getTerritoryList().at(j)->getName() << endl;
								cin >> nbArmies;

								airlift = new Airlift(p1, nbArmies, allPlayers.at(i)->getTerritoryList().at(j), allPlayers.at(k)->getTerritoryList().at(l));
							}
						}
					}
				}
			}
		}
	} while (airlift->validate() == false);
	airlift->execute();

	cout << "\n--------------Negociate Execution with Player 2 ---------------" << endl;
	cout << "List of players:" << endl;
	for (int i = 0; i < allPlayers.size(); i++) {
		if (allPlayers.at(i) != p2) {
			cout << "\t" << allPlayers.at(i)->GetPlayerName() << endl;
		}
		/*for (int j = 0; j < allPlayers.at(i)->getTerritoryList().size(); j++) {
			cout << "\t\t" << allPlayers.at(i)->getTerritoryList().at(j)->getName() << endl;
		}*/
	}

	do {
		cout << "With which player do you wish to negociate? (Write the name of the player) " << endl;
		cin >> negociate;

		for (int i = 0; i < allPlayers.size(); i++) {
			if (allPlayers.at(i)->GetPlayerName() == negociate) {
				negotiate = new Negotiate(p2, allPlayers.at(i));
			}
		}
	} while (negotiate->validate() == false);
	negotiate->execute();

	cout << "\n--------------Bomb Execution with Player 1 ---------------" << endl;

	cout << "Your enemy territories are " << endl;
	for (int i = 0; i < allPlayers.size(); i++) {
		if (allPlayers.at(i) != p1) {
			//cout << "\t" << allPlayers->getListOfPlayers().at(i)->GetPlayerName() << "'s territories are " << endl;
			for (int j = 0; j < allPlayers.at(i)->getTerritoryList().size(); j++) {
				cout << "\t\t" << allPlayers.at(i)->getTerritoryList().at(j)->getName() << " Owner: " << allPlayers.at(i)->getTerritoryList().at(j)->getPlayerOwner()->GetPlayerName() << endl;
			}
		}
	}

	do {
		cout << "Which territory do you wish to bomb? " << endl;
		cin >> targetTerritory;

		for (int i = 0; i < allPlayers.size(); i++) {
			for (int j = 0; j < allPlayers.at(i)->getTerritoryList().size(); j++) {
				if (allPlayers.at(i)->getTerritoryList().at(j)->getName() == targetTerritory) {
					bomb = new Bomb(p1, allPlayers.at(i)->getTerritoryList().at(j));
				}
			}
		}
	} while (bomb->validate() == false);
	bomb->execute();

	// Delete all
	deploy->~Deploy();
	advance->~Advance();
	bomb->~Bomb();
	blockade->~Blockade();
	airlift->~Airlift();
	negotiate->~Negotiate();
	//ordersList1->~OrdersList();
//	ordersList2->~OrdersList();

	//deck->~Deck();
	//hand1->~Hand();
	//hand2->~Hand();

	//p1->~Player();
	//p2->~Player();
	//allPlayers->~Player();

	//for (auto tl1 : list1) {
	//	delete tl1;
	//}
	//list1.clear();

	//for (auto tl2 : list2) {
	//	delete tl2;
	//}
	//list2.clear();

	//delete name1;
	//name1 = nullptr;
	//delete name2;
	//name2 = nullptr;

	cout << "\nEnd of testing order execution" << endl;
}