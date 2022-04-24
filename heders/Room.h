

#ifndef CHAT_ROOM_H
#define CHAT_ROOM_H

#include <Ice/Ice.h>
#include <Chat.h>

class Room : public Chat::Room {

private:
    std::string m_name;
    Chat::userList m_userList;

public:
    Room(std::string name);
    virtual Chat::userList presentUsers(const Ice::Current&) override;
    virtual void sendMessage(const ::std::string& message, const Chat::UserPrx& sender, const ::Ice::Current& current) override;
    virtual void addUser(const Chat::UserPrx& who, const ::Ice::Current& current) override;
    virtual void removeUser(const Chat::UserPrx& who, const ::Ice::Current& current) override;
    virtual std::string getName(const Ice::Current&) override;
    bool isUserOnList(const std::string &name, const Ice::Current &current);
};


#endif //CHAT_ROOM_H
