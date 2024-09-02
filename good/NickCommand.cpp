#include "NickCommand.hpp"
#include "Command.hpp"
#include <sys/socket.h>

NickCommand::~NickCommand() {}

 void NickCommand::execute(Client& client, const std::vector<std::string>& args) {
    if (args.size() != 2) {
        std::string response = "Erreur: NICK nécessite un argument\r\n";
        send(client.getSocket(), response.c_str(), response.size(), 0);
        return;
    }
    std::string nick = args[1];
    client.setNickname(nick);
    std::string response = "Votre surnom est maintenant " + nick + "\r\n";
    send(client.getSocket(), response.c_str(), response.size(), 0);
}