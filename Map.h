#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
using namespace std;

class Territory;
class Continent;
class Map;
class MapLoader;
class Player;

void MapDriver();

//---------------------Territory-----------------------
class Territory {
	friend class MapLoader;
	friend class Map;
  string name;             	// territory name.
  int id;                   // territory ID.
  int ownerID;             // territory owned by player with this id
  int armies;               // # of armies
	string continentName;
	vector<Territory*> adjTerritories;
	Player* player; // Added by Justine & Jennifer
	/* constructors */
public:
	void addAdjTerritory(Territory*);				// Moved by Justine & Jennifer
	~Territory();									// Destructor
	Territory(); 									// Default Constructor
	Territory(int, string, string);  						// Constructor
	Territory(const Territory&);					// Copy Constructor
	Territory& operator=(const Territory&);
	friend bool operator==(const Territory&, const Territory&);
  friend ostream& operator<<(ostream&, const Territory&);
	void setOwnerId(int);
	int getOwnerId(); // to compare to owner of land 
	int getId() const;
	int getPlayerId() const;
	int getArmies() const;
	string getName() const;
	vector<Territory*> getAdjTerritories() const;

	string getContinent(); // added for reinforcement phase

	// Added by justine & Jennifer
	bool isAdjacentTo(int);
	void addArmies(int);
	void removeArmies(int);
	Player* getPlayerOwner();
	void setPlayerOwner(Player* p);
};

//----------------------Continent--------------------------
class Continent {
	friend class MapLoader;
	friend class Map;
	int id;
	int armyValue;
	string name;
	vector<Territory*> territories;
	void addTerritory(Territory*);
	/* constructors */
public:
	~Continent();                      // Destructor
	Continent();					   // Default Constructor
	Continent(int, int, string);       // Constructor
	Continent(const Continent&);       // Copy Constructor
	Continent& operator=(const Continent&);
	friend ostream& operator<<(ostream&, const Continent&);
	bool contains(Territory*) const;
	int getId() const;
	int getArmyValue() const;
	string getName() const;
	vector<Territory*> getTerritories() const;
};

//-----------------------------Map---------------------------
class Map {
	friend class MapLoader;
	MapLoader* mapLoader;
	vector<Continent*> continents; // sub graphs
	vector<Territory*> territories;
	string name;
	int numTerritories;
	bool isLoaded;
	bool addTerritoryToContinent(int, Territory*) const;
	void addContinent(Continent*);
	void addTerritory(Territory*);
	void validateGraph(vector<Territory*>&, Territory*) const;
	void validateContinents(vector<Territory*>&, Territory*, Continent*) const;
	bool validateTerritories() const; // check if each territory belong to only one continent
	string getContinentNameById(int id);
	friend bool contains(vector<Territory*>&, Territory*); // helper function for validating
	void load(); // load from existing maploader
public:
	~Map();                      // Destructor
	Map(const string&);
	Map();
	Map(const Map&);             // Copy Constructor 
	Map& operator=(const Map&);  // Assignment operator Overloading
	friend ostream& operator<<(ostream&, const Map&);
	Territory* findTerritory(int) const;
	Territory* findTerritory(const string&) const;
	bool validate() const;
	void load(const string&);
	bool isMapLoaded();
	vector<Territory*> GetMapTerritories();
	int GetMapTerritoriesNumber();
	void clear(); // delete everything
};

//---------------------------Map loader--------------------------
// maploader will first extract the 3 sections (borders, continents and territories) of the file into 3 vectors of string which is then processed and used to initialize the map
class MapLoader {
public:
	friend class Map;
	MapLoader();
	MapLoader(const MapLoader&);
	MapLoader& operator=(const MapLoader&);
	friend ostream& operator<<(ostream&, const MapLoader&);
	bool readMap(const string&);
	void loadMap(Map*, const string&);
	void loadMap(Map*); // loading map from existing maploader
	Map* loadMap(const string&);
	void clear();
	string mapName;
private:
	vector<string> borders;
	vector<string> continents;
	vector<string> territories;
};

#endif