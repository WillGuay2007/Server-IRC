#include "IrcMessage.h"

namespace
{
    //Helper functions
    bool CanAndMoveToNextSpace(size_t &pos, const std::string &lineToParse)
    {
        return (pos = lineToParse.find(' ')) != std::string::npos;
    }
    bool ColonAtNextParam(size_t pos, const std::string &lineToParse)
    {
        return pos + 1 < lineToParse.size() && (lineToParse[pos + 1] == ':');
    }
    bool NoParamColon(const std::string &lineToParse)
    {
        return !lineToParse.empty() && lineToParse[0] != ':';
    }
    bool IsAtTrailingParam(const std::string &lineToParse)
    {
        return !lineToParse.empty() && lineToParse[0] == ':';
    }
    void StoreAndEraseParameter(std::vector<std::string> &parametersVector, std::string &lineToParse, size_t pos)
    {
        parametersVector.push_back(lineToParse.substr(0, pos));
        lineToParse.erase(0, pos + 1);
    }

    std::vector<std::string> ParseParameters(std::string lineToParse) {
        size_t pos = 0;
        std::vector<std::string> parametersVector;

        while (NoParamColon(lineToParse) && CanAndMoveToNextSpace(pos, lineToParse))
        {
            bool isTrailing = ColonAtNextParam(pos, lineToParse);
            StoreAndEraseParameter(parametersVector, lineToParse, pos);
            if (isTrailing) break;
        }

        if (IsAtTrailingParam(lineToParse))
        {
            parametersVector.push_back(lineToParse.substr(1));
            return parametersVector;
        }

        if (!lineToParse.empty()) parametersVector.push_back(lineToParse);

        return parametersVector;
    }
}

IrcMessage IrcMessage::Parse(std::string lineToParse) {

    if (lineToParse.size() >= 2 && lineToParse.substr(lineToParse.size() - 2) == "\r\n") lineToParse.erase(lineToParse.size() - 2);
    
    IrcMessage msg;
    size_t spacePos = lineToParse.find(' ');
    msg.m_command = lineToParse.substr(0, spacePos);

    if (spacePos != std::string::npos) msg.m_params = ParseParameters(lineToParse.substr(msg.m_command.length() + 1));

    return msg;
}