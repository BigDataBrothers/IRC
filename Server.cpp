#include "Server.hpp"
#include <unistd.h>
#include <fcntl.h>

Server::Server(int port, std::string Npassword) : password(Npassword) {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Error creating socket." << std::endl;
        exit(EXIT_FAILURE);
    }
    int optval = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        std::cerr << "Error setting socket options." << std::endl;
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    std::memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = INADDR_ANY;
    _addr.sin_port = htons(port);
    if (bind(server_socket, (struct sockaddr*)&_addr, sizeof(_addr)) < 0) {
        std::cerr << "Error binding socket." << std::endl;
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    configureSocketNonBlocking(server_socket);
    if (listen(server_socket, SOMAXCONN) < 0) {
        std::cerr << "Error listening on socket." << std::endl;
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    pollfd pfd;
    pfd.fd = server_socket;
    pfd.events = POLLIN;
    pfd.revents = 0;
    poll_fds.push_back(pfd);
}

Server::~Server() {
    stop();
}

void Server::start() {
    while (true)
        handleEvents();
}

void Server::stop() {
    for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
        removeClient(*it);
    close(server_socket);
}

void Server::acceptConnection() {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);

    if (client_socket < 0) {
        std::cerr << "Error accepting connection." << std::endl;
        return;
    }
    Client* new_client = new Client(client_socket/*, client_addr*/);
    clients.push_back(new_client);
    
    pollfd pfd;
    pfd.fd = client_socket;
    pfd.events = POLLIN;
    pfd.revents = 0;
    poll_fds.push_back(pfd);

    std::string password_prompt = "Password: ";
    send(client_socket, password_prompt.c_str(), password_prompt.size(), 0);
}

void Server::removeClient(Client* client) {
    std::vector<Client*>::iterator it = std::find(clients.begin(), clients.end(), client);
    if (it != clients.end()) {
        clients.erase(it);
        close(client->getSocketFd());
        delete client;
    }

    for (std::vector<pollfd>::iterator pfd_it = poll_fds.begin(); pfd_it != poll_fds.end(); ++pfd_it) {
        if (pfd_it->fd == client->getSocketFd()) {
            poll_fds.erase(pfd_it);
            break;
        }
    }
}

// Channel* Server::createChannel(const std::string& channelName) {
//     Channel* new_channel = new Channel(channelName);
//     channels.push_back(new_channel);
//     return new_channel;
// }


// Channel* Server::findChannel(const std::string& channelName) {
//     for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
//         if ((*it)->getName() == channelName)
//             return *it;
//     return nullptr;
// }

Client* Server::findClientBySocket(int socket_fd) {
    for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
        if ((*it)->getSocketFd() == socket_fd)
            return *it;
    return NULL;
}

void Server::authenticateClient(Client* client) {
    for(int i = 0; i != 3; i++) {
        char buffer[1024];
        ssize_t len = recv(client->getSocketFd(), buffer, sizeof(buffer) - 1, 0);
        password.push_back('\n');
        if (len > 0) {
            buffer[len] = '\0';

            std::string received_password(buffer);
            if (received_password.find("\r") != std::string::npos)
                received_password = received_password.substr(0, received_password.find("\r"));
            if (received_password == password) {
                // Authentification réussie
                std::string welcome_message = "Authentication successful.\r\n";
                send(client->getSocketFd(), welcome_message.c_str(), welcome_message.size(), 0);
            } else {
                // Authentification échouée
                std::string error_message = "Invalid password.\r\n";
                send(client->getSocketFd(), error_message.c_str(), error_message.size(), 0);
                removeClient(client);
            }
        } else
            removeClient(client);
    }
}

void Server::handleEvents() {
    int nfds = poll(&poll_fds[0], poll_fds.size(), -1);

    if (nfds < 0) {
        std::cerr << "Error in poll." << std::endl;
        return;
    }

    for (std::vector<pollfd>::iterator it = poll_fds.begin(); it != poll_fds.end(); ++it) {
        if (it->revents & POLLIN) {
            if (it->fd == server_socket)
                acceptConnection();
            else {
                Client* client = findClientBySocket(it->fd);
                if (client)
                    authenticateClient(client);
            }
        }
    }
}

void configureSocketNonBlocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags < 0 || fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0) {
        std::cerr << "Failed to set socket to non-blocking mode" << std::endl;
        exit(EXIT_FAILURE);
    }
}
