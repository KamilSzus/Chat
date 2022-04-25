//
// Created by kamil on 25.04.2022.
//

#ifndef CHAT_SERVERMAIN_H
#define CHAT_SERVERMAIN_H

#include <Ice/Ice.h>
#include <Ice/Application.h>

class ServerMain : public Ice::Application
{
public:
    int run(int argc, char **argv) override;

};

#endif //CHAT_SERVERMAIN_H
