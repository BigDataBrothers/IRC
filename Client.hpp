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
        // void receiveMessages();

        // void sendMessage(const std::string& message) {
        //     std::cout << "non de la cible " << this->_nickname << std::endl;
        //     if (send(this->_socket, message.c_str(), message.size(), 0) == -1) {
        //         std::cerr << "Erreur d'envoi du message à " << getNickname() << std::endl;
        //     } else {
        //         std::cout << "Message envoyé à " << getNickname() << ": " << message << std::endl;
        //     }
        // }


    private:
        int _socket;
        std::string _nickname;
};

#endif