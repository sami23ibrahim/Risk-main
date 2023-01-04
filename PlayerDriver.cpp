#include <iostream>
#include <string>
#include <vector>
#include "Player.h"
#include "Orders.h"
#include "Cards.h"
using namespace std;


void PlayerDriver(){

    int* id = new int(12345);

    string* n = new string("badgirlririrama");

    //creating arbitary list of territories
     Territory * t1 = new Territory();
     Territory* t2 = new Territory();
     vector<Territory*> territoryList = {t1, t2};


     // creating arbitary list of orders
     OrdersList* orderList = new OrdersList();
     //Order o1, o2;
     //Order* ptr1 = &o1;
     //Order* ptr2 = &o2;
     //orderList.addToListOfOrders(ptr1);
    // orderList.addToListOfOrders(ptr2);

     //creating arbitary card and then hand
     //Cards* y= new Cards();{};
     //Cards* z= new Cards();
     //x.setHand(y);
     //x.setHand(z);
     Hand* h = new Hand();

     

     std:: cout << "Creating player\n";
     Player* player1 = new Player( id, new int(10), n, territoryList, h, orderList);
     std::cout << *player1 << endl;

    //  testing toAttack()
     std::cout << "testing toAttack() method---------------------------------\n";
     player1->toAttack();
     cout << "Attack successful" << endl;


    //  testing toDefend()
     std::cout << "testing toDefend() method--------------------------------\n";
     player1->toDefend();
     cout << "Territory Defended" << endl;



       std::cout << "testing issueOrder() method---------------------------------\n";
       player1->issueOrder(); 
       cout << "Order issued" <<endl;
    
    
       std::cout << player1 << endl;
       //delete player that created
       delete player1;

       player1 = nullptr;

}

