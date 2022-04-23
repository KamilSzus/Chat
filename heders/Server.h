#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include <Ice/Ice.h>
#include <chat.h>


class Server : Chat::Server {

private:

public:
    Chat::Room* getRoom(std::string name);
    Chat::roomList getRooms();
    Chat::Room* addRoom(std::string name) throw(Chat::RoomAlreadyExists);
    void removeRoom(std::string name) throw(Chat::RoomNotExists);
    void addNewFactory(Chat::RoomFactory* factory);
};


#endif
