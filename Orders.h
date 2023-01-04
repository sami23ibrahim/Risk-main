#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Map.h"
#include "Player.h"
#include "LoggingObserver.h"
#include "Cards.h"

using namespace std;

class Player;
class Territory;
class Cards;
class Hand;
class Deck;

void OrdersDriver();

class Order : public Subject, public ILoggable {
public:
    //Constructor and destructor
    Order();
    virtual ~Order();

    //Copy constructor, assignement and stream insertion operator
    Order(Order& anotherOrder);
    Order& operator =(const Order& anOrder);
    friend istream& operator>>(istream&, Order&);
    friend ostream& operator<<(ostream&, const Order&);

    //Getters and setters
    virtual string const getName();
    void setName(string);

    //Member functions
    // I added virtual in front
    virtual bool validate() = 0; //pure virtual
    virtual void execute() = 0; //pure virtual

    // Not using these
    void addPlayer(Player*);
    vector<Player*> getListOfPlayers();

    // Following update method is to update the list of players. Make sure to use it after execute an order
    virtual vector<Player*> updateListOfPlayers() = 0;

    string stringToLog() override;

protected:
    string name;
    Player* player;
    //Player* allPlayers;
    vector<Player*> listOfPlayers;
};

//A deploy order tells a certain number of armies taken from the reinforcement pool to deploy to a target territory owned 
//by the player issuing this order.
class Deploy : public Order {
public:
    //Constructor and destructor
    Deploy();
    Deploy(Player*, int, Territory*);   //player, armies, target
    ~Deploy();

    //Copy constructor, assignement and stream insertion operator
    Deploy(Deploy& aDeploy);
    Deploy& operator =(const Deploy& aDeploy);
    friend ostream& operator<<(ostream&, const Deploy&);

    //Getters and setters
    string const getName() override;

    // For Assignment 3
    void setSelfPlayers(Player*);
    void setTargetTerritory(Territory*);
    void setNumArmy(int);
    vector<Player*> updateListOfPlayers() override;

    //Member functions
    bool validate() override;
    void execute() override;

    string stringToLog() override;

private:
    int armies;
    Territory* target;
};

// An advance order tells a certain number of army units to move from a source territory to a target adjacent territory.
class Advance : public Order {
public:
    //Constructor and destructor
    Advance();
    Advance(Player*, int, Territory*, Territory*, Deck*);  //player, armies, source, target
    ~Advance();

    //Copy constructor, assignement and stream insertion operator
    Advance(Advance& anAdvance);
    Advance& operator =(const Advance& anAdvance);
    friend ostream& operator<<(ostream&, const Advance&);

    //Getters and setters
    string const getName() override;

    // For Assignment 3
    void setSelfPlayers(Player*);
    void setSourceTerritory(Territory*);
    void setTargetTerritory(Territory*);
    void setArmyUnits(int);
    vector<Player*> updateListOfPlayers() override;

    //Member functions
    bool validate() override;
    void execute() override;

    string stringToLog() override;
private:
    int armies;
    Territory* source;
    Territory* target;
    //Hand* playerHand;
    Deck* deck;
};

//A bomb order targets a territory owned by another player than the one issuing the order. Its result is
//to remove half of the armies from this territory.The bomb order can only be created by playing the bomb card.
class Bomb : public Order {
public:
    //Constructor and destructor
    Bomb();
    Bomb(Player*, Territory*);  //player, target
    ~Bomb();

    //Copy constructor, assignement and stream insertion operator
    Bomb(Bomb& aBomb);
    Bomb& operator =(const Bomb& aBomb);
    friend ostream& operator<<(ostream&, const Bomb&);

    //Getters and setters
    string const getName() override;

    // For Assignment 3
    void setSelfPlayers(Player*);
    void setTargetTerritory(Territory*);
    vector<Player*> updateListOfPlayers() override;

    //Member functions
    bool validate() override;
    void execute() override;

    string stringToLog() override;

private:
    Territory* target;
};

//A blockade order targets a territory that belongs to the player issuing the order. Its effect is to
//double the number of armies on the territory and to transfer the ownership of the territory to the Neutral player.
//The blockade order can only be created by playing the blockade card.
class Blockade : public Order {
public:
    //Constructor and destructor
    Blockade();
    Blockade(Player*, vector<Player*>, Territory*);  //player, target
    ~Blockade();

    //Copy constructor, assignement and stream insertion operator
    Blockade(Blockade& aBlockade);
    Blockade& operator =(const Blockade& aBlockade);
    friend ostream& operator<<(ostream&, const Blockade&);

    //Getters and setters
    string const getName() override;

    // For Assignment 3
    void setSelfPlayers(Player*);
    void setNeutralPlayer(Player*);
    void setTargetTerritory(Territory*);
    vector<Player*> updateListOfPlayers() override;

    //Member functions
    bool validate() override;
    void execute() override;

    string stringToLog() override;
private:
    Territory* target;
    Player* neutralPlayer;
    //vector<Player*> listOfPlayers;
    //Player* allPlayers;
};

//An airlift order tells a certain number of armies taken from a source territory to be moved to a target
//territory, the source and the target territory being owned by the player issuing the order.The airlift order can only
//be created by playing the airlift card
class Airlift : public Order {
public:
    //Constructor and destructor
    Airlift();
    Airlift(Player*, int, Territory*, Territory*);   //player, armies, source, target
    ~Airlift();

    //Copy constructor, assignement and stream insertion operator
    Airlift(Airlift& anAirlift);
    Airlift& operator =(const Airlift& anAirlift);
    friend ostream& operator<<(ostream&, const Airlift&);

    //Getters and setters
    string const getName() override;

    // For Assignment 3
    void setSelfPlayers(Player*);
    void setSourceTerritory(Territory*);
    void setTargetTerritory(Territory*);
    void setNumArmy(int);
    vector<Player*> updateListOfPlayers() override;

    //Member functions
    bool validate() override;
    void execute() override;

    string stringToLog() override;
private:
    int armies;
    Territory* source;
    Territory* target;
};

//A negotiate order targets an enemy player. It results in the target player and the player issuing
//the order to not be able to successfully attack each others� territories for the remainder of the turn.The negotiate
//order can only be created by playing the diplomacy card.
class Negotiate : public Order {
public:
    //Constructor and destructor
    Negotiate();
    Negotiate(Player*, Player*);    //player, enemyPlayer
    ~Negotiate();

    //Copy constructor, assignement and stream insertion operator
    Negotiate(Negotiate& aNegotiate);
    Negotiate& operator =(const Negotiate& aNegotiate);
    friend ostream& operator<<(ostream&, const Negotiate&);

    //Getters and setters
    string const getName() override;

    // For Assignment 3
    void setSelfPlayers(Player*);
    void setPeacePlayer(Player*);
    vector<Player*> updateListOfPlayers() override;

    //Member functions
    bool validate() override;
    void execute() override;

    string stringToLog() override;
private:
    Player* enemyPlayer;
};

class OrdersList : public Subject, public ILoggable {
public:
    //Constructor and destructor
    OrdersList();
    OrdersList(vector<Order*>);
    ~OrdersList();

    //Copy constructor, assignement and stream insertion operator
    OrdersList(OrdersList& anOrdersList);
    OrdersList& operator =(const OrdersList& anOrdersList);
    friend ostream& operator<<(ostream&, const OrdersList&);

    //Member functions
    void addToListOfOrders(Order*);
    void move(int, int);
    void remove(int);
    string stringToLog();
    void notifyAddOrder(Order* order);

    //private:
        //The OrdersList class contains a list of Order objects
    vector<Order*> listOfOrders;        //vector of pointers to Order object

};
