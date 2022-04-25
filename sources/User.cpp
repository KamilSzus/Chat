//
// Created by kamil on 23.04.2022.
//

#include "../headers/User.h"
#include <iostream>

User::User(std::string userName)
    :m_userName(userName)
{}
std::string User::getName(const Ice::Current &) {
    return m_userName;
}

void User::receiveMessage(const std::string &message, const Chat::UserPrx &sender, const Chat::RoomPrx &room, const Ice::Current &) {
    std::cout << "Room: " <<room->getName() << "From " << sender->getName()<< " " << message << std::endl;
}

void User::receivePrivateMessage(const std::string &message, const Chat::UserPrx &sender, const Ice::Current &) {
    std::cout << "From: " << sender->getName() << ": " << message << std::endl;
}
