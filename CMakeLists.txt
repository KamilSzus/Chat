cmake_minimum_required(VERSION 3.16)
project(Chat)

set(CMAKE_CXX_STANDARD 14)

include_directories(heders)
include_directories(sources)

add_executable(Chat
        temp/Chat.h
        heders/Room.h
        heders/Server.h
        temp/Chat.cpp
        sources/Room.cpp
        sources/Server.cpp sources/User.cpp heders/User.h sources/RoomFactory.cpp heders/RoomFactory.h ServerMain.cpp Client.cpp)
