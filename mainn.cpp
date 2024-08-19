/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 16:27:43 by myassine          #+#    #+#             */
/*   Updated: 2024/08/19 19:22:26 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

// class Server {
//     public:
//             Server(unsigned short port, std::string npassword);
//             ~Server();
//             void run();
//     private:
//             int listen_fd;
//             struct sockaddr_in _addr;
//             std::vector<struct pollfd> fds;
//             std::map<int, Client> clients;
//             std::vector<int> client_fds;
            
//             void acceptNewConnections();
//             void handleClientMessage(int client_fd);
//             void configureSocketNonBlocking(int sockfd);
            
// };

void        error(std::string msg);
bool        is_numeric(const std::string& str);

void        writeSocket(const std::string &message, const int sockfd);
std::string readSocketString(const int sockfd);
void        readSocket(const int sockfd);

// struct Client {
//     int fd;
//     std::string nickname;
//     std::string username;
//     std::string realname;
//     bool hasNick;
//     bool hasUser;

//     Client() : fd(-1), hasNick(false), hasUser(false) {}
//     Client(int fd) : fd(fd), hasNick(false), hasUser(false) {}
// };

void dostuff(const int sockfd)
{
    int n;
    char buffer[BUFFER_SIZE];
      
    std::memset(buffer, 0, BUFFER_SIZE);
    writeSocket("[Serveur]: ", sockfd);
    n = read(sockfd, buffer, 255);
    if (n < 0) 
        error("Lecture depuis le socket");

    std::cout << "Message : " << buffer << std::endl;

    n = write(sockfd, "Message reçu\n", 14);
    if (n < 0) 
        error("Ecriture sur le socket");
}

void writeSocket(const std::string &message, const int sockfd) {
    ssize_t n = write(sockfd, message.c_str(), message.length());
    if (n < 0) {
        error("Écriture sur le socket");
    }
}

std::string readSocketString(const int sockfd) {
    int n;
    char buffer[BUFFER_SIZE];
    std::memset(buffer, 0, sizeof(buffer));
    
    n = read(sockfd, buffer, sizeof(buffer) - 1);
    if (n < 0) 
        error("Lecture depuis le socket");
    
    buffer[n] = '\0';
    std::string msg = buffer;
    return msg;
}

void readSocket(const int sockfd) {
    int n;
    char buffer[BUFFER_SIZE];
    std::memset(buffer, 0, sizeof(buffer));
    
    n = read(sockfd, buffer, sizeof(buffer) - 1);
    if (n < 0) 
        error("Lecture depuis le socket");
}

void passwordAuth(int newsockfd, std::string motsDePasse) {
    std::string mdp;
    while (true) {
        send(newsockfd, "Mots De Passe: ", 16, 0);
        mdp = readSocketString(newsockfd);
        if (mdp == motsDePasse) {
            send(newsockfd, "Mot de passe valide. Vous êtes connecté\n", 43, 0);
            send(newsockfd, "Bienvenue sur le serveur\n", 30, 0);
            break;
        } send(newsockfd, "Mot de passe incorrect. Veuillez réessayer\n", 45, 0);
    }
}

void verifArgv(int argc, char **argv) {
    if (argc != 3)
        error("./serveur [numero de port] [mots de passe]");
    std::string portNum = argv[1];
    if(is_numeric(portNum) == false || std::atoi(portNum.c_str()) < 1024 || std::atoi(portNum.c_str()) > 65535)
        error("Numero de port invalide (1024 ~= 65535)");
}

Server::Server(unsigned short port, std::string npassword) : password(npassword) {
    // password.push_back('\n');// ajouts de '\n' etant donne que nc l'ajoute la fin de ces string
    
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0)
        error("Ouverture de socket");
    int optval = 1;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
        error("Configuration du socket");
    std::memset((char *) &_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = INADDR_ANY;
    _addr.sin_port = htons(port);
    if(bind(listen_fd, (struct sockaddr *) &_addr, sizeof(_addr)) < 0) {
        close(listen_fd);
        error("Liaison de socket");
    }
    configureSocketNonBlocking(listen_fd);
    if (listen(listen_fd, SOMAXCONN) < 0) {
        close(listen_fd);
        error("Ecoute de socket");
    }
    struct pollfd pfd;
    pfd.fd = listen_fd;
    pfd.events = POLLIN;
    pfd.revents = 0;
    fds.push_back(pfd);
}

Server::~Server() {
    if (listen_fd >= 0)
        close(listen_fd);
    for (std::vector<int>::iterator it = client_fds.begin(); it != client_fds.end(); ++it) {
        int fd = *it;
        close(fd);
    }
}

void Server::run() {
    while(true) {
        int ret = poll(&fds[0], fds.size(), 5000);
        if(ret < 0) {
            std::cerr << "Poll a échouer" << std::endl;
            continue;
        }
        if(ret == 0)
            continue;//timeout
        if (fds[0].revents & POLLIN)
            acceptNewConnections();
        for (std::vector<struct pollfd>::iterator it = fds.begin() + 1; it != fds.end(); ++it)
            if (it->revents & POLLIN)
                handleClientMessage(it->fd);
    }
}

void configureSocketNonBlocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags < 0)
        error("fcntl a echouer");
    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0)
        error("fcntl a echouer");
}

void Server::acceptNewConnections() {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    while(true) {
        int client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd < 0) {
            if (errno != EWOULDBLOCK && errno != EAGAIN)// erreur critique
                error("acceptation de la connexion");
            break;
        }
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
        
        passwordAuth(client_fd, password);
        
        std::cout << "Nouvelle connexion acceptée de " << client_ip << ":" << ntohs(client_addr.sin_port) << std::endl;

        struct pollfd pfd_client;
        pfd_client.fd = client_fd;
        pfd_client.events = POLLIN;
        pfd_client.revents = 0;
        fds.push_back(pfd_client); // Ajouter le nouveau client aux structures nécessaires
        clients[client_fd] = Client(client_fd);
    }
}

void Server::handleClientMessage(int client_fd) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    ssize_t len = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if(len <= 0) {
        if(len == 0)
            std::cout << "Client déconnecté" << std::endl;
        else
            error("recv a échouer");

        // Fermer et retirer le client
        close(client_fd);
        for (std::vector<struct pollfd>::iterator it = fds.begin() + 1; it != fds.end(); ++it) {
            if (it->fd == client_fd)
                it = fds.erase(it);
            else
                ++it;
        }
        client_fds.erase(std::remove(client_fds.begin(), client_fds.end(), client_fd), client_fds.end());
    }
    buffer[len] = '\0';
    std::string message(buffer);
    std::istringstream iss(message);
    std::string command;
    iss >> command;

    
    // if (command == "NICK" && clients[client_fd].hasNick == false) {
    //     std::string nick;
    //     iss >> nick;
    //     clients[client_fd].nickname = nick;
    //     clients[client_fd].hasNick = true;
    //     std::cout << "NICK command received: " << nick << std::endl;
    // } else if (command == "USER" && clients[client_fd].hasUser == false) {
    //     std::string username, hostname, name, realname;
    //     iss >> username >> hostname >> name;
    //     getline(iss, realname);
    //     if (!realname.empty() && realname[0] == ':') {
    //         realname = realname.substr(1); // Supprimer les deux-points au début
    //     }
    //     clients[client_fd].username = username;
    //     clients[client_fd].realname = realname;
    //     clients[client_fd].hasUser = true;
    //     std::cout << "USER command received: " << username << ", " << realname << std::endl;
    // } else {
    //     std::cout << clients[client_fd].username << ": Commande non reconnue: " << message << std::endl;
    // }

    // Vérification si le client est complètement connecté
    // if (clients[client_fd].hasNick && clients[client_fd].hasUser) {
    //     std::string welcomeMsg = "001 " + clients[client_fd].nickname + " :Bienvenue sur Server\n";
    //     send(client_fd, welcomeMsg.c_str(), welcomeMsg.size(), 0);
    // }
    std::cout << "Message reçu: " << buffer << std::endl;
    if(send(client_fd, "Message reçu\n", 15, 0) < 0)
        error("send a échouer");
}

int main(int argc, char *argv[]) {
    verifArgv(argc, argv);

    unsigned short port = static_cast<unsigned short>(std::atoi(argv[1]));
    std::string password = argv[2];
    password.push_back('\n');

    Server (port, password);
    .run();
}

// int main(int argc, char *argv[]) {
//     verifArgv(argc, argv);
    
//     int sockfd, newsockfd, portno, client, n;
//     std::string motsDePasse = argv[2];
//     socklen_t clilen;
//     struct sockaddr_in serv_addr, cli_addr;
    
//     motsDePasse.push_back('\n'); // ajouts de '\n' etant donne que nc l'ajoute la fin de ces string
//     sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     if (sockfd < 0)
//         error("Ouverture de socket");
//     std::memset((char *) &serv_addr, 0, sizeof(serv_addr));
//     portno = std::atoi(argv[1]);
//     serv_addr.sin_family = AF_INET;
//     serv_addr.sin_addr.s_addr = INADDR_ANY;
//     serv_addr.sin_port = htons(portno);
//     if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
//         error("Liaison de socket");
//     listen(sockfd, 5);
//     clilen = sizeof(cli_addr);
//     while (true) {
//         newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
//         if (newsockfd < 0) 
//             error("acceptation de la connexion");
//         passwordAuth(newsockfd, motsDePasse);
//         writeSocket("Bienvenue sur le serveur Server\n", newsockfd);
//         dostuff(newsockfd);
//         close(newsockfd);
//     }

//     close(sockfd);
//     (void)n;
//     (void)client;
//     return 0;
// }

    // newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    // if(newsockfd < 0)
    //     error("Aceptation de la connexion");
    // std::memset(buffer, 0, BUFFER_SIZE);
    // n = read(newsockfd, buffer, 255);
    // if (n < 0) 
    //     error("Lecture depuis le socket");
    // std::cout << "Voici le message : " << buffer << std::endl;
    // n = write(newsockfd, "J'ai reçu votre message", 25);
    // if (n < 0) 
    //     error("Ecriture sur le socket");
    // close(newsockfd);