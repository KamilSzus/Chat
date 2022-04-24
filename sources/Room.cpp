//
// Created by kamil on 22.04.2022.
//

#include "../headers/User.h"
#include "../headers/Room.h"

Room::Room(std::string name)
    :m_name(std::move(name))
{}

Chat::userList Room::presentUsers(const Ice::Current &) {
    return m_userList;
}

void Room::sendMessage(const std::string &message, const Chat::UserPrx &sender, const Ice::Current &current) {
    for (auto &i : m_userList) {
        i._ptr->receiveMessage(message, sender, reinterpret_cast<IceProxy::Chat::Room *>(this),
                               (const Ice::Context &) current);
    }
}

void Room::addUser(const Chat::UserPrx &who, const Ice::Current &current) {
    if(!isUserOnList(who->getName(),current)) {
        m_userList.push_back(who);
    }
}

void Room::removeUser(const Chat::UserPrx &who, const Ice::Current &current) {
    if(isUserOnList(who->getName(),current)) {
        for (auto &i : m_userList) {
            if (i._ptr->getName() == who->getName()) {
                m_userList.erase(m_userList.begin() + i);
                break;
            }
        }
    }else{
        throw Chat::UserNotExists();
    }
}

std::string Room::getName(const Ice::Current &) {
    return m_name;
}

bool Room::isUserOnList(const std::string &name, const Ice::Current &current) {
    for(auto & i : m_userList){
        if(i._ptr->getName()==name){
            return true;
        }
    }
    return false;
}

//Chat::userList Room::presentUsers() {
//    return nullptr;
//}
//
//void Room::sendMessage(std::string message, Chat::User *sender) {
//
//}
//
//void Room::addUser(Chat::User *who) {
//
//}
//
//void Room::removeUser(Chat::User *who) throw(Chat::UserNotExists) {
//
//}
//
//std::string Room::getName() {
//    return std::string();
//}


