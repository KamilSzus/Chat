#ifndef CHAT_CLIENTMAIN_H
#define CHAT_CLIENTMAIN_H

#include <Ice/Application.h>

class ClientMain : public Ice::Application {

public:
    int run(int argc, char **argv) override;

private:
    Ice::ObjectAdapterPtr adapterPtr;
    Chat::UserPrx userPrx;
    Chat::ServerPrx serverPrx;
    Chat::RoomPrx roomPrx;

    Ice::ObjectAdapterPtr getAdapter(int portMin, int portMax);

    void listen();

    void showHelp();

    void leaveRoom();

    void joinRoom(std::string &name);

    void createRoom(std::string &name);

    void sendPrivateMessage(std::string &username, std::string &message);

    void sendMessage(std::string &message);

    void showRoomList();

    void showUserList();

    Chat::UserPrx createUser();

    std::vector<std::string> tokenize(std::string &command);

};

#endif //CHAT_CLIENTMAIN_H
