#include <iostream>
#include "GameEngine.h"
#include "LoggingObserver.h"

void EngineDriver()
{
    GameEngine* engine;
    engine = new GameEngine();
    LogObserver* observer = new LogObserver(engine);
    engine->StartupPhase();
}