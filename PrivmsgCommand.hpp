#ifndef PRIVMSGCOMMAND_HPP
#define PRIVMSGCOMMAND_HPP

#include "Command.hpp"
#include "Client.hpp"
#include <map>
#include <vector>
#include <string>

class PrivmsgCommand : public Command{
    public:
        PrivmsgCommand();
        ~PrivmsgCommand();
        void execute(Client& client, const std::vector<std::string>& args);
        // void sendMsg(Client& client, const std::vector<std::string>& args, std::map<int, Client>& clients);
    // private:
        // Server& server;
};

#endif