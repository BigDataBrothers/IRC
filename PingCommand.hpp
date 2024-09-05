#ifndef PINGCOMMAND_HPP
#define PINGCOMMAND_HPP

#include "Command.hpp"

class PingCommand : public Command {
    public:
        ~PingCommand();
        void execute(Client& client, const std::vector<std::string>& args);
};

#endif