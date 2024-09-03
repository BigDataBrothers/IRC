#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client {
    public:
        Client();
        Client(int socket);

        int getSocket() const;
        std::string getNickname() const;
        void setNickname(const std::string& nick);
        // void receiveMessages();

    private:
        int _socket;
        std::string _nickname;
};

#endif