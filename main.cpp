/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 16:27:43 by myassine          #+#    #+#             */
/*   Updated: 2024/08/12 23:36:39 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void dostuff(const int sockfd)
{
    int n;
    char buffer[BUFFER_SIZE];
      
    std::memset(buffer, 0, BUFFER_SIZE);
    writeSocket("[Serveur]: ", sockfd);
    n = read(sockfd, buffer, 255);
    if (n < 0) 
        error("ERREUR:\nLecture depuis le socket");

    std::cout << "Message : " << buffer << std::endl;

    n = write(sockfd, "Message reçu\n", 14);
    if (n < 0) 
        error("ERREUR:\nEcriture sur le socket");
}

void writeSocket(const std::string &message, const int sockfd) {
    ssize_t n = write(sockfd, message.c_str(), message.length());
    if (n < 0) {
        error("ERREUR: Écriture sur le socket");
    }
}

std::string readSocketString(const int sockfd) {
    int n;
    char buffer[BUFFER_SIZE];
    std::memset(buffer, 0, sizeof(buffer));
    
    n = read(sockfd, buffer, sizeof(buffer) - 1);
    if (n < 0) 
        error("ERREUR: Lecture depuis le socket");
    
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
        error("ERREUR: Lecture depuis le socket");
}

void passwordAuth(int newsockfd, std::string motsDePasse) {
    std::string mdp;
    while (true) {
            writeSocket("Mots De Passe: ", newsockfd);
            mdp = readSocketString(newsockfd);
            if (mdp == motsDePasse) {
                writeSocket("Mot de passe valide. Vous êtes connecté\n", newsockfd);
                break;
            } writeSocket("Mot de passe incorrect. Veuillez réessayer\n", newsockfd);
        }
}

void verifArgv(int argc, char **argv) {
    if (argc != 3)
        error("ERREUR:\n./serveur [numero de port] [mots de passe]");
    std::string portNum = argv[1];
    if(is_numeric(portNum) == false || std::atoi(portNum.c_str()) < 1024 || std::atoi(portNum.c_str()) > 65535)
        error("ERREUR:\nNumero de port invalide (1024 ~= 65535)");
}

int main(int argc, char *argv[]) {
    verifArgv(argc, argv);
    
    int sockfd, newsockfd, portno, client, n;
    std::string motsDePasse = argv[2];
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    
    motsDePasse.push_back('\n'); // ajouts de '\n' etand donne que nc l ajoute  la fin de ces string
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERREUR:\nOuverture de socket");
    std::memset((char *) &serv_addr, 0, sizeof(serv_addr));
    portno = std::atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERREUR:\nLiaison de socket");
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    while (true) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) 
            error("ERREUR : acceptation de la connexion");
        passwordAuth(newsockfd, motsDePasse);
        writeSocket("Bienvenue sur le serveur IRC\n", newsockfd);
        dostuff(newsockfd);
        close(newsockfd);
    }

    close(sockfd);
    (void)n;
    (void)client;
    return 0;
}

    // newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    // if(newsockfd < 0)
    //     error("ERREUR:\nAceptation de la connexion");
    // std::memset(buffer, 0, BUFFER_SIZE);
    // n = read(newsockfd, buffer, 255);
    // if (n < 0) 
    //     error("ERREUR:\nLecture depuis le socket");
    // std::cout << "Voici le message : " << buffer << std::endl;
    // n = write(newsockfd, "J'ai reçu votre message", 25);
    // if (n < 0) 
    //     error("ERREUR:\nEcriture sur le socket");
    // close(newsockfd);