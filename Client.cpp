#include "Client.hpp"
#include "Server.hpp"
#include <iostream>
#include <string>

Client::Client() {}

Client::Client(int socket, std::string nick) : _socket(socket), _nickname(nick) {

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
