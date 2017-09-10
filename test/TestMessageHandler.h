//
// Created by root on 9/10/17.
//

#ifndef BOOLPGIA_TESTMESSAGEHANDLER_H
#define BOOLPGIA_TESTMESSAGEHANDLER_H

#include <MessageHandler.h>
#include <TCPClient.h>

using namespace networkingLab;

class TestMessageHandler : public MessageHandler{

public:
    void handleMessage(string message);
};


#endif //BOOLPGIA_TESTMESSAGEHANDLER_H
