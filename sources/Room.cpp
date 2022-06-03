//
// Created by kamil on 22.04.2022.
//

#include "../headers/User.h"
#include "../headers/Room.h"

Room::Room(std::string name)
    : roomName(std::move(name))
{}

Chat::userList Room::presentUsers(const Ice::Current &) {
    return userList;
}

void Room::sendMessage(const std::string &message, const Chat::UserPrx &sender, const Ice::Current &current) {
    for (auto& chatUserPrx : userList)
    {
        Chat::RoomPtr roomPtr = this;
        Chat::RoomPrx roomPrx = Chat::RoomPrx::checkedCast(current.adapter->addWithUUID(roomPtr));
        chatUserPrx->receiveMessage(message, sender, roomPrx);
    }
}

void Room::addUser(const Chat::UserPrx &who, const Ice::Current &current) {
    userList.push_back(who);
    std::cout << "User " << who->getName() << " registered" << std::endl;
}

void Room::removeUser(const Chat::UserPrx &who, const Ice::Current &current) {
    auto it = std::find_if(userList.begin(), userList.end(), [&](Chat::UserPrx &userPrx) {
        return userPrx->getName() == who->getName();
    });

    if (it == userList.end())
        throw Chat::UserNotExists();

    userList.erase(it);
}

std::string Room::getName(const Ice::Current &) {
    return roomName;
}

bool Room::isUserOnList(const std::string &name, const Ice::Current &current) {
    auto it = std::find_if(userList.begin(), userList.end(), [&](Chat::UserPrx &userPrx) {
        return userPrx->getName() == name;
    });

    return it != userList.end();
}



