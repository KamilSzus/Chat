
#include <regex>
#include "../headers/User.h"
#include "../headers/Room.h"
#include "ClientMain.h"

int ClientMain::run(int argc, char **argv) {
    Ice::PropertiesPtr properties = communicator()->getProperties();
    int serverPort = properties->getPropertyAsIntWithDefault("server.port.set", 49152);
    Ice::ObjectPrx proxy = communicator()->stringToProxy("server:default -p " + std::to_string(serverPort));
    m_serverPrx = Chat::ServerPrx::uncheckedCast(proxy);
    int portMin = properties->getPropertyAsIntWithDefault("client.port.range.min", 55555);
    int portMax = properties->getPropertyAsIntWithDefault("client.port.range.max", 65535);
    m_adapterPtr = getAdapter(portMin, portMax);
    m_adapterPtr->activate();
    createUser();
    listen();
    if (m_roomPrx != ICE_NULLPTR) {
        m_roomPrx->removeUser(m_userPrx);
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
        std::cout << port << std::endl;
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
        std::vector<std::string> tokenList = buffer(command);
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
            std::vector<std::string> args = buffer(tokenList[1]);
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
    if (m_roomPrx == ICE_NULLPTR) {
        std::cerr << "You ain't connected to any chat room" << std::endl;
        return;
    }
    std::string name = m_roomPrx->getName();
    m_roomPrx->removeUser(m_userPrx);
    m_roomPrx = ICE_NULLPTR;
    std::cout << "You left chat room " << name << std::endl;
}

void ClientMain::joinRoom(std::string &name) {
    if (name.empty())
    {
        std::cerr << "Chat room name not specified" << std::endl;
        return;
    }
    if (m_roomPrx != ICE_NULLPTR && m_roomPrx->getName() == name)
    {
        std::cerr << "You are already connected to this chat room" << std::endl;
        return;
    }
    Chat::roomList roomList = m_serverPrx->getRooms();
    for (auto it = std::begin(roomList); it != std::end(roomList); ++it)
    {
        Chat::RoomPrx room = *it;
        if (room->getName() == name)
        {
            if (m_roomPrx != ICE_NULLPTR)
            {
                leaveRoom();
            }
            std::cout << m_userPrx->getName() << "\n";
            room->addUser(m_userPrx);
            m_roomPrx = room;
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
        m_serverPrx->addRoom(name);
        std::cout << "Chat room " << name << " created, join using /join " << name << " command" << std::endl;
    }
    catch (Chat::RoomAlreadyExists ex)
    {
        std::cerr << "Chat room with this name already exists, please choose another one" << std::endl;
    }
}

void ClientMain::sendPrivateMessage(std::string &username, std::string &message) {
    if (m_roomPrx == ICE_NULLPTR) {
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
    if (username == m_userPrx->getName()) {
        std::cerr << "Cannot send message to yourself, you fool!" << std::endl;
        return;
    }
    Chat::userList userList = m_roomPrx->presentUsers();
    for (auto it = std::begin(userList); it != std::end(userList); ++it) {
        Chat::UserPrx user = *it;
        if (user->getName() == username) {
            user->receivePrivateMessage(message, m_userPrx);
            return;
        }
    }
    std::cerr << "User " << username << " not found" << std::endl;
}

void ClientMain::sendMessage(std::string &message) {
    if (m_roomPrx == ICE_NULLPTR) {
        std::cerr << "You ain't connected to any chat room" << std::endl;
        return;
    }
    if (message.empty()) {
        std::cerr << "Message not specified" << std::endl;
        return;
    }
    Chat::userList userList = m_roomPrx->presentUsers();
    for (auto it = std::begin(userList); it != std::end(userList); ++it) {
        Chat::UserPrx user = *it;
        user->receiveMessage(message, m_userPrx, m_roomPrx);
    }
}

void ClientMain::showRoomList() {
    Chat::roomList roomList = m_serverPrx->getRooms();
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
    if (m_roomPrx == ICE_NULLPTR)
    {
        std::cerr << "You ain't connected to any chat room" << std::endl;
        return;
    }
    Chat::userList userList = m_roomPrx->presentUsers();
    if (userList.size() == 1)
    {
        std::cout << "You're the lonely guy" << std::endl;
        return;
    }
    std::cout << "Users online:" << std::endl;
    for (auto it = std::begin(userList); it != std::end(userList); ++it)
    {
        Chat::UserPrx user = *it;
        std::cout << user<<"\n";
        std::cout << user->getName() << std::endl;
    }
}

std::vector<std::string> ClientMain::buffer(std::string &command) {
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

Chat::UserPrx ClientMain::createUser() {
    std::string username;
    std::cout << "Please choose your username" << std::endl;
    for (;;) {
        std::cout << "Username: ";
        std::getline(std::cin, username);
        Chat::UserPtr userPtr = new User(username);
        m_userPrx = Chat::UserPrx::uncheckedCast(m_adapterPtr->addWithUUID(userPtr));

        return m_userPrx;
    }

}

int main(int argc, char* argv[]) {
    ClientMain app;
    return app.main(argc, argv, "../chat.prop");
}