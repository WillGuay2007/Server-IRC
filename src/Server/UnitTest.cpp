#include "UnitTest.h"
#include "handlers.h"

void RunAllTests() {
    TestHandleMOTD();
}

void TestHandleJoin() {

};
void TestHandleMOTD() {
    ServerClient client;
    TEST(HandleMOTD(client) == MOTD);
};
void TestHandleNick() {

};
void TestHandleUser() {

};
void TestHandlePing() {

};
void TestCheckIfUserIsRegistered() {

};