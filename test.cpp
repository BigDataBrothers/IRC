#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>

#define PORT 8080
#define TIMEOUT 5000  // Timeout de 5 secondes

class PollingServer {
public:
    PollingServer() : listen_fd(-1) {
        // Création du socket d'écoute
        listen_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (listen_fd < 0) {
            perror("socket");
            exit(EXIT_FAILURE);
        }

        // Configuration de l'adresse du serveur
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(PORT);

        if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("bind");
            close(listen_fd);
            exit(EXIT_FAILURE);
        }

        // Écoute des connexions entrantes
        if (listen(listen_fd, 5) < 0) {
            perror("listen");
            close(listen_fd);
            exit(EXIT_FAILURE);
        }

        // Initialisation de la structure pollfd
        fds[0].fd = listen_fd;
        fds[0].events = POLLIN;
    }

    ~PollingServer() {
        if (listen_fd >= 0) {
            close(listen_fd);
        }
    }

    void run() {
        while (true) {
            int ret = poll(fds, 1, TIMEOUT);
            if (ret < 0) {
                perror("poll");
                exit(EXIT_FAILURE);
            }

            if (ret == 0) {
                std::cout << "Timeout expiré" << std::endl;
                continue;
            }

            if (fds[0].revents & POLLIN) {
                // Une connexion entrante est disponible
                int client_fd = accept(listen_fd, NULL, NULL);
                if (client_fd < 0) {
                    perror("accept");
                    continue;
                }

                std::cout << "Nouvelle connexion acceptée" << std::endl;
                // Vous pouvez ajouter la gestion des données client ici
                close(client_fd);
            }
        }
    }

private:
    int listen_fd;
    struct sockaddr_in server_addr;
    struct pollfd fds[1];
};

int main() {
    PollingServer server;
    server.run();
    return 0;
}
