#include "Client.hpp"

Client::Client(int socket/*, const struct sockaddr_in& addr*/)
        : socket_fd(socket)/*, _addr(addr)*/, nickname("guest"), username("guest"),
          realname("Unknown"), hostname("localhost"), servername("IRCServer"),
          authenticated(false), isOperator(false), awayMessage("") {}

Client::~Client() {
        if (socket_fd != -1)
            close(socket_fd);
        // for (Channel* channel : channels)
        //     if (channel)
        //         channel->removeClient(this);
    }

int Client::getSocketFd() const {return socket_fd; }

std::string Client::getNickname() const { return nickname; }
void Client::setNickname(const std::string &new_nickname) { nickname = new_nickname; }

std::string Client::getUsername() const { return username; }
void Client::setUsername(const std::string &new_username) { username = new_username; }

std::string Client::getRealname() const { return realname; }
void Client::setRealname(const std::string &new_realname) { realname = new_realname; }

std::string Client::getHostname() const { return hostname; }
void Client::setHostname(const std::string &new_hostname) { hostname = new_hostname; }

std::string Client::getServername() const { return servername; }
void Client::setServername(const std::string &new_servername) { servername = new_servername; }

bool Client::isOp() const { return isOperator; }
void Client::setOperator(bool op) { isOperator = op; }

std::string Client::getAwayMessage() const { return awayMessage; }
void Client::setAwayMessage(const std::string &message) { awayMessage = message; }

std::string Client::receiveMessage() {
     char buffer[1024];
    ssize_t bytes_received = recv(socket_fd, buffer, sizeof(buffer) - 1, 0);

    if (bytes_received < 0) {
        std::cerr << "Error receiving message from client." << std::endl;
        return "";
    }
    buffer[bytes_received] = '\0';
    return std::string(buffer);
}

void Client::sendMessage(const std::string &message) {
    ssize_t bytes_sent = send(socket_fd, message.c_str(), message.length(), 0);
    if (bytes_sent < 0)
        std::cerr << "Error sending message to client." << std::endl;
}

// void Client::joinChannel(Channel* channel) {
//     if (std::find(channels.begin(), channels.end(), channel) == channels.end()) {
//         channels.push_back(channel);
//         // channel->addClient(this);
//     }
// }

// void Client::leaveChannel(Channel* channel) {
//    std::vector<Channel*>::iterator it = std::find(channels.begin(), channels.end(), channel);
//     if (it != channels.end()) {
//         channels.erase(it);
//         // channel->removeClient(this);
//     }
// }