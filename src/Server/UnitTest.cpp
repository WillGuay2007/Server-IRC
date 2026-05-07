#include "UnitTest.h"
#include <string>
#include <vector>
#include "MockClient.h"
#include "Channel.h"
#include "ClientRegistry.h"
#include "NickHandler.h"
#include "PingHandler.h"
#include "UserHandler.h"
#include "MOTDHandler.h"
#include "ServerConfigs.h"

void TestNick() {
    MockClient client;
    ClientRegistry registry;
    registry.Add(&client);
    NickHandler handler(registry);

    handler.Handle({}, client);

    TEST(client.GetLastSentResponse().find("431") != std::string::npos);

    handler.Handle({"William"}, client);
    handler.Handle({"William"}, client);

    TEST(client.GetLastSentResponse().find("433") != std::string::npos);
}

void TestPing() {
    MockClient client;
    PingHandler handler;

    handler.Handle({}, client);
    TEST(client.GetLastSentResponse() == "PONG\n");

    handler.Handle({"Bruh"}, client);
    TEST(client.GetLastSentResponse() == "PONG Bruh\n");
}

void TestMOTD() {
    MockClient client;
    MOTDHandler handler;

    handler.Handle({}, client);
    TEST(client.GetLastSentResponse() == MOTD);
}

void TestUser() {
    MockClient client;
    UserHandler handler;

    handler.Handle({}, client);
    TEST(client.GetLastSentResponse().find("461") != std::string::npos);

    handler.Handle({"user1", "0", "*"}, client);
    TEST(client.GetLastSentResponse().find("461") != std::string::npos);

    handler.Handle({"user1", "0", "*", "Real Name"}, client);

    handler.Handle({"user1", "0", "*", "Real Name"}, client);
    TEST(client.GetLastSentResponse().find("462") != std::string::npos);
}

void RunAllTests() {
    TestNick();
    TestPing();
    TestMOTD();
    TestUser();
}