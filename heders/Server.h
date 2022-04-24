#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include <Ice/Ice.h>
#include <Chat.h>
#include <cstdio>
#include <vector>


class Server : Chat::Server {
private:
    Chat::roomList m_list;

public:
    virtual Chat::RoomPrx getRoom(const ::std::string& name, const ::Ice::Current& current) override;
    virtual Chat::roomList getRooms(const ::Ice::Current& current) override;
    virtual Chat::RoomPrx addRoom(const ::std::string& name, const ::Ice::Current& current) override;
    virtual void removeRoom(const ::std::string& name, const ::Ice::Current& current) override;
    virtual void addNewFactory(const Chat::RoomFactoryPrx& factory, const ::Ice::Current& current) override;
};


#endif
