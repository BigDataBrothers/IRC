#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// class Channel;

class Client {
public:
    Client(int socket/*, const struct sockaddr_in& addr*/);
    ~Client();
    
    std::string receiveMessage();
    void sendMessage(const std::string &message);
    
    // void joinChannel(Channel* channel);
    // void leaveChannel(Channel* channel);
    
    bool isAuthenticated() const;
    void authenticate();
    
    int getSocketFd() const;
    std::string getNickname() const;
    void setNickname(const std::string &new_nickname);
    std::string getUsername() const;
    void setUsername(const std::string &new_username);
    std::string getRealname() const;
    void setRealname(const std::string &new_realname);
    std::string getHostname() const;
    void setHostname(const std::string &new_hostname);
    std::string getServername() const;
    void setServername(const std::string &new_servername);
    bool isOp() const;
    void setOperator(bool op);
    std::string getAwayMessage() const;
    void setAwayMessage(const std::string &message);

private:
    int socket_fd;
    std::string nickname;
    std::string username;
    std::string realname;
    std::string hostname;
    std::string servername;
    bool authenticated;
    bool isOperator;
    std::string awayMessage;
    // std::vector<Channel*> channels;
};

#endif // CLIENT_HPP