#include "PrivmsgCommand.hpp"
#include "Server.hpp"

PrivmsgCommand::~PrivmsgCommand() {}

void PrivmsgCommand::execute(Client& client, const std::vector<std::string>& args) {
    if (args.size() < 3) {
        std::string response = "Erreur: PRIVMSG nécessite deux arguments\r\n";
        send(client.getSocket(), response.c_str(), response.size(), 0);
        return;
    }
    std::string target = args[1];
    std::string message = args[2];
    //
    std::string response = "Message envoyé à " + target + ": " + message + "\r\n";
    send(client.getSocket(), response.c_str(), response.size(), 0);

}