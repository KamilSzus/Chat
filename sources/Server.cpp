#include "../headers/Room.h"
#include "../headers/Server.h"

Chat::roomList Server::getRooms(const Ice::Current &current) {
    return m_list;
}

Chat::RoomPrx Server::getRoom(const std::string &name, const Ice::Current &current) {
    Chat::RoomPrx expectedValue = nullptr;
    for(auto & i : m_list){
        if(i->getName()==name){
            expectedValue = i;
            break;
        }
    }

    return expectedValue;
}

Chat::RoomPrx Server::addRoom(const std::string &name, const Ice::Current &current) {
    auto newRoom = getRoom(name,current);
    if(!newRoom){
        Chat::RoomPtr roomPtr = new Room(name);
        Chat::RoomPrx roomPrx = Chat::RoomPrx::uncheckedCast(current.adapter->addWithUUID(roomPtr));
        m_list.push_back(roomPrx);
    }else{
        throw Chat::RoomAlreadyExists();
    }

    return m_list.back();
}

void Server::removeRoom(const std::string &name, const Ice::Current &current) {
    auto newRoom = getRoom(name,current);
    if(!newRoom){
        for(auto & i : m_list) {
            if (i->getName() == name) {
                m_list.erase(m_list.begin() + i);
                break;
            }
        }
    }else{
        throw Chat::RoomNotExists();
    }

}

void Server::addNewFactory(const Chat::RoomFactoryPrx &factory, const Ice::Current &current) {
    m_roomFactoryList.push_back(factory);
}
