#include "EngineEventHandler.h"

std::vector<EngineEventHandler::StartCallback> EngineEventHandler::startCallbacks;
std::vector<EngineEventHandler::UpdateCallback> EngineEventHandler::updateCallbacks;
std::vector<EngineEventHandler::QuitCallback> EngineEventHandler::quitCallbacks;