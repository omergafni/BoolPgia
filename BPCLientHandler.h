/*
 * BPCLientHandler.h
 *
 *  Created on: Sep 7, 2017
 *      Author: user
 */

#ifndef BPCLIENTHANDLER_H_
#define BPCLIENTHANDLER_H_

#include <MultipleTCPSocketsListener.h>
#include "ClientHandler.h"
#include "TCPSocket.h"
#include "ScoreSaverService.h"

using namespace networkingLab;

class BPCLientHandler : public ClientHandler {
private:
	TCPSocket* client1;
	string clientName;
	TCPSocket* client2;
	string opponentName;
	MultipleTCPSocketsListener* playingClients;
	ScoreSaverService* scoreSaver;
public:
	BPCLientHandler(TCPSocket* client1, TCPSocket* client2);
	virtual void handleClients();
	virtual ~BPCLientHandler();

	void sendToClient(TCPSocket *client, string message);
	string readFromClient(TCPSocket *client);
	void beginGameBetweenClients();
	bool gameIsRunning;
	bool gameWonMessage(string message);
	bool gameDisconnectedMessage(string message);
	void writeWinnerScoreToDB(TCPSocket *winner);
	void setUserNames(string clientName, string opponentName);
};

#endif /* BPCLIENTHANDLER_H_ */
