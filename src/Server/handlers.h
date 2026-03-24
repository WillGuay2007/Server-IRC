#include "ServerClient.h"
#include <string>
#include "ServerResponses.h"
#include "ClientSocket.h"
#include "Channel.h"

const std::string MOTD = "You either cum in the sink or sink in the cum\n";
const std::string serverName = "ScaryServer";

std::string GeneratePrefix(ServerClient& client, EServerResponse response);
void SendStringResponse(ServerClient& client, std::string response);
void HandleJoin(ServerClient& client, std::vector<std::string>& parameters, std::vector<Channel*> channels);
void HandleMOTD(ServerClient& client, std::vector<std::string>& parameters);
void HandleNick(ServerClient& client, std::vector<std::string>& parameters, std::vector<ServerClient*> clients);
void HandleUser(ServerClient& client, std::vector<std::string>& parameters);
void HandlePing(ServerClient& client, std::vector<std::string>& parameters);