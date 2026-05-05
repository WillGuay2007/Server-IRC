#include "ServerClient.h"
#include "ClientSocket.h"
#include "Channel.h"

ServerClient::ServerClient(ClientSocket* socket) : m_socket(socket) {}
ServerClient::~ServerClient() { delete m_socket;}

bool ServerClient::Receive(char* buffer, int size) { return m_socket->WaitForResponse(buffer, size); }
void ServerClient::Send(std::string response) {m_socket->Send(response.c_str(), response.size());}