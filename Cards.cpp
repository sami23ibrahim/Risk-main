#include <iostream>
#include <list>
#include <string>

#include "Cards.h"
using namespace std;

Cards::Cards()
{
}

// Copy constructor
Cards::Cards(const Cards& c)
{
	this->cardTypes = *new vector<string>(c.cardTypes);
	this->type = new string(*(c.type));
}

// Assignment operator
Cards& Cards::operator=(const Cards& c)
{
	//this->type = c.type;
	this->cardTypes = *new vector<string>(c.cardTypes);
	this->type = new string(*(c.type));

	return *this;
}

vector<string>* Cards::getTypePtr()
{
	return &cardTypes;
}

void Cards::printType()
{
	cout << "The card types are: " << endl;
	for (int i = 0; i < cardTypes.size(); i++)
	{
		cout << cardTypes.at(i) << endl;
	}
}

void Cards::setCardType(int n)
{
	type = &cardTypes.at(n);
}

string* Cards::getCardType()
{
	return type;
}

Cards::~Cards()
{
	cardTypes.clear();
	type = NULL;
//	delete	type;
}

// ------------------------------------------------------
// Deck
// ------------------------------------------------------
Deck::Deck()
{
	//this->myDeck = *new vector<Cards*>();
	//this->cardPtr = new Cards();
	//this->tempCard = new Cards();
}

// Assignment operator
Deck::Deck(const Deck& deck) : Cards(deck)
{
	this->myDeck = *new vector<Cards*>(deck.myDeck);
	this->cardPtr = new Cards(*(deck.cardPtr));
	this->tempCard = new Cards(*(deck.tempCard));
}

// Copy constructor
Deck& Deck::operator=(const Deck& deck)
{
	this->myDeck = *new vector<Cards*>(deck.myDeck);
	this->cardPtr = new Cards(*(deck.cardPtr));
	this->tempCard = new Cards(*(deck.tempCard));

	return *this;
}

void Deck::create_deck()
{
	//myDeck.clear();
	//cardPtr = NULL;
	//delete cardPtr;
	//tempCard = NULL;
	//delete tempCard;
	//cardPtr = NULL;
	// myDeck.clear();
	// Assign 40 cards in deck vector, each type has 8 cards, 5 types
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cardPtr = new Cards;

			// Type 1 = Bomb
			if (j == 0)
			{
				cardPtr->setCardType(0);
				myDeck.push_back(cardPtr);
			}
			// Type 2 = Blockage
			else if (j == 1)
			{
				cardPtr->setCardType(1);
				myDeck.push_back(cardPtr);
			}
			// Type 3 = Airlift
			else if (j == 2)
			{
				cardPtr->setCardType(2);
				myDeck.push_back(cardPtr);
			}
			// Type 4 = Negociate
			else if (j == 3)
			{
				cardPtr->setCardType(3);
				myDeck.push_back(cardPtr);
			}
		}
	}

	cout << "Deck has been created." << endl << endl;
}

void Deck::printDeck()
{
	cout << "Deck contains " << myDeck.size() << "cards." << endl;
	for (int i = 0; i < myDeck.size(); i++) {
		cout << "Card " << i << " is " << *myDeck.at(i)->getCardType() << endl;
	}
}

void Deck::printDeckSize()
{
	cout << "Deck currently contains " << myDeck.size() << " cards." << endl;
}

// Removes the card from the Deck and returns that card
Cards* Deck::draw()
{
	// Draw a random card from the deck
	srand(time(NULL));

	// Card number is from 0 to the size of the current deck
	int tempSize = myDeck.size();
	int temp = (rand() % tempSize);

	tempCard = myDeck.at(temp);

	myDeck.erase(myDeck.begin() + temp); // Remove from myDeck vector

	return tempCard;
}

void Deck::addCardDeck(Cards* card)
{
	myDeck.push_back(card);
}

Deck::~Deck()
{

	cout << "~Deck() " << cardPtr;

	
	delete tempCard; // for draw & remove element of vector
	delete(cardPtr);
	myDeck.clear();
	myDeck.shrink_to_fit();
	//myDeck.~vector();
}

// ----------------------------------------------------------
// Hand
// ----------------------------------------------------------
Hand::Hand()
{
}

// Assignment Operator
Hand::Hand(const Hand& hand) : Cards(hand)
{
	this->myHand = *new vector<Cards*>(hand.myHand);
	this->playCard = *new vector<Cards*>(hand.playCard);
}

// Copy constructor
Hand& Hand::operator=(const Hand& hand)
{
	this->myHand = *new vector<Cards*>(hand.myHand);
	this->playCard = *new vector<Cards*>(hand.playCard);

	return *this;
}

void Hand::setHand(Cards* c)
{
	myHand.push_back(c);
}

void Hand::printHand()
{
	cout << "Hand contains " << myHand.size() << " cards." << endl;
	for (int i = 0; i < myHand.size(); i++)
	{
		cout << "Card " << i << " is " << *myHand.at(i)->getCardType() << endl;
	}
}

void Hand::printPlayCard()
{
	if (playCard.size() > 0)
	{
		cout << "The cards to play are " << endl;
		for (int i = 0; i < playCard.size(); i++)
		{
			cout << i << " is " << *playCard.at(i)->getCardType() << endl;
		}
	}
	else
		cout << "There are no more cards to play..." << endl << endl;
}

void Hand::play(Cards* c, Deck* d)
{
	playCard.push_back(c);

	cout << *c->getCardType() << " card is being played." << endl;

	returnToDeck(d);

	removePlayedCard(c);

	playCard.pop_back();
}

void Hand::returnToDeck(Deck* d)
{
	for (int i = 0; i < playCard.size(); i++)
	{
		d->addCardDeck(playCard.at(i));
	}
}

vector<Cards*> Hand::getHand()
{
	return myHand;
}

vector<Cards*>* Hand::getPlayCard()
{
	return &playCard;
}

void Hand::removePlayedCard(Cards* c)
{
	for (int i = 0; i < myHand.size(); i++)
	{
		if (*myHand.at(i)->getCardType() == *c->getCardType())
		{
			myHand.erase(myHand.begin() + i);
			//cout << "Deleting card " << *c->getCardType() << " of hand..." << endl << endl;
			return;
		}
	}
}

void Hand::clearPlayCards()
{
	playCard.clear();

	cout << "Play cards vector has been cleared." << endl << endl;
}

Hand::~Hand()
{
	for (int i = 0; i < myHand.size(); i++)
		delete myHand[i];
}