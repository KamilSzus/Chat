
#include <Ice/Ice.h>
#include <Room.h>
#include <User.h>

int main(int argc, char* argv[])
{
    int status = 0;
    Ice::CommunicatorPtr ic;
    try {
        ic = Ice::initialize(argc, argv);
        Ice::ObjectPrx base = ic->stringToProxy("SimplePrinter:default -p 10000");
        Chat::RoomPrx room = Chat::RoomPrx::checkedCast(base);
        if (!room)
            throw "Invalid proxy";
        room->getName();
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