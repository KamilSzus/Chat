

#ifndef CHAT_ROOM_H
#define CHAT_ROOM_H

#include <Ice/Ice.h>
#include <chat.h>

class Room : Chat::Room {

private:


public:
    Chat::userList presentUsers();
    void sendMessage(std::string message, Chat::User* sender);
    void addUser(Chat::User* who);
    void removeUser(Chat::User* who) throw (Chat::UserNotExists);
    std::string getName();
};


#endif //CHAT_ROOM_H
