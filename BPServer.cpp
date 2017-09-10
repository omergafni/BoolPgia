#include <TCPClient.h>
#include "BPServer.h"
#include "BPLoginService.h"

BPServer::BPServer() : TCPServer(new BPDispatcher(), SERVER_PORT) {
    authService = new AuthService("/root/CLionProjects/BoolPgia/users/");
}

string BPServer::serverWelcomeMessage() {

    string message ="--------------------------------------------------------\n";

    message+= "______             _           ______     _       \n"
            "| ___ \\           | |          | ___ \\   (_)      \n"
            "| |_/ / ___   ___ | |  ______  | |_/ /_ _ _  __ _ \n"
            "| ___ \\/ _ \\ / _ \\| | |______| |  __/ _` | |/ _` |\n"
            "| |_/ / (_) | (_) | |          | | | (_| | | (_| |\n"
            "\\____/ \\___/ \\___/|_|          \\_|  \\__, |_|\\__,_|\n"
            "                                     __/ |        \n"
            "                                    |___/         \n";

    message+="Authors: Leon Kotler & Omer Gafni\n";
    message+="--------------------------------------------------------\n\n";

    return message;
}

BPServer::~BPServer() {
    delete authService;
    delete dispatcher;
}

void BPServer::run() {
    TCPSocket *connectedClient = NULL;
    cout << "server is alive and listening on port: " << socket->returnPort() << endl;
    startDispatcher();

    while (isRunning) {
        connectedClient = socket->listenAndAccept();
        sendToClient(connectedClient, serverWelcomeMessage());
        loginService(connectedClient);
    }
}

void BPServer::loginService(TCPSocket *client) {

    BPLoginService* loginService = new BPLoginService(this);
    loginService->setClient(client);
    loginService->start();
}

void BPServer::loginClient(TCPSocket *client) {
    sendToClient(client,"Login: \n");
    sendToClient(client,"Username:");
    string username = readFromClient(client);

    sendToClient(client, "Password:");
    string password = readFromClient(client);

    while(!authService->canLogIn(username,password)){
        sendToClient(client, "Wrong credentials\n");
        sendToClient(client,"Username:");
        username = readFromClient(client);

        sendToClient(client, "Password:");
        password = readFromClient(client);

    }
    sendToClient(client, "Logged in successfully!\n\n");
    addClientToBPDispatcher(client,username);
}

void BPServer::registerClient(TCPSocket *client) {
    sendToClient(client,"Register:\n");
    sendToClient(client,"Username: ");
    string username = readFromClient(client);

    sendToClient(client, "Password: ");
    string password = readFromClient(client);

    while(!authService->registerUser(username,password)){

        sendToClient(client, "Username already taken");
        sendToClient(client,"Username:\n");
        username = readFromClient(client);

        sendToClient(client, "Password:\n");
        password = readFromClient(client);
    }
    sendToClient(client, "Registered successfully!");
    addClientToBPDispatcher(client,username);
}

void BPServer::addClientToBPDispatcher(TCPSocket *connectedClient, const string &username) {
    cout << "Server: client connected [" << connectedClient->fromAddr() << ":" << connectedClient->returnPort() << "]"
         << endl;
    dispatcher->addClientToDispatcher(connectedClient);

    ((BPDispatcher *) dispatcher)->addClientToMap(connectedClient, username);

    sendToClient(connectedClient,dispatcher->getInstructions());
}

void BPServer::sendToClient(TCPSocket *client, string message) {
    client->write(message.c_str(), message.size());
}

string BPServer::readFromClient(TCPSocket *client) {
    char buff[1024];
    ::bzero(buff,sizeof(buff));
    client->read(buff,sizeof(buff));
    return string(buff);
}

