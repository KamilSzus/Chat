#include "../headers/Room.h"
#include "../headers/Server.h"

Chat::roomList Server::getRooms(const Ice::Current &current) {
    return roomList;
}

Chat::RoomPrx Server::getRoom(const std::string &name, const Ice::Current &current) {
    auto it = std::find_if(roomList.begin(), roomList.end(), [&](Chat::RoomPrx &roomPrx) {
        return roomPrx->getName() == name;
    });

    if (it == roomList.end())
        return ICE_NULLPTR;

    return roomList.at(std::distance(roomList.begin(), it));
}

Chat::RoomPrx Server::addRoom(const std::string &name, const Ice::Current &current) {
    auto newRoom = getRoom(name,current);
    if (newRoom != ICE_NULLPTR)
        throw Chat::RoomAlreadyExists();

    Chat::RoomPrx roomPrx = Chat::RoomPrx::uncheckedCast(current.adapter->addWithUUID(new Room(name)));
    roomList.push_back(roomPrx);

    return roomPrx;
}

void Server::removeRoom(const std::string &name, const Ice::Current &current) {
    auto it = std::find_if(roomList.begin(), roomList.end(), [&](Chat::RoomPrx &roomPrx) {
        return roomPrx->getName() == name;
    });

    if (it == roomList.end())
        throw Chat::RoomNotExists();

    roomList.erase(it);
}

void Server::addNewFactory(const Chat::RoomFactoryPrx &factory, const Ice::Current &current) {
    m_roomFactoryList.push_back(factory);
}
