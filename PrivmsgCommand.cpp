#include "PrivmsgCommand.hpp"
#include "Server.hpp"
#include <iostream>
#include <sstream>

PrivmsgCommand::PrivmsgCommand() {}

PrivmsgCommand::~PrivmsgCommand() {}

void PrivmsgCommand::execute(Client& client, const std::vector<std::string>& args) {
    (void)client; (void)args ;
}


// void PrivmsgCommand::sendMsg(Client& client, const std::vector<std::string>& args, std::map<int, Client>& clients) {
//     // Vérification du nombre d'arguments
//     if (args.size() < 3) {
//         // Si le nombre d'arguments est insuffisant, informer l'utilisateur
//         std::cout << "Usage: /msg [Target] [Message]" << std::endl;
//         return;
//     }

//     // Extraction de la cible et du message à envoyer
//     std::string target = args[1];
//     std::string message = args[2];

//     std::cout << "cible " << target << " : " << message << std::endl;
//     // Recherche du client cible dans la liste des clients
//     bool found = false;
//     std::map<int, Client>::iterator it;
//     for (it = clients.begin(); it != clients.end(); ++it) { 
        
//         std::cout <<  "nicke = " << it->second.getNickname() << std::endl;
//         if (it->second.getNickname() == target) {
//             // Client trouvé, envoi du message
//             found = true;
//             // Format du message : "[Source Nickname] : [Message]"
//             std::string fullMessage = client.getNickname() + " : " + message;
//             // Appel à une méthode d'envoi de message du Client (à implémenter si nécessaire)
//             it->second.sendMessage(fullMessage);
//             std::cout << "Message sent to " << target << ": " << message << std::endl;
//             break ;
//         }
//     }
//     if (!found) {
//         // Si le client n'a pas été trouvé
//         std::cout << "Error: User '" << target << "' not found." << std::endl;
//     }
// }


void PrivmsgCommand::sendMsg(Client& client, const std::vector<std::string>& args, std::map<int, Client>& clients) {
    (void)client;
    (void)clients;
    if (args.size() < 3)
    {
        std::string response = "Erreur: /msg [Target] [Message]\r\n";
        send(client.getSocket(), response.c_str(), response.size(), 0);
        return;
    }
    std::string target;
    target = args[1];
    std::string message;

    // Itérer sur les éléments de args à partir de l'index 2
    for (size_t i = 2; i < args.size(); ++i) {
        message += args[i];
        if (i < args.size() - 1) {
            message += " "; // Ajouter un espace entre les mots
        }
    }

    std::map<int, Client>::iterator it;

    bool found = false;
    for (it = clients.begin(); it != clients.end(); ++it) {
        if (it->second.getNickname() == target) {
            found = true;
            std::string fullMessage = "[" + client.getNickname() + "] : " + message + "\r\n";
            send(it->second.getSocket(), fullMessage.c_str(), fullMessage.size(), 0);
            break;
        }
    }
    if (!found) {
        std::string errorResponse = "Erreur: Utilisateur '" + target + "' non trouvé\r\n";
        send(client.getSocket(), errorResponse.c_str(), errorResponse.size(), 0);
    }
}