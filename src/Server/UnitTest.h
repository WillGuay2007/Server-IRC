#include <string>
#include <vector>
#include "ServerClient.h"
#include "Channel.h"

#pragma once

#define TEST(expression) \
do { \
    if(!(expression)){\
        fprintf(stderr,"\e[0;31m TEST %s FAILED: %s : %d \e[0m \n", #expression,__FILE__,__LINE__); \
    } \
    else{ \
        fprintf(stdout,"\e[0;32m TEST %s PASSED \e[0m \n", #expression); \
    } \
} while(0) \

void RunAllTests();
std::string TestHandleJoin(ServerClient& client, std::vector<std::string>& parameters, std::vector<Channel*>& channels);
std::string TestHandleMOTD(ServerClient& client, std::vector<std::string>& parameters);
std::string TestHandleNick(ServerClient& client, std::vector<std::string>& parameters, std::vector<ServerClient*>& clients);
std::string TestHandleUser(ServerClient& client, std::vector<std::string>& parameters);
std::string TestHandlePing(ServerClient& client, std::vector<std::string>& parameters);
bool TestCheckIfUserIsRegistered(ServerClient& client);