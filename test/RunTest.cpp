/*
 * RunTest.cpp
 *
 *  Created on: Sep 7, 2017
 *      Author: user
 */

#include <UDPSocket.h>
#include "RunTest.h"

RunTest::RunTest() {}

RunTest::~RunTest() {}

bool RunTest::start() {
	cout << "Running BoolPgia system test" << endl;

    TCPClient* client1 = new TCPClient(4444);
    TCPClient* client2 = new TCPClient(5555);
    client1->setTCPRecvThread(new TCPRecvThread(client1->getTcpSocket(),new TestMessageHandler()));
    client2->setTCPRecvThread(new TCPRecvThread(client2->getTcpSocket(),new TestMessageHandler()));

    // start and create server
    BPServer* server = new BPServer();
    server->start(); sleep(2);


    // connect client1
    client1->open(SERVER_IP,SERVER_PORT); sleep(3);
    client1->send("1"); sleep(1);
    client1->send("leon"); sleep(1);
    client1->send("12345"); sleep(1);

    // connect client2
    client2->open(SERVER_IP,SERVER_PORT); sleep(3);
    client2->send("1"); sleep(1);
    client2->send("omer"); sleep(1);
    client2->send("12345"); sleep(1);

    // start game
    client1->send("play-random"); sleep(1);

    // refuse game
    client2->send("N"); sleep(1);
    //client2->send("Y"); sleep(1);
    //client1->send("win"); sleep(1);


    // players cmd
    client1->send("players"); sleep(1);

    // scores cmd
    client1->send("scores"); sleep(1);

    client1->send("exit"); sleep(1);
    client2->send("exit"); sleep(1);

    server->close();
    delete server, client1, client2;

}
