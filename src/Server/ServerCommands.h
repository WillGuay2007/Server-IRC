#include "ICommand.h"

class cmd_find : public ICommand {
    virtual void execute() override;
};

class cmd_nick : public ICommand {
    virtual void execute() override;
};

class cmd_join : public ICommand {
    virtual void execute() override;
};