//
// Created by kamil on 23.04.2022.
//

#include "User.h"

User::User(std::string userName)
    :m_userName(std::move(userName))
{}

std::string User::getName(const Ice::Current &) {
    return m_userName;
}

void User::receiveMessage(const std::string &message, const Chat::UserPrx &sender, const Chat::RoomPrx &room, const Ice::Current &) {
    
}

void User::receivePrivateMessage(const std::string &message, const Chat::UserPrx &sender, const Ice::Current &) {

}
