#include "ServerCommands.h"
#include <iostream>

void cmd_find::execute() {
    std::cout << "Executed FIND command.\n";
}

void cmd_join::execute() {
    std::cout << "Executed JOIN command.\n";
}

void cmd_nick::execute() {
    std::cout << "Executed NICK command.\n";
}