//
// Created by kamil on 23.04.2022.
//

#ifndef CHAT_ROOMFACTORY_H
#define CHAT_ROOMFACTORY_H

#include <Ice/Ice.h>
#include <Chat.h>

class RoomFactory : Chat::RoomFactory{

private:


public:
    Chat::Room* addRoom(std::string name) throw (Chat::RoomAlreadyExists);
    void removeRoom(std::string name) throw (Chat::RoomNotExists);
    int getCPULoad();
};

#endif //CHAT_ROOMFACTORY_H
