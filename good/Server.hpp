#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <cstring>
#include <iostream>
#include <unistd.h>
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
        struct sockaddr_in  _addr;
        int                 _port;
        int                 _serverSocket;
        std::string         _password;
};

#endif