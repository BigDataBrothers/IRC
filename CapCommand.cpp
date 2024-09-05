#include "CapCommand.hpp"

void MsgServer(std::string nickname, int clientSocket) {
        std::string msg = ":server 001 " + nickname + " :Welcome to the IRC Network " + nickname + "\r\n";
        std::cout << msg << std::endl;
        send(clientSocket, msg.c_str(), msg.size(), 0);
}

CapCommand::~CapCommand() {}

void CapCommand::execute(Client& client, const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::string response = "Erreur: CAP nécessite un argument\r\n";
        send(client.getSocket(), response.c_str(), response.size(), 0);
        return;
    }

    std::string subCommand = args[1];
    if (subCommand == "LS") {
        std::string response = "CAP * LS :multi-prefix sasl\r\n";
        send(client.getSocket(), response.c_str(), response.size(), 0);
    } else if (subCommand == "REQ") {
        if (args.size() < 3) {
            std::string response = "Erreur: CAP REQ nécessite des capacités\r\n";
            send(client.getSocket(), response.c_str(), response.size(), 0);
            return;
        }
        std::string response = "CAP * ACK :" + args[2] + "\r\n";
        send(client.getSocket(), response.c_str(), response.size(), 0);
    } else if (subCommand == "END") {
        std::string response = "CAP * END\r\n";
        send(client.getSocket(), response.c_str(), response.size(), 0);
        // Finaliser la négociation des capacités
        // std::string welcomeMessage = ":server 001 " + client.getNickname() + " :Bienvenue sur le serveur IRC\r\n";
        // send(client.getSocket(), welcomeMessage.c_str(), welcomeMessage.size(), 0);
        MsgServer(client.getNickname(), client.getSocket());
    } else {
        std::string response = "Erreur: CAP sous-commande inconnue\r\n";
        send(client.getSocket(), response.c_str(), response.size(), 0);
    }
}