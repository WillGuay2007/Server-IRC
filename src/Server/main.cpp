#include <thread>
#include "Server.h"
#include "ServerMonitor.h"
#include "UnitTest.h"

int main()
{
    RunAllTests();
    Server server;
    ServerMonitor monitor = ServerMonitor(server);
    //Thread sinon l'interface apparait jamais.
    std::thread serverThread([&server]() {server.Start();});
    serverThread.detach();
    monitor.Display();
}
