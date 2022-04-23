//
// Created by kamil on 23.04.2022.
//

#ifndef CHAT_USER_H
#define CHAT_USER_H

#include <Ice/Ice.h>
#include <chat.h>

class User : Chat::User {

private:


public:
    std::string getName();
    void receiveMessage(std::string message, Chat::User* sender, Chat::Room* room);
    void receivePrivateMessage(std::string message, Chat::User* sender);

};


#endif //CHAT_USER_H
