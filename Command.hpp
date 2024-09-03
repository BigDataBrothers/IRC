#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Client.hpp"
#include <vector>
#include <sys/socket.h>

class Command {
public:
    Command() {}
    virtual ~Command() {}
    virtual void execute(Client& client, const std::vector<std::string>& args) = 0;
};

#endif