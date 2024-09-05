#include "PingCommand.hpp"

PingCommand::~PingCommand() {}

void PingCommand::execute(Client& client, const std::vector<std::string>& args) {
     if (args.size() < 2) {
        std::string error = "ERROR :Not enough parameters given\r\n";
        send(client.getSocket(), error.c_str(), error.size(), 0);
        return;
    }

    std::string pong = "PONG" + args[1] + "\r\n";
    send(client.getSocket(), pong.c_str(), pong.size(), 0);
    std::cout << "PONG sent to client " << client.getNickname() << " with argument: " << args[1] << std::endl;
}