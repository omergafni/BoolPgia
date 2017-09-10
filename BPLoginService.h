#ifndef BOOLPGIA_BPLOGINSERVICE_H
#define BOOLPGIA_BPLOGINSERVICE_H
#include <MThread.h>
#include <TCPSocket.h>
#include "BPServer.h"

using namespace networkingLab;

class BPLoginService : public MThread{
private:
    BPServer* server;
    TCPSocket* client;
    void handle();

public:
    BPLoginService(BPServer* server);
    void setClient(TCPSocket* client);
    void run();

};


#endif //BOOLPGIA_BPLOGINSERVICE_H
