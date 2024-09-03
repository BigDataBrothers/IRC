#include "Client.hpp"
#include "Server.hpp"
#include <iostream>
#include <string>

Client::Client() {}

Client::Client(int socket) : _socket(socket), _nickname("Guest") {

}

int Client::getSocket() const {
    return _socket;
}

std::string Client::getNickname() const {
    return _nickname;
}

void Client::setNickname(const std::string& nick) {
    _nickname = nick;
}

// void Client::receiveMessages() {
//     char buffer[1024];
//     while (true) {
//         int bytes_received = recv(_socket, buffer, sizeof(buffer) - 1, 0);
//         if (bytes_received <= 0) {
//             std::cerr << "Erreur lors de la réception ou connexion fermée" << std::endl;
//             close(_socket);
//             break;
//         }
//         buffer[bytes_received] = '\0';
//         std::cout << "Message reçu: " << buffer << std::endl;
//         // Traitez le message reçu ici
//     }
// }