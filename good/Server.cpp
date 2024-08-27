#include "Server.hpp"

Server::Server(int port, std::string password) : _port(port), _password(password) {
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocket < 0) {
        std::cerr << "Erreur lors de la création du socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = INADDR_ANY;
    _addr.sin_port = htons(port);

    if (bind(_serverSocket, (struct sockaddr*)&_addr, sizeof(_addr)) < 0) {
        std::cerr << "Erreur lors du bind du socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (listen(_serverSocket, SOMAXCONN) < 0) {
        std::cerr << "Erreur lors du listen sur le socket" << std::endl;
        exit(EXIT_FAILURE);
    }
}

Server::~Server() {
    close(_serverSocket);
}

void Server::start() {
    std::cout << "Serveur IRC démarré sur le port " << _port << std::endl;

    while (true) {
        struct sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        int clientSocket = accept(_serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
        if (clientSocket < 0) {
            std::cerr << "Erreur lors de l'acceptation d'une connexion" << std::endl;
            continue;
        }

        std::cout << "Nouveau client connecté" << std::endl;
        handleClient(clientSocket);
    }
}

void Server::handleClient(int client_socket) {
    // Simple echo pour l'instant
    char buffer[1024];
    int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        std::cout << "Message reçu: " << buffer << std::endl;
        send(client_socket, buffer, bytes_received, 0);
    }
    close(client_socket);
}