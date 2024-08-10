/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 16:27:43 by myassine          #+#    #+#             */
/*   Updated: 2024/08/10 21:47:30 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2)
        error("Erreur:\nNumero de port inexistant");
    std::string portNum = argv[1];
    if(is_numeric(portNum) == false || std::stoi(portNum) < 1024 || std::stoi(portNum) > 65535)
        error("Erreur:\nNumero de port invalide (1024 ~= 65535)");
    
    int sockfd, newsockfd, portno, client, n;
    char buffer[256];
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    
    if(sockfd = socket(AF_INET, SOCK_STREAM, 0) < 0)
        error("Erreur:\nOuverture socket échouer");
    std::memset((char *) &serv_addr, 0, sizeof(serv_addr));
    portno = std::atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("Erreur:\nLiaison socket échouer");
    // listen(s)
}