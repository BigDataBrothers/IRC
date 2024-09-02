#include "CommandHandler.hpp"

CommandHandler::CommandHandler() {
    registerCommand("/nick", new NickCommand());
    // Enregistrez d'autres commandes ici
}

CommandHandler::~CommandHandler() {
    for (std::map<std::string, Command*>::iterator it = commands.begin(); it != commands.end(); ++it)
        delete it->second;
}

void CommandHandler::registerCommand(const std::string& name, Command* command) {
    commands[name] = command;
}

void CommandHandler::handleCommand(Client& client, const std::string& message) {
    std::vector<std::string> tokens = tokenize(message);
    if (tokens.empty()) return;

    std::string commandName = tokens[0];
    std::map<std::string, Command*>::iterator it = commands.find(commandName);
    if (it != commands.end())// execution command nick
        it->second->execute(client, tokens);
    else {
        std::string response = "Commande inconnue: " + commandName + "\r\n";
        send(client.getSocket(), response.c_str(), response.size(), 0);
    }
}

std::vector<std::string> CommandHandler::tokenize(const std::string& str) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}