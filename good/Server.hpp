#ifndef SERVER_HPP
#define SERVER_HPP

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
    private:
    void acceptNewConnection();
    void handleClientMessage(int clientSocket);

    int _serverSocket;
    int _port;
    std::vector<pollfd> _poll_fds;
    std::string         _password;

    // private:
    //     struct sockaddr_in  _addr;
    //     int                 _port;
    //     int                 _serverSocket;
    //     std::vector<pollfd> _poll_fds;
};

#endif