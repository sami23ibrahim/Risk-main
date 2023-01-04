#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <ctype.h>
#include <algorithm>
#include "Map.h"

using namespace std;
//---------------------Territory-----------------------
Territory::~Territory() { 
    ownerID = -1;
    armies = 0;
    id = -1;
    name = "";             	// territory name.

    continentName = "";
   // adjTerritories.clear();
   // delete player;

} // map will handle memory

Territory::Territory() {
  ownerID = -1;
  armies = 0;
  id = -1;
  name = "";
  continentName = "";
}

Territory::Territory(int id, string name, string continentName) {
  ownerID = -1;
  armies = 0;
  this->id = id;
  this->name = name;
  this->continentName = continentName;
}

Territory::Territory(const Territory& other) {
  ownerID = other.ownerID;
  armies = other.armies;
  id = other.id;
  name = other.name;
  continentName = other.continentName;
  for (Territory* t : other.adjTerritories) {
    adjTerritories.push_back(t); // shallow copy
  }
}

Territory& Territory::operator=(const Territory& rhs) {
  ownerID = rhs.ownerID;
  armies = rhs.armies;
  id = rhs.id;
  name = rhs.name;
  continentName = rhs.continentName;
  adjTerritories.clear();
  for (Territory* t : rhs.adjTerritories) {
    adjTerritories.push_back(t);
  }
  return *this;
}

bool operator==(const Territory& t1, const Territory& t2) {
    return t1.id == t2.id;
}

ostream& operator<<(ostream& out, const Territory& territory) {
  out << "Territory: " << territory.name << " (" << territory.id << ")" << endl;
  cout << "Continent: " << territory.continentName << endl;
  out << "Armies: " << territory.armies << endl;
  out << "Owned by player " << territory.ownerID << endl;
  return out;
}

void Territory::addAdjTerritory(Territory* t) {
    adjTerritories.push_back(t);
}

// getter and setters
void Territory::setOwnerId(int playerId) { this->ownerID = playerId; }
int Territory::getOwnerId() { return ownerID; }
int Territory::getId() const { return id; }
int Territory::getPlayerId() const { return ownerID; }
int Territory::getArmies() const { return armies; }
string Territory::getContinent() { return continentName; }
string Territory::getName() const { return name; }
vector<Territory*> Territory::getAdjTerritories() const {
    return adjTerritories;
}

// Added by Justine & Jennifer
bool Territory::isAdjacentTo(int territoryID) {
    bool adjacent = false;
    for (int i = 0; i < adjTerritories.size(); i++) {
        if (territoryID == adjTerritories.at(i)->getId()) {
            adjacent = true;
        }
    }
    return adjacent;
}

void Territory::addArmies(int numberOfArmies) {
    if (numberOfArmies > 0) {
        armies = armies + numberOfArmies;
    }
    else {
        cout << "Cannot add negative or zero number of armies" << endl;
    }
}

void Territory::removeArmies(int numberOfArmies) {
    if (numberOfArmies > 0) {
        armies = armies - numberOfArmies;
    }
    else {
        cout << "Cannot remove negative or zero number of armies" << endl;
    }
}

Player* Territory::getPlayerOwner()
{
    return player;
}

void Territory::setPlayerOwner(Player* p)
{
    player = p;
}


//----------------------Continent--------------------------
Continent::~Continent() { } // map will handle memory

Continent::Continent() {
    this->armyValue = -1;
    this->id = -1;
    this->name = "";
}

Continent::Continent(int armyValue, int id, string name) {
    this->armyValue = armyValue;
    this->id = id;
    this->name = name;
}

Continent::Continent(const Continent& other) {
    armyValue = other.armyValue;
    id = other.id;
    name = other.name;
    for (Territory* t : other.territories) {
        territories.push_back(t);
    }
}

Continent& Continent::operator=(const Continent& rhs) {
    armyValue = rhs.armyValue;
    id = rhs.id;
    name = rhs.name;
    for (Territory* t : rhs.territories) {
        territories.push_back(t);
    }
    return *this;
}

ostream& operator<<(ostream& out, const Continent& continent) {
    out << "Continent: " << continent.name << " (" << continent.id << ")" << endl;
    out << "Army value: " << continent.armyValue << endl;
    out << "Territories: " << endl << endl;
    for (int i = 0; i < continent.territories.size(); ++i) {
        if (i == continent.territories.size() - 1) out << *(continent.territories[i]);
        else out << *(continent.territories[i]) << endl << endl;
    }
    return out;
}

void Continent::addTerritory(Territory* t) {
    territories.push_back(t);
}

bool Continent::contains(Territory* t) const {
    return find(territories.begin(), territories.end(), t) != territories.end();
}

// getters and setters
int Continent::getId() const { return id; }
int Continent::getArmyValue() const { return armyValue; }
string Continent::getName() const { return name; }
vector<Territory*> Continent::getTerritories() const { return territories; }

//-----------------------------Map---------------------------
void Map::clear() {
    for (Territory* t : territories) delete t;
    for (Continent* c : continents) delete c;
    territories.clear();
    continents.clear();
    mapLoader->clear();
    numTerritories = 0;
    isLoaded = false;
    cout << "Map cleared" << endl;
}

Map::~Map() {
    for (Territory* t : territories) delete t;
    for (Continent* c : continents) delete c;
    delete mapLoader;
}

Map::Map(const string& fileName) {
    mapLoader = new MapLoader();
    load(fileName);
}

Map::Map() {
    mapLoader = new MapLoader();
}

Map::Map(const Map& other) {
    if (!other.isLoaded) {
        numTerritories = 0;
        name = "N/A";
        isLoaded = false;
        mapLoader = new MapLoader();
        return;
    }
    mapLoader = new MapLoader(*(other.mapLoader));
    load();
}

Map& Map::operator=(const Map& rhs) {
    // clearing memory
    clear();
    delete mapLoader;
    if (!rhs.isLoaded) {
        mapLoader = new MapLoader();
        return *this;
    }
    mapLoader = new MapLoader(*(rhs.mapLoader));
    load();
    return *this;
}

bool contains(vector<Territory*>& territories, Territory* t) {
    auto begin = territories.begin();
    auto end = territories.end();
    return find(begin, end, t) != end;
}

bool Map::addTerritoryToContinent(int continentId, Territory* t) const {
    for (Continent* c : continents) {
        if (c->getId() == continentId) {
            c->addTerritory(t);
            return true;
        }
    }
    // territory's continent does not exist, map file is invalid
    return false;
}

void Map::addTerritory(Territory* t) {
    territories.push_back(t);
}

void Map::addContinent(Continent* t) {
    continents.push_back(t);
}

Territory* Map::findTerritory(int id) const {
    for (Territory* t : territories) {
        if (t->getId() == id) return t;
    }
    return nullptr;
}

Territory* Map::findTerritory(const string& name) const {
    for (Territory* t : territories) {
        if (t->getName() == name) return t;
    }
    return nullptr;
}

void Map::validateGraph(vector<Territory*>& visited, Territory* t) const {
    // DFS graph traversal
    if (contains(visited, t)) return;
    visited.push_back(t);
    auto adjTerritories = t->getAdjTerritories();
    for (auto adjTerritory : adjTerritories) {
        validateGraph(visited, adjTerritory);
    }
}

void Map::validateContinents(vector<Territory*>& visited, Territory* t, Continent* c) const {
    // DFS graph traversal limited within the continent subgraph
    if (contains(visited, t) || !(c->contains(t))) return;
    visited.push_back(t);
    auto adjTerritories = t->getAdjTerritories();
    for (auto adjTerritory : adjTerritories) {
        validateContinents(visited, adjTerritory, c);
    }
}

bool Map::validateTerritories() const {
    // for each continent, we visit all territories
    // if one territory is already visited, that means there is a duplicate territory meaning there is not a one-one relationship
    vector<Territory*> visited;
    for (Continent* c : continents) {
        auto territories = c->getTerritories();
        for (auto t : territories) {
            if (contains(visited, t)) return false;
            visited.push_back(t);
        }
    }
    return true;
}

bool Map::validate() const {
    if (!isLoaded) return false;
    bool isGraphConnected = false;
    bool areContinentsValid = false;
    vector<Territory*> visited;
    // checking if map is a connected graph
    validateGraph(visited, territories[0]);
    if (visited.size() == numTerritories) isGraphConnected = true;
    else return false;
    visited.clear();
    // checking if continents are connected subgraphs
    for (Continent* c : continents) {
        auto t = c->getTerritories()[0];
        validateContinents(visited, t, c);
        if (visited.size() == c->getTerritories().size()) areContinentsValid = true;
        else return false;
        visited.clear();
    }
    return isGraphConnected && areContinentsValid && validateTerritories();
}

void Map::load(const string& fileName) {
    mapLoader->loadMap(this, fileName);
}

void Map::load() {
    mapLoader->loadMap(this);
}


vector<Territory*> Map::GetMapTerritories() {
    return territories;
}

int Map::GetMapTerritoriesNumber() {
    return territories.size();
}

//---------------------------Map loader----------------------
MapLoader::MapLoader() {
    mapName = "N/A"; // default name
}

bool MapLoader::readMap(const string& fileName) {
    string line = "";
    fstream fileObj(fileName);
    if (!fileObj.is_open()) {
        cerr << "Could not open " << fileName << endl << endl;
        return false;
    }
    // flags to keep track of which section we are reading
    bool isContinent = false;
    bool isCountry = false;
    bool isBorder = false;
    while (getline(fileObj, line)) {
        if (line[0] == ';') continue; // ignore comments
        if (line.find("name") == 0) {
            // name of the map (optional)
            mapName = line;
            continue;
        }
        if (line.find("[continents]") == 0) {
            isContinent = true;
            isCountry = false;
            isBorder = false;
            continue;
        }
        if (line.find("[countries]") == 0) {
            isCountry = true;
            isContinent = false;
            isBorder = false;
            continue;
        }
        if (line.find("[borders]") == 0) {
            isBorder = true;
            isCountry = false;
            isContinent = false;
            continue;
        }
        // ignore whitespaces
        if (line == "\r" || line == "") continue;
        if (isContinent) continents.push_back(line);
        if (isCountry) territories.push_back(line);
        if (isBorder) borders.push_back(line);
    }
    fileObj.close();
    return true;
}

void MapLoader::clear() {
    mapName = "N/A";
    borders.clear();
    continents.clear();
    territories.clear();
}

void MapLoader::loadMap(Map* map) {
    // data is extracted from each line of each section (continents, countries and borders) and used to create the map
     // check if map contnet is valid
    if (continents.size() == 0 || territories.size() == 0 || borders.size() == 0 || borders.size() != territories.size()) {
        map->clear();
        return;
    }
    map->name = mapName;
    map->numTerritories = territories.size();
    for (int i = 0; i < continents.size(); ++i) {
        // initializing continents
        istringstream iss(continents[i]);
        string name = "";
        int armyValue = 0;
        int id = i + 1;
        iss >> name >> armyValue; // reading the first 2 tokens in the string
        map->addContinent(new Continent(armyValue, id, name));
    }
    for (const string& territory : territories) {
        // initializing territories, and adding territories to continents
        istringstream iss(territory);
        int id = 0;
        string name = "";
        int continentId = 0;
        iss >> id >> name >> continentId; // reading first 3 tokens
        Territory* t = new Territory(id, name, map->getContinentNameById(continentId));
        map->addTerritory(t);
        if (!map->addTerritoryToContinent(continentId, t)) {
            map->clear();
            return;
        }
    }
    for (const string& border : borders) {
        // adding all the adj territories to each territory
        istringstream iss(border);
        int id = 0;
        int adjId = 0;
        iss >> id; // reading first token
        Territory* t = map->findTerritory(id);
        // reading the rest
        while (iss >> adjId) {
            Territory* adjT = map->findTerritory(adjId);
            t->addAdjTerritory(adjT);
        }
    }
    map->isLoaded = true;
}

void MapLoader::loadMap(Map* map, const string& fileName) {
    // data is extracted from each line of each section (continents, countries and borders) and used to create the map
    // check if map can be opened
    if (!readMap(fileName)) {
        map->clear();
        return;
    }
    // check if map content is valid
    if (continents.size() == 0 || territories.size() == 0 || borders.size() == 0 || borders.size() != territories.size()) {
        map->clear();
        return;
    }
    map->name = mapName;
    map->numTerritories = territories.size();
    for (int i = 0; i < continents.size(); ++i) {
        // initializing continents
        istringstream iss(continents[i]);
        string name = "";
        int armyValue = 0;
        int id = i + 1;
        iss >> name >> armyValue; // reading the first 2 tokens in the string
        map->addContinent(new Continent(armyValue, id, name));
    }
    for (const string& territory : territories) {
        // initializing territories, and adding territories to continents
        istringstream iss(territory);
        int id = 0;
        string name = "";
        int continentId = 0;
        iss >> id >> name >> continentId; // reading first 3 tokens
        Territory* t = new Territory(id, name, map->getContinentNameById(continentId));
        map->addTerritory(t);
        // if territory's continent does not exist, invalid map
        if (!map->addTerritoryToContinent(continentId, t)) {
            map->clear();
            return;
        }
    }
    for (const string& border : borders) {
        // adding all the adj territories to each territory
        istringstream iss(border);
        int id = 0;
        int adjId = 0;
        iss >> id; // reading first token
        Territory* t = map->findTerritory(id);
        // reading the rest
        while (iss >> adjId) {
            Territory* adjT = map->findTerritory(adjId);
            t->addAdjTerritory(adjT);
        }
    }
    map->isLoaded = true;
}

MapLoader::MapLoader(const MapLoader& other) {
    mapName = other.mapName;
    for (auto i : other.borders) borders.push_back(i);
    for (auto i : other.continents) continents.push_back(i);
    for (auto i : other.territories) territories.push_back(i);
}

MapLoader& MapLoader::operator=(const MapLoader& rhs) {
    mapName = rhs.mapName;
    borders.clear();
    continents.clear();
    territories.clear();
    for (auto i : rhs.borders) borders.push_back(i);
    for (auto i : rhs.continents) continents.push_back(i);
    for (auto i : rhs.territories) territories.push_back(i);
    return *this;
}

ostream& operator<<(ostream& out, const MapLoader& mapLoader) {
    out << mapLoader.mapName;
    return out;
}

ostream& operator<<(ostream& out, const Map& map) {
    for (int i = 0; i < map.continents.size(); ++i) {
        if (i == map.continents.size() - 1) out << *(map.continents[i]);
        else out << *(map.continents[i]) << endl << "-------------------------" << endl;
    }
    return out;
}

Map* MapLoader::loadMap(const string& filename) {
    return new Map(filename);
}

bool Map::isMapLoaded() {
    return isLoaded;
}

string Map::getContinentNameById(int id) {
    for (auto continent : continents) {
        if (id == continent->getId()) return continent->getName();
    }
    return "N/A";
}