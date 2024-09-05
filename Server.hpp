#ifndef SERVER_HPP
#define SERVER_HPP

#include "CommandHandler.hpp"
#include "Client.hpp"

#include <map>
#include <poll.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <exception>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class Server {
    public:
        Server(int port, std::string password);
        ~Server();

        void start();
        void handleClient(int client_socket);
        std::string getHostname() const;
    private:
    void acceptNewConnection();
    void handleClientMessage(int clientSocket);

    int clientCounter;
    int _serverSocket;
    int _port;
    std::vector<pollfd> _poll_fds;
    std::string         _password;
    std::map<int, Client> clients; // Map des sockets clients vers les objets Client
    CommandHandler commandHandler; // Ajoutez un membre CommandHandler
    char hostname[256];
};


#endif