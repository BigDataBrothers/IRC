#include "Server.hpp"
#include "PrivmsgCommand.hpp"

bool is_numeric(const std::string& str) {
    if (str.empty()) return false;
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
        if (!std::isdigit(*it))
            return false;
    return true;
}

void verifArgv(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "./serveur [numero de port] [mots de passe]" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string portNum = argv[1];
    if(is_numeric(portNum) == false || std::atoi(portNum.c_str()) < 1024 || std::atoi(portNum.c_str()) > 65535) {
        std::cerr << "Numero de port invalide (1024 ~= 65535)" << std::endl;
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv) {

    verifArgv(argc, argv);
    unsigned int port = static_cast<unsigned short>(std::atoi(argv[1]));
    std::string password = argv[2];
    try {
        Server server(port, password);
        
        server.start();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Server shutting down." << std::endl;
    exit(EXIT_SUCCESS);
}