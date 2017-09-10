/*
 * BPCLientHandler.cpp
 *
 *  Created on: Sep 7, 2017
 *      Author: user
 */

#include "BPCLientHandler.h"
#include "BPServer.h"


BPCLientHandler::BPCLientHandler(TCPSocket* client1, TCPSocket* client2) {
	this-> client1 = client1;
	this-> client2 = client2;

	playingClients = new MultipleTCPSocketsListener();
	playingClients->addSocket(client1);
	playingClients->addSocket(client2);

    scoreSaver = new ScoreSaverService("/root/CLionProjects/BoolPgia/scores");
}

void BPCLientHandler::handleClients() {
	cout << "***** Calling handle clients from BP Client Handler *****" << endl;
	TCPSocket* speaker;
	string message;

	beginGameBetweenClients();

	while(gameIsRunning){

		speaker = playingClients->listenToSocket();

		if (speaker == NULL)
			continue;

		message = readFromClient(speaker);

		if(gameWonMessage(message)){
			cout << "Received \"game-won\" message from " << endl;
			writeWinnerScoreToDB(speaker);
		}
		else if (gameDisconnectedMessage(message)){
			cout << "Received \"disconnect\" message from " << endl;
			// something....
		}
		gameIsRunning=false;
	}
}

void BPCLientHandler::sendToClient(TCPSocket *client, string message) {
	client->write(message.c_str(), message.size());
}

string BPCLientHandler::readFromClient(TCPSocket *client) {
	char buff[1024];
	::bzero(buff,sizeof(buff));
	client->read(buff,sizeof(buff));
	return string(buff);
}

void BPCLientHandler::beginGameBetweenClients() {

    // Ask clients to their UDP ports
    sendToClient(client1,"udp-port?");
    string client1ListeningOn = readFromClient(client1);
    sendToClient(client2,"udp-port?");
    string client2ListeningOn = readFromClient(client2);

    // Execute start command
	cout << "Trying to run game between clients" << endl;
	sendToClient(client1, "start-match:"+client2->fromAddr()+":"+client2ListeningOn);
	sendToClient(client2, "start-match:"+client1->fromAddr()+":"+client1ListeningOn);
	gameIsRunning = true;
}

bool BPCLientHandler::gameWonMessage(string message) {
	std::size_t found = message.find("win");

	return found != std::string::npos;
}

bool BPCLientHandler::gameDisconnectedMessage(string message) {
	std::size_t found = message.find("exit");

	return found != std::string::npos;
}

void BPCLientHandler::writeWinnerScoreToDB(TCPSocket *winner) {
	cout << "Writing scores to DB..." << endl;
    string winnerName;
    if (winner->equals(client1))
        winnerName = clientName;
    else winnerName = opponentName;

    scoreSaver->incScore(winnerName);
}

BPCLientHandler::~BPCLientHandler() {
	delete playingClients;
    delete scoreSaver;
}

void BPCLientHandler::setUserNames(string clientName, string opponentName) {
    this->clientName = clientName;
    this->opponentName = opponentName;
}

