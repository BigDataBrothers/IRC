#include "PrivmsgCommand.hpp"
#include "Server.hpp"
#include <iostream>
#include <sstream>

std::string colorize(const std::string& text, int color) {
    std::ostringstream oss;
    oss << "\x03" << color << text << "\x03";
    return oss.str();
}

PrivmsgCommand::PrivmsgCommand() {}

PrivmsgCommand::~PrivmsgCommand() {}

void PrivmsgCommand::execute(Client& client, const std::vector<std::string>& args) {
    (void)client; (void)args ;
}


void PrivmsgCommand::sendMsg(Client& client, const std::vector<std::string>& args, std::map<int, Client>& clients) {
    (void)client;
    (void)clients;
    if (args.size() < 3)
    {
        std::string response = "Error: /msg [Target] [Message]\r\n";
        send(client.getSocket(), response.c_str(), response.size(), 0);
        return;
    }
    std::string target;
    target = args[1];
    std::string message;

    // Itérer sur les éléments de args à partir de l'index 2
    for (size_t i = 2; i < args.size(); ++i) {
        message += args[i];
        if (i < args.size() - 1) {
            message += " "; // Ajouter un espace entre les mots
        }
    }

    std::map<int, Client>::iterator it;

    bool found = false;
    for (it = clients.begin(); it != clients.end(); ++it) {
        if (it->second.getNickname() == target) {
            found = true;
            std::string fullMessage = "[" + client.getNickname() + "] -> " + message + "\r\n";
            std::string fm = colorize(fullMessage, 12);
            send(it->second.getSocket(), fm.c_str(), fm.size(), 0);
            break;
        }
    }
    if (!found) {
        std::string errorResponse = "Error: Utilisateur '" + target + "' non trouvé\r\n";
        send(client.getSocket(), errorResponse.c_str(), errorResponse.size(), 0);
    }
}