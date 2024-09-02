#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

#include "NickCommand.hpp"
#include "Command.hpp"
#include "Client.hpp"
#include <sys/socket.h>
#include <iostream>
#include <sstream>
#include <string>
#include <map>

class CommandHandler {
    public:
        CommandHandler();
        ~CommandHandler();
        void registerCommand(const std::string& name, Command* command);
        void handleCommand(Client& client, const std::string& message);
        std::vector<std::string> tokenize(const std::string& str);
    private:
        std::map<std::string, Command*> commands;
};

#endif