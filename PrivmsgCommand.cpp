#include "PrivmsgCommand.hpp"
#include "Server.hpp"
#include <iostream>
#include <sstream>

PrivmsgCommand::PrivmsgCommand() {}

PrivmsgCommand::~PrivmsgCommand() {}

void PrivmsgCommand::execute(Client& client, const std::vector<std::string>& args) {
    (void)client; (void)args ;
}

// void PrivmsgCommand::sendMsg(Client& client, const std::vector<std::string>& args, std::map<int, Client>& clients) {}