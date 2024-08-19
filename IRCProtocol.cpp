#include "IRCProtocol.hpp"

IRCProtocol::IRCProtocol(Server* server) : server(server) {}

void IRCProtocol::handleMessage(Client* client, const std::string &message) {
    // Parsing et gestion des commandes basiques (à implémenter)
}

void IRCProtocol::handleJoin(Client* client, const std::string &channelName) {
    Channel* channel = server->findChannel(channelName);
    if (!channel) {
        channel = server->createChannel(channelName);
    }
    channel->addClient(client);
    client->joinChannel(channel);
    channel->broadcastMessage(client->getNickname() + " has joined " + channelName, client);
}

void IRCProtocol::handlePart(Client* client, const std::string &channelName) {
    Channel* channel = server->findChannel(channelName);
    if (channel) {
        channel->removeClient(client);
        client->leaveChannel(channel);
        channel->broadcastMessage(client->getNickname() + " has left " + channelName, client);
    }
}

void IRCProtocol::handlePrivMsg(Client* client, const std::string &target, const std::string &message) {
    Channel* channel = server->findChannel(target);
    if (channel) {
        channel->broadcastMessage(client->getNickname() + ": " + message, client);
    }
}