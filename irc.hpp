/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myassine <myassine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 16:23:08 by myassine          #+#    #+#             */
/*   Updated: 2024/08/15 21:18:18 by myassine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <string>
#include <poll.h>
#include <fcntl.h>
#include <errno.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 256

class IRCServer {
    public:
            IRCServer(unsigned short port, std::string npassword);
            ~IRCServer();
            void run();
    private:
            int listen_fd;
            std::string password;
            struct sockaddr_in server_addr;
            std::vector<struct pollfd> fds;
            std::vector<int> client_fds;
            
            void acceptNewConnections();
            void handleClientMessage(int client_fd);
            void configureSocketNonBlocking(int sockfd);
            
};

void        error(std::string msg);
bool        is_numeric(const std::string& str);

void        writeSocket(const std::string &message, const int sockfd);
std::string readSocketString(const int sockfd);
void        readSocket(const int sockfd);