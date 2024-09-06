#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <iostream>
#include <unistd.h>
#include <exception>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class Client {
    public:
        Client();
        Client(int socket, std::string nick);

        int getSocket() const;
        std::string getNickname() const;
        void setNickname(const std::string& nick);

    private:
        int _socket;
        std::string _nickname;
};

#endif