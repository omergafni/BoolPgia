/*
 * BPDispatcher.h
 *
 *  Created on: Sep 7, 2017
 *      Author: user
 */

#ifndef BPDISPATCHER_H_
#define BPDISPATCHER_H_
#include "Dispatcher.h"
#include "BPCLientHandler.h"
#include "TCPSocket.h"
#include <vector>
#include <sstream>
#include <map>

using namespace std;
using namespace networkingLab;

class BPDispatcher : public Dispatcher{

public:
	BPDispatcher();
	virtual ~BPDispatcher();
	void addClientToMap(TCPSocket *client, string userName);
	ScoreSaverService* scoreSaver;

protected:
	map<string, string>* connectedUsers;
	map<string, string>* playingUsers;

	virtual void readFromClients();

	virtual string getInstructions();

	string getCommandFromClient(TCPSocket* client);

	void playGame(TCPSocket* client, string opponentUsername);

	void playRandomGame(TCPSocket* client);

	void showScores(TCPSocket* client);

	vector<string> split(string str, char delimiter);

	void showPlayers(TCPSocket *client);

	string getIpPortFromClient(TCPSocket *client);

	void sendToClient(TCPSocket *client, string message);

	string readFromClient(TCPSocket *client);

	void moveToGameSession(TCPSocket *client);

	void removeFromGameSession(TCPSocket *client);

	bool askOpponentToPlay(TCPSocket* client,TCPSocket *opponent);

	void removeClientFromConnectedUsers(string username);

	void addClientToPlayingUsers(string username, string address);

	void removeClientFromPlayingUsers(string username, string address);

	void addClientToConnectedUsers(string username, string address);

	virtual void returnClientAfterSession(TCPSocket* client);

	TCPSocket* findOpponent(string username);

	void startBrokerBetweenClients(TCPSocket *client, TCPSocket *opponent);

    string usernameFromConnectedUsers(TCPSocket *client);

	string usernameFromPlayingUsers(TCPSocket *client);
};

#endif /* BPDISPATCHER_H_ */
