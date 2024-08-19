#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <algorithm>
#include "Client.hpp"

class Client;

class Channel {
public:
    Channel(const std::string &Nname);
    ~Channel();

    std::string getName() const;

    void addClient(Client* client);
    void removeClient(Client* client);
    void broadcastMessage(const std::string &message, Client* sender);

private:
    std::string name;                    // Nom du canal
    std::vector<Client*> clients;        // Liste des clients connectés
};

#endif