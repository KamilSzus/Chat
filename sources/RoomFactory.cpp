#include <Ice/Config.h>
#include "../headers/Room.h"
#include "../headers/RoomFactory.h"

RoomFactory::RoomFactory()
        :m_numberOfChats(0)
{}

::Ice::Int RoomFactory::getCPULoad(const Ice::Current &current) {
    return m_numberOfChats;
}

void RoomFactory::removeRoom(const std::string &name, const Ice::Current &current) {
    if(!isRoomOnList(name)) {
        for(auto & i : roomList){
            if(i->getName()==name){
                std::cout<<i<<"number of elements before"<<m_numberOfChats<<"\n";
                roomList.erase(std::remove(roomList.begin(),roomList.end(),i),roomList.end());
                m_numberOfChats--;
                std::cout<<i<<"number of elements after"<<m_numberOfChats<<"\n";
                return;
            }
        }
    }else {
        throw Chat::RoomNotExists();
    }
}

Chat::RoomPrx RoomFactory::addRoom(const std::string &name, const Ice::Current &current) {
    if(!isRoomOnList(name)) {
        Chat::RoomPtr roomPtr = new Room(name);
        Chat::RoomPrx roomPrx = Chat::RoomPrx::uncheckedCast(current.adapter->addWithUUID(roomPtr));
        m_numberOfChats++;
        roomList.push_back(roomPrx);
        return roomPrx;
    }else {
        throw Chat::RoomAlreadyExists();
    }
}

bool RoomFactory::isRoomOnList(const std::string &name) {
    for(auto & i : roomList){
        if(i->getName()==name){
            return true;
        }
    }
    return false;
}

