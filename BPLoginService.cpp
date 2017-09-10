//
// Created by root on 9/10/17.
//

#include "BPLoginService.h"

BPLoginService::BPLoginService(BPServer *server) {
    this->server = server;
}

void BPLoginService::handle() {

    server->sendToClient(client, "Login or register to start the game\n"
            "[1] Login\n"
            "[2] Register\n");

    string userChoice = server->readFromClient(client);

    if (userChoice == "1")
        server->loginClient(client);
    else if (userChoice == "2")
        server->registerClient(client);
    else {
        server->sendToClient(client, "Enter [1] or [2]\n");
        handle();
    }
}

void BPLoginService::setClient(TCPSocket *client) {
    this->client = client;
}

void BPLoginService::run() {
    handle();
}
