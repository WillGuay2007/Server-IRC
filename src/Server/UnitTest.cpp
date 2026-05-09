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
#include "AwayHandler.h"
#include "JoinHandler.h"
#include "PartHandler.h"
#include "PrivMsgHandler.h"
#include "NamesHandler.h"
#include "ChannelRegistry.h"
#include "Channel.h"

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

void TestAway() {
    MockClient client;
    AwayHandler handler;

    handler.Handle({"Gone for lunch"}, client);
    TEST(client.GetLastSentResponse().find("306") != std::string::npos);

    handler.Handle({}, client);
    TEST(client.GetLastSentResponse().find("305") != std::string::npos);
}

void TestJoin() {
    MockClient client;
    Channel general("#General");
    ChannelRegistry registry{&general};
    JoinHandler handler(registry);

    handler.Handle({}, client);
    TEST(client.GetLastSentResponse().find("461") != std::string::npos);

    handler.Handle({"#General"}, client);
    TEST(general.HasMember(&client));
    TEST(client.GetLastSentResponse().find("366") != std::string::npos);
}

void TestPart() {
    MockClient client;
    Channel general("#General");
    general.AddMember(&client);
    ChannelRegistry registry{&general};
    PartHandler handler(registry);

    handler.Handle({}, client);
    TEST(client.GetLastSentResponse().find("461") != std::string::npos);

    handler.Handle({"#General"}, client);
    TEST(!general.HasMember(&client));

    handler.Handle({"#General"}, client);
    TEST(client.GetLastSentResponse().find("442") != std::string::npos);
}

void TestPrivMsg() {
    MockClient sender;
    MockClient receiver;
    ClientRegistry clientRegistry;
    Channel general("#General");
    clientRegistry.Add(&sender);
    clientRegistry.Add(&receiver);
    sender.SetNick("sender");
    sender.SetUsername("sender");
    sender.SetRealName("sender");
    receiver.SetNick("receiver");
    ChannelRegistry channelRegistry{&general};
    PrivMsgHandler handler(clientRegistry, channelRegistry);

    handler.Handle({"receiver", "Hello!"}, sender);
    TEST(receiver.GetLastSentResponse().find("Hello!") != std::string::npos);

    handler.Handle({"", "Hello!"}, sender);
    TEST(sender.GetLastSentResponse().find("401") != std::string::npos);
}

void RunAllTests() {
    TestNick();
    TestPing();
    TestMOTD();
    TestUser();
    TestAway();
    TestJoin();
    TestPart();
    TestPrivMsg();
}