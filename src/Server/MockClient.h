#pragma once
#include <string>
#include "BaseClient.h"


class MockClient : public BaseClient {
public:
    MockClient() = default;
    ~MockClient() = default;

    void Send(const std::string response) override { m_lastSent = response; }

    std::string GetLastSentResponse() { return m_lastSent; }

private:
    std::string m_lastSent; 
};