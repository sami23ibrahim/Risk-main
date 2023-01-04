
#include <iostream>
#include <string>
#include "CommandProcessing.h"
#include "GameEngine.h"
using namespace std;
void CommandProcessorDriver() {
    CommandProcessor commandProcessorObj;
    commandProcessorObj.gameEnginePtr = new GameEngine();
    FileCommandProcessorAdapter* adaptor;



    //Cheater Neutral Aggressive Benevolent
    commandProcessorObj.TournamentFunctionInput("tournament -M map1 map2 map3 map4 map5 map6 -P Aggressive sami wrong blabla the5th -G 2 -D 100");

    commandProcessorObj.TournamentValidation();
    
    cout << endl;
    cout << "Type >> console << to use the (-console)" << endl;
    cout << "Or type the >> file name << that you want to use (-file)." << endl;
    string userChoice;
    cout << endl;
    getline(cin, userChoice);
    //-----------------------------------------------------
    //
        //************ READING FROM CONSOLE readCommand();************************
    if (userChoice == "console") {
        cout << "---------------------------------------------------------------------------"<< endl;
        cout << ">> testing readCommand() Method FROM CONSOLE << AND >> validate() METHOD <<" << endl;
        cout << "---------------------------------------------------------------------------" << endl;
       // STATE:          ITS VALID COMMANDS:

        //start:          loadmap mapName.
        //maploaded:      loadmap mapName  or validatemap.
        //mapvalidated:   addplayer playerName.
        //playersadded:   addplayer playerName or gamestart.
        //win:            quit or replay.

        cout << "The Current Game State Is: >>START<<" << endl;
        commandProcessorObj.gameEnginePtr->SetState(start);
        commandProcessorObj.readCommand();
        commandProcessorObj.validate(commandProcessorObj.commandObjects.back());
        commandProcessorObj.printAllSavedCommands();

        cout << "The Current Game State Is: >>MAPLOADED<<" << endl;
        commandProcessorObj.gameEnginePtr->SetState(maploaded);
        commandProcessorObj.readCommand();
        commandProcessorObj.validate(commandProcessorObj.commandObjects.back());
        commandProcessorObj.printAllSavedCommands();

        cout << "The Current Game State Is: >>MAPVALIDATED<<" << endl;
        commandProcessorObj.gameEnginePtr->SetState(mapvalidated);
        commandProcessorObj.readCommand();
        commandProcessorObj.validate(commandProcessorObj.commandObjects.back());
        commandProcessorObj.printAllSavedCommands();

        cout << "The Current Game State Is: >>playersadded<<" << endl;
        commandProcessorObj.gameEnginePtr->SetState(playersadded);
        commandProcessorObj.readCommand();
        commandProcessorObj.validate(commandProcessorObj.commandObjects.back());
        commandProcessorObj.printAllSavedCommands();

        cout << "The Current Game State Is: >>WIN<<" << endl;
        commandProcessorObj.gameEnginePtr->SetState(win);
        commandProcessorObj.readCommand();
        commandProcessorObj.validate(commandProcessorObj.commandObjects.back());
        commandProcessorObj.printAllSavedCommands();
        cout << endl;
    }
        //************ READING FROM FILE readCommand();****************************
    else {
        FileLineReader* fileLineReaderObj = new FileLineReader(userChoice + ".txt");//"commands.txt
        adaptor = new FileCommandProcessorAdapter(fileLineReaderObj);
        cout << "--------------------------------------------" << endl;
        cout << ">> testing readCommand() Method FROM file <<" << endl;
        cout << "--------------------------------------------" << endl;

        adaptor->gameEnginePtr = new GameEngine();
        adaptor->gameEnginePtr->SetState(start);
        adaptor->readCommand();
        adaptor->printAllSavedCommands();
        cout << endl;
    }

}
