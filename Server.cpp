#include "Server.hpp"

Server::Server(int port, std::string password) : _port(port), _password(password) {
    // Initialisation du socket du serveur
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocket < 0) {
        std::cerr << "Erreur lors de la création du socket" << std::endl;
        exit(EXIT_FAILURE);
    }
    int opt = 1;
    if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Erreur lors de la configuration du socket" << std::endl;
        close(_serverSocket);
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
    // Ajout du mdp ici
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    int clientSocket = accept(_serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
    if (clientSocket < 0) {
        std::cerr << "Erreur lors de l'acceptation d'une connexion" << std::endl;
        return;
    }

     pollfd pfd = {clientSocket, POLLIN, 0};
    // pollfd client_fd;
    // client_fd.fd = clientSocket;
    // client_fd.events = POLLIN;
    _poll_fds.push_back(pfd);

     clients[clientSocket] = Client(clientSocket);
    std::cout << "Nouveau client connecté" << std::endl;
}

void Server::handleClientMessage(int clientSocket) {
    char buffer[1024];
    int bytes_received = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) {
        std::cerr << "Connexion fermée ou erreur lors de la réception" << std::endl;
        close(clientSocket);
        for (std::vector<pollfd>::iterator it = _poll_fds.begin(); it != _poll_fds.end(); ++it) {
            if (it->fd == clientSocket) {
                _poll_fds.erase(it);
                break;
            }
        }
         clients.erase(clientSocket);
        return;
    }
    buffer[bytes_received] = '\0';
    std::cout << "[" << clients[clientSocket].getNickname() << "] " << buffer;
   //parser buffer pour les cmd
    Client& client = clients[clientSocket];
    commandHandler.handleCommand(client, buffer, clients);
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
