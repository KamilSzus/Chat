//
// Created by kamil on 22.04.2022.
//

#include "../heders/Room.h"

Room::Room(std::string name)
    :m_name(std::move(name))
{}

Chat::userList Room::presentUsers(const Ice::Current &) {
    return m_userList;
}

void Room::sendMessage(const std::string &message, const Chat::UserPrx &sender, const Ice::Current &current) {

}

void Room::addUser(const Chat::UserPrx &who, const Ice::Current &current) {

}

void Room::removeUser(const Chat::UserPrx &who, const Ice::Current &current) {

}

std::string Room::getName(const Ice::Current &) {
    return m_name;
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


