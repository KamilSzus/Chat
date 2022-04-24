#include "../heders/Room.h"
#include "../heders/Server.h"

Chat::roomList Server::getRooms(const Ice::Current &current) {
    return m_list;
}

Chat::RoomPrx Server::getRoom(const std::string &name, const Ice::Current &current) {
    Chat::RoomPrx expectedValue = nullptr;
    for(auto & i : m_list){
        if(i._ptr->getName()==name){
            expectedValue = i;
            break;
        }
    }

    return expectedValue;
}

Chat::RoomPrx Server::addRoom(const std::string &name, const Ice::Current &current) {
    auto newRoom = getRoom(name,current);
    if(!newRoom){
        Room* room = new Room(name);
        m_list.push_back(reinterpret_cast<IceProxy::Chat::Room *>(room));
    }else{
        throw Chat::RoomAlreadyExists();
    }

    return m_list.back();
}

void Server::removeRoom(const std::string &name, const Ice::Current &current) {
    auto newRoom = getRoom(name,current);
    if(!newRoom){
        for(auto & i : m_list) {
            if (i._ptr->getName() == name) {
                m_list.erase(m_list.begin() + i);
                break;
            }
        }
    }else{
        throw Chat::RoomNotExists();
    }

}

void Server::addNewFactory(const Chat::RoomFactoryPrx &factory, const Ice::Current &current) {

}
