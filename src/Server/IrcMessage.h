#pragma once

#include <string>
#include <vector>

class IrcMessage {
public:
    static IrcMessage Parse(std::string lineToParse);
    const std::string& GetCommand() const { return m_command;};
    const std::vector<std::string>& GetParams() const { return m_params;};
private:
    std::string m_command;
    std::vector<std::string> m_params;
};