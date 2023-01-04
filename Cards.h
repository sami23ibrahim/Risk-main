// Jennifer Osorio
// COMP 345 - Winter 2022
// Assignment 1

#pragma once

#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cassert>
// Need to include Player.h, Orders.h
#include "Orders.h"
#include "Player.h"
using namespace std;

void CardsDriver();

class Hand;
class Deck;
class Player; // Needed for when play

class Cards
{
private:
	vector<string> cardTypes = { "Bomb", "Blockade", "Airlift", "Negotiate" };
	string* type;

public:

	Cards();

	Cards(const Cards& c); // Copy constructor

	Cards& operator=(const Cards& c); // Assignment operator

	vector<string>* getTypePtr(); // get pointer to the type
	void printType(); // To try print type
	void setCardType(int n); // Set the type of card
	string* getCardType(); // Show the name of the type of card

	// Creates an Order object & places it in the Players' list of orders, removes card from Hand
	// & places it back in Deck
	//virtual void play();

	virtual ~Cards();
};


// Deck class
class Deck : public Cards
{
private:
	vector<Cards*> myDeck; // Collection of cards
	Cards* cardPtr; // pointer to card
	Cards* tempCard; // for draw & remove element of vector

public:

	Deck();
	Deck(const Deck& deck); // Assignment operator

	Deck& operator=(const Deck& deck); // Copy constructor

	void create_deck(); // Creates deck with each type of cards

	void printDeck();
	void printDeckSize();

	// Draw method that allows player to draw a card at random from the cards remaining in the deck
	// and place it in their hand
	Cards* draw();

	void addCardDeck(Cards* card); // Add specific card to deck
	~Deck(); // Destructor
};

class Hand : public Cards
{
private:
	vector<Cards*> myHand; // Collection of cards
	vector<Cards*> playCard; // For store play cards

public:

	Hand();
	Hand(const Hand& hand); // Assignment Constructor

	Hand& operator=(const Hand& hand);  // Copy constructor

	void setHand(Cards* c);
	void printHand();
	void printPlayCard();

	void play(Cards* c, Deck* d);

	void returnToDeck(Deck* d);

	vector<Cards*> getHand();
	vector<Cards*>* getPlayCard();

	void removePlayedCard(Cards* c);
	void clearPlayCards();

	~Hand(); // Destructor
};