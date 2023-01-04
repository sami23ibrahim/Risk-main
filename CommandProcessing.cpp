
#include "CommandProcessing.h"
#include "LoggingObserver.h"
#include "GameEngine.h"
#include <limits>
#include <vector>
#include <string>
#include <fstream>
#include <regex>
#include <stdlib.h>
#include <utility>
using namespace std;
// This free function split a string to several components according to the delimiter
//source: https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
vector<string> split(string s, string delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	string token;
	vector<string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}

//class CommandProcessor;
//================================ Command ========================================================================

// Constructors
Command::Command() {
	CommandName = "Default";
	CommandEffect = "Default";
	type = CommandType::none;
}

Command::Command(string commandInfo) : CommandName(commandInfo), CommandEffect("no Effect") {
	regex loadMapRegex("loadmap \\S+");
	regex addPlayerRegex("addplayer \\S+");
	regex addTournamentRegex("tournament \\*");
	// initialize command type  

	if (commandInfo.substr(0, 10) == "tournament") {
		type = CommandType::tournament;
	}

	//if (regex_match(commandInfo, addTournamentRegex)) {
	//	type = CommandType::tournament;
	//}
	else if( regex_match(commandInfo, loadMapRegex)){
		type = CommandType::loadmap;
	}
	else if (commandInfo=="validatemap") {
		type = CommandType::validatemap;
	}
	else if (regex_match(commandInfo, addPlayerRegex)) {
		type = CommandType::addplayer;
	}
	else if (commandInfo=="gamestart") {
		type = CommandType::gamestart;
	}
	else if (commandInfo=="replay") {
		type = CommandType::replay;
	}
	else if (commandInfo=="quit") {
		type = CommandType::quit;
	}
	else {
		type = CommandType::none;
	//	cout << "Invalid command!" << endl;
	}
}

// Destructor
Command::~Command() {
}

//Assignment operator
Command& Command::operator =(const Command& other) {
	CommandName = other.CommandName;
	CommandEffect = other.CommandEffect;
	type = other.type;
	return *this;
}

Command::Command(const Command& other) {
	CommandName = other.CommandName;
	CommandEffect = other.CommandEffect;
	type = other.type;
}
//Stream insertion

ostream& operator << (ostream& out, const Command& c) {
	out << "The command is " << c.CommandName << " and its Effect is " << c.CommandEffect << endl;
	return out;
}
string Command::GetCommandName() {
	return CommandName;
}
//  saveEffect() method can be used to save the effect of the command as a string in the Command object.
void Command::saveEffect(string effect) {
	CommandEffect = effect;
	Notify(this);              //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=>>>>>>> part 5
}
//virtual method inherited from Subject class
string Command::stringToLog() {//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=>>>>>>> part 5
	return "Effect: " + CommandEffect;
}
//-------------------------------------------------- CommandProcessor ((target))-----------------------------------------------
//Constructors
CommandProcessor::CommandProcessor(){
}
CommandProcessor::CommandProcessor(const CommandProcessor& other) {
	for (auto p : other.commandObjects) {
		Command* temp = new Command(*p);
		commandObjects.push_back(temp);
	}
}
//Destructor
CommandProcessor::~CommandProcessor() {
	for (auto p : commandObjects) {
		delete p;
	}
	commandObjects.clear();
}

//Assignment operator

CommandProcessor& CommandProcessor::operator =(const CommandProcessor& other) {
	for (auto p : other.commandObjects) {
		Command* temp = new Command(*p);
		commandObjects.push_back(temp);
	}
	return *this;
}
// readCommand() reads a string from the console and
// stores the command internally in a collection of Command objects using the saveCommand() method




string CommandProcessor::readCommand() {
	//cout << "Please enter a command: " << endl;
	cout <<  endl << ":: Enter a command -> " << endl<< endl;
	string input;
	getline(cin, input);
	Command* command = new Command(input);
	//validate(command);
	saveCommand(command);
	return input;
}

// saves the string of the command inside the command itself
// and then puts it into the collection of commands.
void CommandProcessor::saveCommand(Command* command) {
	commandObjects.push_back(command);
	for (auto observer : *observers) {
        command->Attach(observer);
  }
	Notify(this);//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=>>>>>>> part 5

}
// a public method to other objects such as the GameEngine or the Player.
Command* CommandProcessor::getCommand() {
	readCommand();
	return commandObjects.back();
}
void CommandProcessor::printAllSavedCommands() {
	cout << "THE SAVED COMMANDS ARE : " << endl;
	for (Command* c : commandObjects) {
		cout << "Command Name: " << c->CommandName << " ,Command Effect: " << c->CommandEffect << endl;
	}
}
// validate() checks if a certain command has been entered is a valid command in the current state of
// the game engine. if its not valid the error msg will be saved in the effect of the command.

bool CommandProcessor::validate(Command* command) {
    	if (command->type == Command::CommandType::loadmap) {
		if (gameEnginePtr->GetState() ==   start ||gameEnginePtr->GetState() == maploaded) {
			command->saveEffect("maploaded");
			cout << endl<< command->CommandEffect;
			return true;
		}
		else
		command->saveEffect("Command (" + command->CommandName + ") is not a valid command in the current state of the game");
		cout << endl << command->CommandEffect;
		return false;
	}
		else if (command->type == Command::CommandType::tournament) {
			if (gameEnginePtr->GetState() == start ) {
				command->saveEffect("Tournament Mode activated.");
				cout << endl << command->CommandEffect;
				return true;
			}
			else
				command->saveEffect("Command (" + command->CommandName + ") is not a valid command in the current state of the game");
				cout << endl << command->CommandEffect;
			return false;
		}
    	else if (command->type == Command::CommandType::addplayer) {
		if (gameEnginePtr->GetState() == mapvalidated || gameEnginePtr->GetState() == playersadded) {
			command->saveEffect("playersadded");
		//	cout << endl << command->CommandEffect;
			return true;
		}
		else
		command->saveEffect("Command (" + command->CommandName + ") is not a valid command in the current state of the game");
		cout << endl << command->CommandEffect;
		return false;
	}
	else if (command->type == Command::CommandType::validatemap) {
		if (gameEnginePtr->GetState() == maploaded) {
			command->saveEffect("mapvalidated");
			cout << endl << command->CommandEffect;
		
			return true;
		}
		else
		command->saveEffect("Command (" + command->CommandName + ") is not a valid command in the current state of the game");
		cout << endl << command->CommandEffect;
		return false;
	}
	else if (command->type == Command::CommandType::gamestart) {
		if (gameEnginePtr->GetState() ==playersadded) {
			command->saveEffect("assignreinforcement");
			// cout << endl << command->CommandEffect;
			return true;
		}
		else
		command->saveEffect("Command (" + command->CommandName + ") is not a valid command in the current state of the game");
		cout << endl << command->CommandEffect;
		return false;
	}
	else if (command->type == Command::CommandType::replay) {
		if (gameEnginePtr->GetState() == win) {
			command->saveEffect("start");
			cout << endl << command->CommandEffect;
			commandObjects.push_back(command);
		return true;
		}
		else
		command->saveEffect("Command (" + command->CommandName + ") is not a valid command in the current state of the game");
		cout << endl << command->CommandEffect;
		return false;
	}
	else if (command->type == Command::CommandType::quit) {
		if (true) {
			command->saveEffect("exit program");
			cout << endl << command->CommandEffect;
			exit(1);
			return true;
		}
		else
		command->saveEffect("Command (" + command->CommandName + ") is not a valid command in the current state of the game");
		cout << endl << command->CommandEffect;
		return false;
	}
	if (command->CommandName != "maploaded" || command->CommandName != "validatemap" || command->CommandName != "addplayer" ||
		command->CommandName != "gamestart" || command->CommandName != "replay" || command->CommandName != "quit")
	{
		command->saveEffect("Command (" + command->CommandName + ") is not a valid command!!");
		cout << endl << command->CommandEffect;
		return false;
	}
	return false;
}
//0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
void CommandProcessor::TournamentFunctionInput(string input) {
	vector<string> enteredTournamentString = split(input, " ");
	int i = 1; // skip the first word "tournament"
	while (i < enteredTournamentString.size()) {
		if (enteredTournamentString[i] == "-M") {
			while (enteredTournamentString[++i] != "-P") {
				allMaps.push_back(enteredTournamentString[i]);

			}
		}
		else if (enteredTournamentString[i] == "-P") {
			while (enteredTournamentString[++i] != "-G") {
				allPlayerStrategies.push_back(enteredTournamentString[i]);
			}

		}
		else if (enteredTournamentString[i] == "-G") {
			i++;
			string temp;
			temp = enteredTournamentString[i++];
			if (!isdigit(temp[0])) {
				cout << "The number of games has to be a digit" << endl;
				exit(0);
			}
			numberOfGames = stoi(temp);

		}
		else if (enteredTournamentString[i] == "-D") {
			i++;
			string temp;
			temp = enteredTournamentString[i++];
			if (!isdigit(temp[0])) {
				cout << "The max number of turns has to be a digit" << endl;
				exit(0);
			}
			maxNumberOfTurns = stoi(temp);
		}
	}

	// output to check result
	cout << ">> MAPS ENTERED <<" << endl;
	for (string s : allMaps)
		cout << s <<endl;
	cout << endl;
	cout << ">> PLAYER STRATEGIES ENTERED <<" << endl;
	for (string s : allPlayerStrategies)
		cout << s <<endl;
	cout << endl;
	cout << ">> NUMBER OF GAMES <<" << endl<< numberOfGames << endl;
	cout << ">> MAX NUMBER OF TURNS <<" << endl << maxNumberOfTurns << endl;
	cout << endl;
}

bool CommandProcessor::TournamentValidation() {
	bool allGood = true;
	if (allMaps.size() < 1 || allMaps.size() > 5) {
		cout << "Please enter 1-5 maps" << endl;
		allGood = false;
	}
	if (allPlayerStrategies.size() < 2 || allPlayerStrategies.size() > 4) {
		cout << "Please enter 2-4 players strategies" << endl;
		allGood = false;
	}
	if (numberOfGames < 1 || numberOfGames > 5) {
		cout << "Please enter a number among 1,2,3,4,5 for number of games" << endl;
		allGood = false;
	}
	if (maxNumberOfTurns < 10 || maxNumberOfTurns > 50) {
		cout << "Please enter a number between 10 and 50 for max number of turns" << endl;
		allGood = false;
	}
	// validate strategy
	string strategies[5] = { "Aggressive" , "Benevolent" , "Neutral", "Cheater", "Human" };
	int invalidStrategyCounter = 0;
	//bool isStrategyValid = false;
	for (int i = 0; i < allPlayerStrategies.size(); i++) {
		for (int j = 0; j < 5; j++) {
			if (allPlayerStrategies[i] == strategies[j]) {
				break;
			//isStrategyValid = true;
			}
			else if (allPlayerStrategies[i] != strategies[j]&& j==4) {
				cout << allPlayerStrategies[i] +" X NOT VALID" << endl;
				invalidStrategyCounter++;
			}
		}
	}
	if (invalidStrategyCounter>0) {
		cout << invalidStrategyCounter;
		cout << " Players Strategies entered are NOT valid" << endl;
		allGood = false;
	}
	return allGood;
}
//0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000



string CommandProcessor::stringToLog() {                      //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=>>>>>>> part 5
	if (!commandObjects.empty())
		return "Command: " + commandObjects.back()->getCommandText();
	return "error occurred";
}
//================================= FileLineReader((adaptee)) =====================================================

//Constructors
FileLineReader::FileLineReader(const string& filename) {
	ifstream inFile(filename);
	this->fileName = filename;
	if (!inFile) {
		cout << "File does not exist or cannot be opened.\n";
		cout << "Program will terminate.\n";
		//exit(0);
	}
	else cout << "Reading Commands from file: "+ filename << endl;
}
//Destructor
FileLineReader::~FileLineReader() {
}
FileLineReader::FileLineReader(const FileLineReader& other) {
	fileName = other.fileName;
}
//Assignment operator
FileLineReader& FileLineReader::operator =(const FileLineReader& other) {
	fileName = other.fileName;
	return *this;
}
//Stream insertion
ostream& operator << (ostream& out, const FileLineReader& flr) {
	out << flr.fileName << endl;
	return out;
}
// This method reads a file line by line
 void FileLineReader::readLineFromFile(string fn) {
	ifstream ifs;
	ifs.open(fn);
	if (!ifs.eof()) {
		string line;
		while (getline(ifs, line)) {
			commandObjects2.push_back(line);
		}
	}ifs.close();
}



 string CommandProcessor::FileTournamentFunctionInput(string input) {
	 ifstream ifs;
	 string line;
	 ifs.open(input);
	 if (!ifs.eof()) {
		 getline(ifs, line);
		 TournamentFunctionInput(line);
	 }
	 ifs.close();
	 return line;
 }

//============================== FileCommandProcessorAdapter ((adapter))=====================================================
// Constructors
FileCommandProcessorAdapter::FileCommandProcessorAdapter() {
}
FileCommandProcessorAdapter::FileCommandProcessorAdapter(FileLineReader* flr)
{ this->flrReference = flr; }

//Destructor
FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
}
//overriding the readCommand()
string FileCommandProcessorAdapter::readCommand() {
		flrReference->readLineFromFile(flrReference->fileName);
	 for (string c : flrReference->commandObjects2) {
		 Command* command = new Command(c);
		 validate(command);
		 saveCommand(command);
}
	return "done";
}
bool FileCommandProcessorAdapter::validate(Command* command) {
	if (command->type == Command::CommandType::loadmap) {
		if (gameEnginePtr->GetState() == start || gameEnginePtr->GetState() == maploaded) {
			cout << " => maploaded" << endl;
			command->saveEffect("maploaded");
			return true;
		}
		else
			cout << " => Command (" + command->CommandName + ") is not a valid command in the current state of the game" << endl;
		command->saveEffect("Command (" + command->CommandName + ") is not a valid command in the current state of the game");
		return false;
	}
	else if (command->type == Command::CommandType::addplayer) {
		if (gameEnginePtr->GetState() == mapvalidated || gameEnginePtr->GetState() == playersadded) {
			command->saveEffect("playersadded");
			return true;
		}
		else
			command->saveEffect("Command (" + command->CommandName + ") is not a valid command in the current state of the game");
		return false;
	}
	else if (command->type == Command::CommandType::validatemap) {
		if (gameEnginePtr->GetState() == maploaded) {
			command->saveEffect("mapvalidated");
			return true;
		}
		else
			command->saveEffect("Command (" + command->CommandName + ") is not a valid command in the current state of the game");
		return false;
	}
	else if (command->type == Command::CommandType::gamestart) {
		if (gameEnginePtr->GetState() == playersadded) {
			command->saveEffect("assignreinforcement");
			return true;
		}
		else
			command->saveEffect("Command (" + command->CommandName + ") is not a valid command in the current state of the game");
		return false;
	}
	else if (command->type == Command::CommandType::replay) {
		if (gameEnginePtr->GetState() == win) {
			command->saveEffect("start");
			commandObjects.push_back(command);
			return true;
		}
		else
			command->saveEffect("Command (" + command->CommandName + ") is not a valid command in the current state of the game");
		return false;
	}
	else if (command->type == Command::CommandType::quit) {
		if (gameEnginePtr->GetState() == win) {
			command->saveEffect("exit program");
			return true;
		}
		else
			command->saveEffect("Command (" + command->CommandName + ") is not a valid command in the current state of the game");
		return false;
	}
	if (command->CommandName != "maploaded" || command->CommandName != "validatemap" || command->CommandName != "addplayer" ||
		command->CommandName != "gamestart" || command->CommandName != "replay" || command->CommandName != "quit")
	{
		cout << "Command (" + command->CommandName + ") is not a valid command!!" << endl;
		command->saveEffect("Command (" + command->CommandName + ") is not a valid command!!");
		return false;
	}
	return false;
}

