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
    virtual Chat::RoomPrx addRoom(const ::std::string& name, const ::Ice::Current& current) override;
    virtual void removeRoom(const ::std::string& name, const ::Ice::Current& current) override;
    virtual ::Ice::Int getCPULoad(const ::Ice::Current& current) override;
};

#endif //CHAT_ROOMFACTORY_H
