#include "Channel.hpp"

Channel::Channel(const std::string &Nname) : name(Nname) {}

Channel::~Channel() {}

std::string Channel::getName() const {
    return name;
}

void Channel::addClient(Client* client) {
    if (std::find(clients.begin(), clients.end(), client) == clients.end())
        clients.push_back(client);
}

void Channel::removeClient(Client* client) {
    clients.erase(std::remove(clients.begin(), clients.end(), client), clients.end());
}

void Channel::broadcastMessage(const std::string &message, Client* sender) {
    for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
        if (*it != sender)
            (*it)->sendMessage(message);
}