#include "Server.hpp"

Server::Server(int port, std::string password) : _port(port), _password(password) {
    // Initialisation du socket du serveur
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocket < 0) {
        std::cerr << "Erreur lors de la création du socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(_port);

    if (bind(_serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Erreur lors du bind" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (listen(_serverSocket, 5) < 0) {
        std::cerr << "Erreur lors du listen" << std::endl;
        exit(EXIT_FAILURE);
    }

    pollfd server_fd;
    server_fd.fd = _serverSocket;
    server_fd.events = POLLIN;
    _poll_fds.push_back(server_fd);
}

void Server::acceptNewConnection() {
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    int clientSocket = accept(_serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
    if (clientSocket < 0) {
        std::cerr << "Erreur lors de l'acceptation d'une connexion" << std::endl;
        return;
    }

    std::cout << "Nouveau client connecté" << std::endl;
    pollfd client_fd;
    client_fd.fd = clientSocket;
    client_fd.events = POLLIN;
    _poll_fds.push_back(client_fd);
}

void Server::handleClientMessage(int clientSocket) {
    char buffer[1024];
    int bytes_received = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) {
        std::cerr << "Connexion fermée ou erreur lors de la réception" << std::endl;
        close(clientSocket);
        // Supprimer le client de la liste des descripteurs surveillés
        for (std::vector<pollfd>::iterator it = _poll_fds.begin(); it != _poll_fds.end(); ++it) {
            if (it->fd == clientSocket) {
                _poll_fds.erase(it);
                break;
            }
        }
        return;
    }
    buffer[bytes_received] = '\0';
    std::cout << "Message reçu: " << buffer << std::endl;

    std::string response = "Message envoyé: ";
    response += buffer;

    int bytes_sent = send(clientSocket, response.c_str(), response.size(), 0);
    if (bytes_sent < 0)
        std::cerr << "Erreur lors de l'envoi du message" << std::endl;
}

void Server::start() {
    std::cout << "Serveur IRC démarré sur le port " << _port << std::endl;

    while (true) {
        int poll_count = poll(_poll_fds.data(), _poll_fds.size(), -1);
        if (poll_count < 0) {
            std::cerr << "Erreur lors de l'appel à poll()" << std::endl;
            exit(EXIT_FAILURE);
        }

        for (size_t i = 0; i < _poll_fds.size(); ++i)
            if (_poll_fds[i].revents & POLLIN) {
                if (_poll_fds[i].fd == _serverSocket)
                    acceptNewConnection();
                else
                    handleClientMessage(_poll_fds[i].fd);
            }
    }
}

Server::~Server() {
    close(_serverSocket);
}
