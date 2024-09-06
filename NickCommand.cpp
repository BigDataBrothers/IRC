#include "NickCommand.hpp"
#include "Command.hpp"

NickCommand::~NickCommand() {}

 void NickCommand::execute(Client& client, const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::string response = "Error: NICK nécessite un argument\r\n";
        send(client.getSocket(), response.c_str(), response.size(), 0);
        return;
    }
    std::string nick = args[1];
    std::string oldnick = client.getNickname();
    client.setNickname(nick);
    std::string response = ":" + oldnick + " NICK :" + nick + "\r\n";
    send(client.getSocket(), response.c_str(), response.size(), 0);
}