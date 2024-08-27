#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>

#include <sstream> 

#include "Client.hpp"

class Channel; // Déclaration anticipée

class Server {
public:
    Server(int port, std::string Npassword);
    ~Server();
    void start();
    void stop();

private:
    void acceptConnection();
    void removeClient(Client* client);
    // Channel* findChannel(const std::string& channelName);
    Client* findClientBySocket(int socket_fd);
    void handleEvents();
    void authenticateClient(Client* client);

    int server_socket;
    std::string password;
    struct sockaddr_in _addr;
    std::vector<Client*> clients;
    // std::vector<Channel*> channels;
    std::vector<pollfd> poll_fds;
};

void configureSocketNonBlocking(int sockfd);

#endif // SERVER_HPP
