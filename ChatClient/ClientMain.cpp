
#include <regex>
#include "../headers/User.h"
#include "ClientMain.h"

int ClientMain::run(int argc, char **argv) {

    Ice::PropertiesPtr properties = communicator()->getProperties();
    int serverPort = properties->getPropertyAsIntWithDefault("server.port.set", 49152);
    int portMin = properties->getPropertyAsIntWithDefault("client.port.range.min", 55555);
    int portMax = properties->getPropertyAsIntWithDefault("client.port.range.max", 65535);

    Ice::ObjectPrx proxy = communicator()->stringToProxy("server:default -p " + std::to_string(serverPort));
    connectedServerProxy = Chat::ServerPrx::uncheckedCast(proxy);
    m_adapterPtr = getAdapter(portMin, portMax);
    m_adapterPtr->activate();

    createUser();
    commandLoop();

    if (joinedRoomProxy != ICE_NULLPTR) {
        joinedRoomProxy->removeUser(myUserProxy);
    }
    communicator()->shutdown();
    communicator()->waitForShutdown();

    return EXIT_SUCCESS;
}

Ice::ObjectAdapterPtr ClientMain::getAdapter(int portMin, int portMax) {
    while (true)
    {
        srand(time(nullptr));
        int port = portMin + rand() % (portMax - portMin);
        try
        {
            Ice::ObjectAdapterPtr ptr = communicator()->createObjectAdapterWithEndpoints(
                    "client_adapter",
                    "default -p " + std::to_string(port));
            return ptr;
        }
        catch (Ice::SocketException &ex)
        {
            continue;
        }
    }
}

void ClientMain::commandLoop() {

    showRoomList();

    std::cout << "Commands:\n" << "/leave /join /new /dm /delete /quit\n" << "While in room, type anything to send message\n";

    while(true)
    {
        std::string command;
        getline(std::cin, command);

        std::vector<std::string> tokenList = buffer(command);
        std::string tokenCommand = tokenList.at(0);

        if (tokenCommand == "/leave" || tokenCommand == "/l")
        {
            leaveRoom();
        }
        else if (tokenCommand == "/join" || tokenCommand == "/j")
        {
            joinRoom(tokenList[1]);
        }
        else if (tokenCommand == "/create" || tokenCommand == "/new")
        {
            createRoom(tokenList[1]);
        }
        else if (tokenCommand == "/dm")
        {
            std::vector<std::string> args = buffer(tokenList[1]);
            sendPrivateMessage(args[0], args[1]);
        }
        else if (tokenCommand == "/delete" || tokenCommand == "/del")
        {
            deleteRoom(tokenList[1]);
        }
        else if (tokenCommand == "/quit" || tokenCommand == "/q")
        {
            break;
        }
        else if (tokenCommand.at(0) != '/')
        {
            sendMessage(command);
        }
        else if (!tokenCommand.empty())
        {
            std::cout << "Unknown tokenCommand" << std::endl;
        }

    }
}

void ClientMain::leaveRoom() {
    if (joinedRoomProxy == ICE_NULLPTR) {
        std::cerr << "You ain't connected to any chat room" << std::endl;
        return;
    }

    std::string name = joinedRoomProxy->getName();
    joinedRoomProxy->removeUser(myUserProxy);
    joinedRoomProxy = ICE_NULLPTR;
    std::cout << "You left chat room " << name << std::endl;
}

void ClientMain::joinRoom(std::string &name) {
    if (name.empty())
    {
        std::cerr << "Room name not specified." << std::endl;
        return;
    }
    if (joinedRoomProxy != ICE_NULLPTR && joinedRoomProxy->getName() == name)
    {
        std::cerr << "You are already connected to this room." << std::endl;
        return;
    }

    Chat::RoomPrx room = connectedServerProxy->getRoom(name);
    if (room == ICE_NULLPTR)
        std::cerr << "Room " << name << " not found" << std::endl;
    else {
        if (joinedRoomProxy != ICE_NULLPTR)
            leaveRoom();

        room->addUser(myUserProxy);
        joinedRoomProxy = room;
        std::cout << "You have joined room " << name << std::endl;

        showUserList();
    }
}

void ClientMain::createRoom(std::string &name) {
    if (name.empty())
    {
        std::cerr << "Chat room name not specified" << std::endl;
        return;
    }

    try
    {
        connectedServerProxy->addRoom(name);
        std::cout << "Room " << name << " created." << std::endl;
    }
    catch (Chat::RoomAlreadyExists &ex)
    {
        std::cerr << "Room with this name already exists." << std::endl;
    }
}

void ClientMain::sendPrivateMessage(std::string &username, std::string &message) {
    if (joinedRoomProxy == ICE_NULLPTR) {
        std::cerr << "You ain't connected to any chat room" << std::endl;
        return;
    }
    if (username.empty()) {
        std::cerr << "Username not specified" << std::endl;
        return;
    }
    if (message.empty()) {
        std::cerr << "Message not specified" << std::endl;
        return;
    }
    if (username == myUserProxy->getName()) {
        std::cerr << "Cannot send message to yourself" << std::endl;
        return;
    }
    Chat::userList userList = joinedRoomProxy->presentUsers();
    for (auto it = std::begin(userList); it != std::end(userList); ++it) {
        Chat::UserPrx user = *it;
        if (user->getName() == username) {
            user->receivePrivateMessage(message, myUserProxy);
            return;
        }
    }
    std::cerr << "User " << username << " not found" << std::endl;
}

void ClientMain::sendMessage(std::string &message) {
    if (joinedRoomProxy == ICE_NULLPTR) {
        std::cerr << "You ain't connected to any chat room" << std::endl;
        return;
    }
    if (message.empty()) {
        std::cerr << "Message not specified" << std::endl;
        return;
    }
    Chat::userList userList = joinedRoomProxy->presentUsers();
    for (auto it = std::begin(userList); it != std::end(userList); ++it) {
        Chat::UserPrx user = *it;
        user->receiveMessage(message, myUserProxy, joinedRoomProxy);
    }
}

void ClientMain::showRoomList() {
    Chat::roomList roomList = connectedServerProxy->getRooms();
    if (roomList.empty())
    {
        std::cout << "No rooms available. Create first room!" << std::endl;
        return;
    }

    std::cout << "Chat rooms:" << std::endl;
    for (auto it = std::begin(roomList); it != std::end(roomList); ++it)
    {
        Chat::RoomPrx room = *it;
        std::cout << "\t" << room->getName() << std::endl;
    }
}

void ClientMain::showUserList() {
    if (joinedRoomProxy == ICE_NULLPTR)
    {
        std::cerr << "You must join room first" << std::endl;
        return;
    }

    Chat::userList userList = joinedRoomProxy->presentUsers();

    std::cout << "Users in the joined room:" << std::endl;
    for (auto it = std::begin(userList); it != std::end(userList); ++it)
    {
        Chat::UserPrx user = *it;
        std::cout << "\t" << user->getName() << std::endl;
    }
}

Chat::UserPrx ClientMain::createUser() {
    std::string username;

    std::cout << "Input username: " << std::endl;
    std::getline(std::cin, username);

    Chat::UserPtr userPtr = new User(username);
    myUserProxy = Chat::UserPrx::uncheckedCast(m_adapterPtr->addWithUUID(userPtr));

    return myUserProxy;
}

void ClientMain::deleteRoom(std::string &name) {
    if (name.empty())
    {
        std::cerr << "Room name is empty" << std::endl;
        return;
    }

    try
    {
        connectedServerProxy->removeRoom(name);
        std::cout << "Room " << name << " deleted." << std::endl;
    }
    catch (Chat::RoomNotExists &ex)
    {
        std::cerr << "Room does not exists" << std::endl;
    }
}

std::vector<std::string> ClientMain::buffer(std::string &command) {
    command = std::regex_replace(command, std::regex("^ +| +$|( ) +"), "$1");
    std::vector<std::string> tokenList(2);
    size_t pos = command.find(' ');

    tokenList[0] = command.substr(0, pos);
    if (pos != std::string::npos)
    {
        tokenList[1] = command.substr(pos + 1);
    }
    return tokenList;
}

int main(int argc, char* argv[]) {
    ClientMain app;
    return app.main(argc, argv, "../chat.prop");
}