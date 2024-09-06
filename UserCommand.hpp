#ifndef USERCOMMAND_HPP
#define USERCOMMAND_HPP

#include "Command.hpp"

class UserCommand : public Command {
    public:
        ~UserCommand();
        void execute(Client& client, const std::vector<std::string>& args);
};

#endif