#ifndef CHAT_CLIENTMAIN_H
#define CHAT_CLIENTMAIN_H

#include <Ice/Application.h>

class ClientMain : public Ice::Application {

public:
    int run(int argc, char **argv) override;

private:
    Ice::ObjectAdapterPtr m_adapterPtr;
    Chat::UserPrx m_userPrx;
    Chat::ServerPrx m_serverPrx;
    Chat::RoomPrx m_roomPrx;

    static Ice::ObjectAdapterPtr getAdapter(int portMin, int portMax);

    void listen();

    static void showHelp();

    void leaveRoom();

    void joinRoom(std::string &name);

    void createRoom(std::string &name);

    void sendPrivateMessage(std::string &username, std::string &message);

    void sendMessage(std::string &message);

    void showRoomList();

    void showUserList();

    Chat::UserPrx createUser();

    static std::vector<std::string> buffer(std::string &command);

};

#endif //CHAT_CLIENTMAIN_H
