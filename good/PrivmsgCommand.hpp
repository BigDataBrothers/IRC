#ifndef PRIVMSGCOMMAND_HPP
#define PRIVMSGCOMMAND_HPP

#include "Command.hpp"

class PrivmsgCommand : public Command {
public:
    ~PrivmsgCommand();
    void execute(Client& client, const std::vector<std::string>& args);
};

#endif