//
// Created by kamil on 24.04.2022.
//

#include <memory>
#include "../headers/Room.h"
#include "../headers/Server.h"
#include "ServerMain.h"

int ServerMain::run(int argc, char **argv) {
    Ice::PropertiesPtr properties = Ice::Application::communicator()->getProperties();
    int serverPort = properties->getPropertyAsIntWithDefault("server.port.set", 49152);

    Ice::ObjectAdapterPtr adapterPtr = Ice::Application::communicator()->createObjectAdapterWithEndpoints(
            "server_adapter",
            "default -p " + std::to_string(serverPort));

    Chat::ServerPtr serverPtr = new Server();
    adapterPtr->add(serverPtr, Ice::stringToIdentity("server"));
    adapterPtr->activate();

    std::cout << "Server running..." << std::endl;
    Ice::Application::communicator()->waitForShutdown();

    return EXIT_SUCCESS;
}


int main(int argc, char* argv[])
{
    ServerMain app;
    return app.main(argc, argv, "../chat.prop");
}