#ifndef NICKCOMMAND_HPP
#define NICKCOMMAND_HPP

#include "Command.hpp"

class NickCommand : public Command {
    public:
        ~NickCommand();
        void execute(Client& client, const std::vector<std::string>& args);
};

#endif