/*
 * BPServer.h
 *
 *  Created on: Sep 7, 2017
 *      Author: user
 */

#ifndef BPSERVER_H_
#define BPSERVER_H_

#include <AuthService.h>
#include "TCPServer.h"
#include "BPDispatcher.h"
#include "GameProtocol.h"

using namespace networkingLab;

class BPServer : public TCPServer{
private:
	AuthService* authService;
	virtual string serverWelcomeMessage();

public:
	BPServer();
	virtual ~BPServer();
	virtual void run();

	string readFromClient(TCPSocket *client);

	void sendToClient(TCPSocket *client, string message);

	void addClientToBPDispatcher(TCPSocket *connectedClient, const string &username);

	void loginClient(TCPSocket *client);

	void registerClient(TCPSocket *client);

	void loginService(TCPSocket *client);
};

#endif /* BPSERVER_H_ */
