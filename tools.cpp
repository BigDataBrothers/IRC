/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 16:26:53 by myassine          #+#    #+#             */
/*   Updated: 2024/08/19 19:22:37 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools.hpp"
#include <unistd.h>
#include <fcntl.h>

void error(std::string msg)
{
    std::cerr << "ERREUR:" << std::endl << msg << std::endl;
    exit(EXIT_FAILURE);
}

bool is_numeric(const std::string& str) {
    if (str.empty()) return false;
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
        if (!std::isdigit(*it))
            return false;
    return true;
}

void configureSocketNonBlocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags < 0)
        error("fcntl a echouer");
    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0)
        error("fcntl a echouer");
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