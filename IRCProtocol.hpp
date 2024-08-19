#ifndef IRC_PROTOCOL_HPP
#define IRC_PROTOCOL_HPP

#include <string>
#include "Client.hpp"
#include "Server.hpp"

class IRCProtocol {
public:
    IRCProtocol(Server* server);

    void handleMessage(Client* client, const std::string &message);

private:
    void handleJoin(Client* client, const std::string &channelName);
    void handlePart(Client* client, const std::string &channelName);
    void handlePrivMsg(Client* client, const std::string &target, const std::string &message);

    Server* server;
};

#endif