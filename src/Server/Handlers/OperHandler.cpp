#include "OperHandler.h"

void OperHandler::Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) {
   if (params.size() < 2) {
        clientToHandle.Send(GeneratePrefix(ERR_NEEDMOREPARAMS) + clientToHandle.GetNick() + " OPER :Not enough parameters\r\n");
        return;
    } 

    std::string name = params[0];
    std::string password = params[1];

    if (name == OPER_NAME) {
        if (password == OPER_PASSWORD) {
            clientToHandle.SetOper(true);
            clientToHandle.Send(GeneratePrefix(RPL_YOUREOPER) + " :You are now an operator.\r\n");
            clientToHandle.Send(":" + SERVER_NAME + " MODE " + clientToHandle.GetNick() + " +o\r\n");
            return;
        }
    }
    clientToHandle.Send(GeneratePrefix(ERR_PASSWDMISMATCH) + " :Password incorrect\r\n");

}
