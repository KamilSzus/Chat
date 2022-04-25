
#include <regex>
#include "../headers/Room.h"
#include "ClientMain.h"

int ClientMain::run(int argc, char **argv) {
    Ice::PropertiesPtr properties = communicator()->getProperties();
    int serverPort = properties->getPropertyAsIntWithDefault("server.port.set", 49152);
    Ice::ObjectPrx proxy = communicator()->stringToProxy("server:default -p " + std::to_string(serverPort));
    this->serverPrx = serverPrx.uncheckedCast(proxy);
    int portMin = properties->getPropertyAsIntWithDefault("client.port.range.min", 55555);
    int portMax = properties->getPropertyAsIntWithDefault("client.port.range.max", 65535);
    this->adapterPtr = getAdapter(portMin, portMax);
    this->adapterPtr->activate();
    listen();
    if (this->chatRoomPrx != ICE_NULLPTR) {
        this->chatRoomPrx->removeUser(this->userPrx);
    }
    communicator()->shutdown();
    communicator()->waitForShutdown();

    return EXIT_SUCCESS;
}

Ice::ObjectAdapterPtr ClientMain::getAdapter(int portMin, int portMax) {
    while (true)
    {
        int port = 54321;
        try
        {
            Ice::ObjectAdapterPtr ptr = communicator()->createObjectAdapterWithEndpoints(
                    "client_adapter",
                    "default -p " + std::to_string(port));
            return ptr;
        }
        catch (Ice::SocketException ex)
        {
            continue;
        }
    }
}

void ClientMain::listen() {
    std::cout << "Type /help to see list of available commands" << std::endl;
    for(;;)
    {
        std::string command;
        getline(std::cin, command);
        std::vector<std::string> tokenList = tokenize(command);
        command = tokenList.at(0);
        if (command == "/help")
        {
            showHelp();
        }
        else if (command == "/rooms")
        {
            showRoomList();
        }
        else if (command == "/users")
        {
            showUserList();
        }
        else if (command == "/leave")
        {
            leaveRoom();
        }
        else if (command == "/join")
        {
            joinRoom(tokenList[1]);
        }
        else if (command == "/create")
        {
            createRoom(tokenList[1]);
        }
        else if (command == "/msgprv")
        {
            std::vector<std::string> args = tokenize(tokenList[1]);
            sendPrivateMessage(args[0], args[1]);
        }
        else if (command == "/msg")
        {
            sendMessage(tokenList[1]);
        }
        else if (command == "/quit")
        {
            break;
        }
        else if (!command.empty())
        {
            std::cout << "Unknown command" << std::endl;
        }
    }
}

void ClientMain::showHelp() {
    std::cout << "List of available commands:" << std::endl;
    std::cout << "/join <RoomName> - joins chat room" << std::endl;
    std::cout << "/leave - leaves current chat room" << std::endl;
    std::cout << "/rooms - lists available chat rooms" << std::endl;
    std::cout << "/create <RoomName> - creates chat room" << std::endl;
    std::cout << "/users - lists all users in chat room" << std::endl;
    std::cout << "/msg <message> - sends message to everyone in chat room" << std::endl;
    std::cout << "/msgprv <user> <message> - sends private message to specific user in chat room" << std::endl;
    std::cout << "/help - lists available commands" << std::endl;
    std::cout << "/quit - disconnects and exits application" << std::endl;
}

void ClientMain::leaveRoom() {
    if (this->chatRoomPrx == ICE_NULLPTR) {
        std::cerr << "You ain't connected to any chat room" << std::endl;
        return;
    }
    std::string name = this->chatRoomPrx->getName();
    this->chatRoomPrx->removeUser(this->userPrx);
    this->chatRoomPrx = ICE_NULLPTR;
    std::cout << "You left chat room " << name << std::endl;
}

void ClientMain::joinRoom(std::string &name) {
    if (name.empty())
    {
        std::cerr << "Chat room name not specified" << std::endl;
        return;
    }
    if (this->chatRoomPrx != ICE_NULLPTR && this->chatRoomPrx->getName() == name)
    {
        std::cerr << "You are already connected to this chat room" << std::endl;
        return;
    }
    Chat::roomList roomList = serverPrx->getRooms();
    for (auto it = std::begin(roomList); it != std::end(roomList); ++it)
    {
        Chat::RoomPrx room = *it;
        if (room->getName() == name)
        {
            if (this->chatRoomPrx != ICE_NULLPTR)
            {
                this->leaveRoom();
            }
            room->addUser(this->userPrx);
            this->chatRoomPrx = room;
            std::cout << "You joined chat room " << name << std::endl;
            return;
        }
    }
    std::cerr << "Chat room " << name << " not found" << std::endl;
}

void ClientMain::createRoom(std::string &name) {
    if (name.empty())
    {
        std::cerr << "Chat room name not specified" << std::endl;
        return;
    }
    try
    {
        this->serverPrx->addRoom(name);
        std::cout << "Chat room " << name << " created, join using /join " << name << " command" << std::endl;
    }
    catch (Chat::RoomAlreadyExists ex)
    {
        std::cerr << "Chat room with this name already exists, please choose another one" << std::endl;
    }
}

void ClientMain::sendPrivateMessage(std::string &username, std::string &message) {
    if (this->chatRoomPrx == ICE_NULLPTR)
    {
        std::cerr << "You ain't connected to any chat room" << std::endl;
        return;
    }
    if (username.empty())
    {
        std::cerr << "Username not specified" << std::endl;
        return;
    }
    if (message.empty())
    {
        std::cerr << "Message not specified" << std::endl;
        return;
    }
    if (username == this->userPrx->getName())
    {
        std::cerr << "Cannot send message to yourself, you fool!" << std::endl;
        return;
    }
    Chat::userList userList = this->chatRoomPrx->presentUsers();
    for (auto it = std::begin(userList); it != std::end(userList); ++it)
    {
        Chat::UserPrx user = *it;
        if (user->getName() == username)
        {
            user->receivePrivateMessage(message,this->userPrx);
            std::cout << "<" << this->userPrx->getName() << "><EVERYBODY> " << message << std::endl;
            return;
        }
    }
    std::cerr << "User " << username << " not found" << std::endl;
}

void ClientMain::sendMessage(std::string &message) {
    if (this->chatRoomPrx == ICE_NULLPTR)
    {
        std::cerr << "You ain't connected to any chat room" << std::endl;
        return;
    }
    if (message.empty())
    {
        std::cerr << "Message not specified" << std::endl;
        return;
    }
    chatRoomPrx->sendMessage(message,this->userPrx);
}

void ClientMain::showRoomList() {
    Chat::roomList roomList = serverPrx->getRooms();
    if (roomList.empty())
    {
        std::cout << "No chat rooms available" << std::endl;
        return;
    }
    std::cout << "Available chat rooms:" << std::endl;
    for (auto it = std::begin(roomList); it != std::end(roomList); ++it)
    {
        Chat::RoomPrx room = *it;
        std::cout << "Room: " << room->getName() << ", Users: " << room->presentUsers().size() << std::endl;
    }
}

void ClientMain::showUserList() {
    if (this->chatRoomPrx == ICE_NULLPTR)
    {
        std::cerr << "You ain't connected to any chat room" << std::endl;
        return;
    }
    Chat::userList userList = this->chatRoomPrx->presentUsers();
    if (userList.size() == 1)
    {
        std::cout << "You're the lonely guy" << std::endl;
        return;
    }
    std::cout << "Users online:" << std::endl;
    for (auto it = std::begin(userList); it != std::end(userList); ++it)
    {
        Chat::UserPrx user = *it;
        std::cout << user->getName() << std::endl;
    }
}

std::vector<std::string> ClientMain::tokenize(std::string &command) {
    command = std::regex_replace(command, std::regex("^ +| +$|( ) +"), "$1");
    std::vector<std::string> tokenList(2);
    int pos = command.find(" ");
    tokenList[0] = command.substr(0, pos);
    if (pos != std::string::npos)
    {
        tokenList[1] = command.substr(pos + 1);
    }
    return tokenList;
}

int main(int argc, char* argv[]) {
    ClientMain app;
    return app.main(argc, argv, "chat.prop");
}