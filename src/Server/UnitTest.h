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
void TestHandleJoin();
void TestHandleMOTD();
void TestHandleNick();
void TestHandleUser();
void TestHandlePing();
void TestCheckIfUserIsRegistered();