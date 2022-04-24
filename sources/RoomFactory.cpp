//
// Created by kamil on 23.04.2022.
//

#include "RoomFactory.h"

::Ice::Int RoomFactory::getCPULoad(const Ice::Current &current) {
    return 0;
}

void RoomFactory::removeRoom(const std::string &name, const Ice::Current &current) {

}

Chat::RoomPrx RoomFactory::addRoom(const std::string &name, const Ice::Current &current) {
    return Chat::RoomPrx();
}
