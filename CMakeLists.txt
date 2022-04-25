cmake_minimum_required(VERSION 3.16)
project(Chat)

set(CMAKE_CXX_STANDARD 14)

include_directories(headers)
include_directories(sources)

add_compile_options(-pthread -lIce)

add_executable(Chat
        headers/Room.h
        headers/Server.h
        headers/User.h
        headers/RoomFactory.h
        headers/Chat.h
        sources/Room.cpp
        sources/Server.cpp
        sources/User.cpp
        sources/RoomFactory.cpp
        sources/Chat.cpp
        ChatClient/ClientMain.cpp
        ChatServer/ServerMain.h ChatClient/ClientMain.h)
