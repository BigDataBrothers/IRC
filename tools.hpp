#include "Server.hpp"

void error(std::string msg);
bool is_numeric(const std::string& str);
void configureSocketNonBlocking(int sockfd);