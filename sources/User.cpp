//
// Created by kamil on 23.04.2022.
//

#include "../headers/User.h"
#include <iostream>

User::User(std::string userName)
    : userName(userName)
{}
std::string User::getName(const Ice::Current &) {
    return userName;
}

void User::receiveMessage(const std::string &message, const Chat::UserPrx &sender, const Chat::RoomPrx &room, const Ice::Current &) {
    std::cout << "[" << room->getName() << "] " << sender->getName()<< ": " << message << std::endl;
}

void User::receivePrivateMessage(const std::string &message, const Chat::UserPrx &sender, const Ice::Current &) {
    std::cout << "[DM] " << sender->getName() << ": " << message << std::endl;
}
