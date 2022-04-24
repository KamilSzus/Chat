
#ifndef CHAT_USER_H
#define CHAT_USER_H

#include <Ice/Ice.h>
#include <Chat.h>

class User : public Chat::User {

private:
    std::string m_userName;

public:
    User(std::string userName);

    virtual std::string getName(const Ice::Current &) override;

    virtual void receiveMessage(const ::std::string& message, const Chat::UserPrx& sender, const Chat::RoomPrx& room, const ::Ice::Current&) override;

    virtual void receivePrivateMessage(const ::std::string& message, const Chat::UserPrx& sender, const ::Ice::Current&) override;

};


#endif //CHAT_USER_H
