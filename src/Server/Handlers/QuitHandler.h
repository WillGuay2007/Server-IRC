#pragma once

#include "handler.h"

class ClientRegistry;

class QuitHandler : public Handler {
public:
    QuitHandler(ClientRegistry& clientRegistry) : m_clientRegistry(clientRegistry) {}
    ~QuitHandler() = default;
    void Handle(const std::vector<std::string>& params, BaseClient& clientToHandle) override;
private:
    ClientRegistry& m_clientRegistry;
};