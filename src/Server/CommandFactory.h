#pragma once

#include "ServerCommands.h"

class CommandFactory {
public:
    ICommand* CreateNewCommand(char* commandName);
};