#include "CapCommand.hpp"

CapCommand::~CapCommand() {}

void CapCommand::execute(Client& client, const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::string response = "Error: CAP nécessite un argument\r\n";
        send(client.getSocket(), response.c_str(), response.size(), 0);
        return;
    }

    std::string subCommand = args[1];
    if (subCommand == "LS") {
        std::string response = ":server CAP " + client.getNickname() + " LS :\r\n";
        send(client.getSocket(), response.c_str(), response.size(), 0);
    // } else if (subComcket(), welcomeMmand == "REQ") {
    //     if (args.size() < 3) {
    //         std::string response = "Error: CAP REQ nécessite des capacités\r\n";
    //         send(client.getSocket(), response.c_str(), response.size(), 0);
    //         return;
    //     }
    //     std::string response = "CAP * ACK :" + args[2] + "\r\n";
    //     send(client.getSocket(), response.c_str(), response.size(), 0);
    // } else if (subCommand == "LIST" ) {
    //     std::string response = ":server CAP " + client.getNickname() + " LIST :\r\n";
    //     send(client.getSocket(), response.c_str(), response.size(), 0);
    } else if (subCommand == "END") {
        // std::string response = "CAP * END\r\n";
        // send(client.getSocket(), response.c_str(), response.size(), 0);
        std::string welcomeMessage = ":server 001 " + client.getNickname() + " :Welcome to the IRC Network " + client.getNickname() + "\r\n";
        send(client.getSocket(), welcomeMessage.c_str(), welcomeMessage.size(), 0);
    } else {
        std::string response = "Error: CAP sous-commande inconnue\r\n";
        send(client.getSocket(), response.c_str(), response.size(), 0);
    }
}