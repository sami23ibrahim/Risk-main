#pragma once

#include <iostream>
#include <fstream>
#include<vector>
#include<string>
#include "LoggingObserver.h"

using namespace std;
class GameEngine; //((client)
class ILoggable;
class Observer;

//================================ Command ========================================================================
class Command : public ILoggable, public Subject
{
public:
	enum class CommandType { tournament,loadmap, validatemap, addplayer, gamestart, replay, quit, none };
	string CommandName;
	string CommandEffect;
	CommandType type;
	GameEngine* gameEnginePtr;
	//Constructor
	Command();
	Command(string commandInfo);
	Command(const Command& other);
	string getCommandText() { return CommandName; }
	//Destructor
	~Command();
	void saveEffect(string command);
	//Assignment operator
	Command& operator =(const Command& other);
	//Stream insertion
	friend ostream& operator << (ostream& out, const Command& c);
	string GetCommandName();
	string GetCommandType();
	friend void CommandProcessorDriver();
	// forced implementation of ILoggable interface's function
	string stringToLog();
};
//--------------------- CommandProcessor ((target))-----------------------
//reads commands from the console
class CommandProcessor : public ILoggable, public Subject
{
protected:

	virtual string readCommand();
	void saveCommand( Command* command);// saves the string of the command inside the command itself and then
public:
	//000000000000000000000000000000000000000000000000000000000

	vector <string> allMaps;
	vector <string> allPlayerStrategies;
	int numberOfGames = 0;
	int maxNumberOfTurns = 0;

	void TournamentFunctionInput(string);
	bool TournamentValidation();

	string FileTournamentFunctionInput(string input);
	//000000000000000000000000000000000000000000000000000000000
	friend void CommandProcessorDriver();
	GameEngine* gameEnginePtr;
	vector<Command*> commandObjects;
	//Constructor and Destructor
	CommandProcessor();
	CommandProcessor(const CommandProcessor& other);
	~CommandProcessor();
	//Assignment operator and Stream insertion
	CommandProcessor& operator =(const CommandProcessor& other);
	friend ostream& operator << (ostream& out, const CommandProcessor& cp);
	Command* getCommand();// uses readCommand() that gets a string from the user.
	bool validate(Command* comand);
	void printAllSavedCommands();
	// forced implementation of ILoggable interface's function
	string stringToLog();
};
//================================= FileLineReader((adaptee)) =====================================================

class FileLineReader
{
public:
	GameEngine* gameEnginePtr;
	vector<string> commandObjects2;
	string fileName;
	FileLineReader();
	FileLineReader(const string& filename);
	FileLineReader(const FileLineReader& other);
	~FileLineReader();
	//Assignment operator
	FileLineReader& operator =(const FileLineReader& other);
	//Stream insertion
	friend ostream& operator << (ostream& out, const FileLineReader& f);
	friend void CommandProcessorDriver();
	//Method that reads one line(command) from file
	void readLineFromFile(string fn);

};
//============================== FileCommandProcessorAdaptor ((adapter))=====================================================
class FileCommandProcessorAdapter :public CommandProcessor
{
public:
	GameEngine* gameEnginePtr;
	//Constructors
	FileCommandProcessorAdapter();
	FileCommandProcessorAdapter(FileLineReader* flr);
	FileCommandProcessorAdapter(const FileCommandProcessorAdapter& other);
	//Destructor
	~FileCommandProcessorAdapter();
	friend void CommandProcessorDriver();
	bool validate(Command* comand);
protected:
	 string readCommand();
private:
	// Reference to the Adaptee:
	FileLineReader* flrReference;
};
