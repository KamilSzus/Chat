//
// Created by kamil on 22.04.2022.
//

#include "../heders/Server.h"

Chat::Room *Server::getRoom(std::string name) {
    return nullptr;
}

Chat::roomList Server::getRooms() {
    return ;
}

Chat::Room *Server::addRoom(std::string name) throw(Chat::RoomAlreadyExists) {
    return nullptr;
}

void Server::removeRoom(std::string name) throw(Chat::RoomNotExists) {

}

void Server::addNewFactory(Chat::RoomFactory *factory) {

}

