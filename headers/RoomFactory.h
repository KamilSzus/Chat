#ifndef CHAT_ROOMFACTORY_H
#define CHAT_ROOMFACTORY_H

#include <Ice/Ice.h>
#include "../headers/Chat.h"

class RoomFactory : public Chat::RoomFactory{

private:
    int m_numberOfChats;
    Chat::roomList roomList;
public:
    RoomFactory();
    virtual Chat::RoomPrx addRoom(const ::std::string& name, const ::Ice::Current& current) override;
    virtual void removeRoom(const ::std::string& name, const ::Ice::Current& current) override;
    virtual ::Ice::Int getCPULoad(const ::Ice::Current& current) override;
    bool isRoomOnList(const std::string &name);

};

#endif //CHAT_ROOMFACTORY_H
