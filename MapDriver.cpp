#include <iostream>
#include "Map.h"
#include <map>

void MapDriver() {
    string name = "source_maps/sw_baltic.map";
    string name1 = "source_maps/bigeurope2.map";
    string name2 = "source_maps/bigeurope.map";
    string name3 = "source_maps/invalid.txt";

    // test if clear and load works
    Map* map = new Map(name);
    cout << map->GetMapTerritoriesNumber() << endl;
    map->clear();
    map->load(name2);
    cout << map->GetMapTerritoriesNumber() << endl;
    map->clear();
}
