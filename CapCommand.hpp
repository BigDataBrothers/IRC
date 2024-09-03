#ifndef CAPCOMMAND_HPP
#define CAPCOMMAND_HPP

#include "Command.hpp"

class CapCommand : public Command {
public:
    ~CapCommand();
    void execute(Client& client, const std::vector<std::string>& args);
};

#endif