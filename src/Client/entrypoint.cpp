#include "entrypoint.h"
#include "ClientSocket.h"
#include <string>
#include <iostream>
#include <winsock2.h>

void client_start()
{
    WSADATA data;
    WSAStartup(MAKEWORD(2,2), &data);

    ClientSocket client(6667);

    client.Connect();

    std::string msg = "Hello server\r\n";
    client.Send(msg.c_str(), (int)msg.size());

    char buffer[512];
    client.WaitForResponse(buffer, sizeof(buffer));

    std::cout << "Server says: " << buffer << std::endl;

    WSACleanup();
}
