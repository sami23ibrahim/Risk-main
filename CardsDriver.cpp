#include "Cards.h"

#include <iostream>
#include <string>

using namespace std;

void CardsDriver()
{
	Deck* deck = new Deck;
	Hand* hand = new Hand;

	cout << "Creating Deck..." << endl << endl;
	deck->create_deck();

	string answer;
	cout << "Do you wish to see the cards in the deck? (y/n)" << endl;
	cin >> answer;

	if (answer == "y")
	{
		deck->printDeck();
	}

	cout << endl;
	deck->printDeckSize();

	// Assign 6 random cards
	cout << "Let's assign you 6 random cards. " << endl;
	for (int i = 0; i < 6; i++)
	{
		hand->setHand(deck->draw());
	}

	cout << "Your 6 cards have been assigned. Do you wish to see them?(y/n)" << endl;
	cin >> answer;

	cout << endl;

	if (answer == "y")
	{
		hand->printHand();
	}

	cout << endl;

	deck->printDeckSize();

	// Show play
	cout << "Testing play. " << endl;
	cout << "Lets play all your cards from your hand" << endl << endl;

	int handSize = hand->getHand().size();

	for (int i = 0; i < handSize; i++)
	{
		hand->play(hand->getHand().at(0), deck);
	}

	hand->printPlayCard();
	hand->printHand();

	deck->printDeckSize();

	delete hand;
	delete deck;
}