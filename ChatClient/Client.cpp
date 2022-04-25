
#include <Ice/Ice.h>
#include "../headers/Room.h"
#include "../headers/User.h"

int main(int argc, char* argv[])
{
    int status = 0;
    Ice::CommunicatorPtr ic;
    try {
        ic = Ice::initialize(argc, argv);
        Ice::ObjectPrx base = ic->stringToProxy("Room:default -p 10000");
        Chat::RoomPrx room = Chat::RoomPrx::checkedCast(base);
        if (!room) {
            throw "Invalid proxy";
        }
        User* we = new User("We");
        Chat::UserPrx userPrx = (reinterpret_cast<IceProxy::Chat::User*>(we));
        room->addUser(userPrx);
        std::string test = room->getName();
        room->sendMessage("nowa",userPrx);
    } catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
        status = 1;
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
        status = 1;
    }
    if (ic)
        ic->destroy();
    return status;
}