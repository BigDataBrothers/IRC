#include "Client.hpp"

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