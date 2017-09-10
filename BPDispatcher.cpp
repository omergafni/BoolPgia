#include <algorithm>
#include <Broker.h>
#include "BPDispatcher.h"

BPDispatcher::BPDispatcher() {
    connectedUsers = new map<string, string>();
    playingUsers = new map<string, string>();
    scoreSaver = new ScoreSaverService("/root/CLionProjects/BoolPgia/scores");
}

BPDispatcher::~BPDispatcher() {
    vector<TCPSocket *>::iterator begin = peers->getSockets()->begin();
    vector<TCPSocket *>::iterator end = peers->getSockets()->end();
    string close = "close";

    for (; begin != end; begin++) {
        (*begin)->write(close.c_str(), close.length());
    }

    delete connectedUsers;
    delete playingUsers;
    delete scoreSaver;
}

void BPDispatcher::readFromClients() {

    TCPSocket *talkingClient = peers->listenToSocket();

    if (talkingClient == NULL)
        return;

    string command = getCommandFromClient(talkingClient);

    cout << "[*] Received command: " + command + " from: " + usernameFromConnectedUsers(talkingClient) << endl;

    vector<string> parsedCommand = split(command, ' ');

    if (parsedCommand[0] == "play-with")
        playGame(talkingClient, parsedCommand[1]);

    else if (command == "play-random")
        playRandomGame(talkingClient);

    else if (command == "scores")
        showScores(talkingClient);

    else if (command == "players")
        showPlayers(talkingClient);

    else if (command == "exit") {
        removeClientFromDispatcher(talkingClient);
        removeClientFromConnectedUsers(usernameFromConnectedUsers(talkingClient));
    } else badRequest(talkingClient);
}

string BPDispatcher::getInstructions() {

    string instructions;

    instructions += "Bool Pgia instructions:\n";
    instructions += "--------------------------\n";
    instructions += "\tplay-with [username] : start a game with an opponent\n";
    instructions += "\tplay-random : start a game with a random opponent\n";
    instructions += "\tscores : show score-board\n";
    instructions += "\tplayers : show connected players\n";
    instructions += "\texit : disconnect\n";

    return instructions;
}

string BPDispatcher::getCommandFromClient(TCPSocket *client) {
    return readFromClient(client);
}

void BPDispatcher::playGame(TCPSocket *client, string opponentUsername) {

    sendToClient(client, "Trying to start game with: " + opponentUsername);

    TCPSocket *opponent = findOpponent(opponentUsername);

    if (opponent == NULL) {
        sendToClient(client, "Can't find " + opponentUsername);
        return;
    }
    if (opponent->equals(client)) {
        sendToClient(client, "You can't play against yourself");
        return;
    }

    moveToGameSession(client);
    moveToGameSession(opponent);

    bool acceptOffer = askOpponentToPlay(client, opponent);

    if (!acceptOffer) {
        sendToClient(client, "Your opponent refused to play with you");
        removeFromGameSession(client);
        removeFromGameSession(opponent);
    } else
        startBrokerBetweenClients(client, opponent);
}

void BPDispatcher::playRandomGame(TCPSocket *client) {

    string opponent = "";

    map<string, string>::iterator it = connectedUsers->begin();

    // Find first user that's not you
    for (; it != connectedUsers->end(); it++)
        if (it->second != getIpPortFromClient(client))
            opponent = it->first;

    // No opponents found
    if (opponent.size() == 0) {
        sendToClient(client, "No one to play with");
        return;
    }

    // Found opponent
    playGame(client, opponent);
}

void BPDispatcher::showScores(TCPSocket *client) {
    map<string, string> *scores = scoreSaver->getScores();

    map<string, string>::iterator it = scores->begin();
    string scoreRow;
    for (; it != scores->end(); it++) {
        scoreRow += it->first + " " + it->second + "\n";
    }
    sendToClient(client, scoreRow);

    delete scores;
}

vector<string> BPDispatcher::split(string str, char delimiter) {

    vector<string> internal;
    stringstream ss(str); // Turn the string into a stream.
    string tok;

    while (getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }

    return internal;
}

void BPDispatcher::showPlayers(TCPSocket *client) {

    string playerList;

    map<string, string>::iterator it = connectedUsers->begin();

    for (; it != connectedUsers->end(); it++)
        playerList += it->first + " | " + it->second + "\n";

    sendToClient(client, playerList);
}

void BPDispatcher::addClientToMap(TCPSocket *client, string userName) {
    string userIpPort = getIpPortFromClient(client);
    connectedUsers->insert(pair<string, string>(userName, userIpPort));
}

string BPDispatcher::getIpPortFromClient(TCPSocket *client) {
    string userPort = to_string(client->returnPort());
    string userIp = client->fromAddr();

    return userIp + ":" + userPort;
}

void BPDispatcher::sendToClient(TCPSocket *client, string message) {

    client->write(message.c_str(), message.size());
}

string BPDispatcher::readFromClient(TCPSocket *client) {
    char buff[1024];
    ::bzero(buff, sizeof(buff));
    client->read(buff, sizeof(buff));
    return string(buff);
}

bool BPDispatcher::askOpponentToPlay(TCPSocket *client, TCPSocket *opponent) {

    string clientName = playingUsers->find(getIpPortFromClient(client))->second;

    sendToClient(opponent, clientName + " would like to play with you, do you accept? Y\\N");

    string answer = readFromClient(opponent);

    for (;;) {
        if (answer == "Y" || answer == "y")
            return true;
        else if (answer == "N" || answer == "n")
            return false;
        else sendToClient(opponent, "Type Y\\N");
    }
}

void BPDispatcher::moveToGameSession(TCPSocket *client) {
    string username = usernameFromConnectedUsers(client);
    string address = getIpPortFromClient(client);

    cout << "[*] Moving to game session procedure: " << endl;
    removeClientFromDispatcher(client);
    removeClientFromConnectedUsers(username);
    addClientToPlayingUsers(username, address);
}

void BPDispatcher::removeFromGameSession(TCPSocket *client) {
    string username = usernameFromPlayingUsers(client);
    string address = getIpPortFromClient(client);
    cout << "[*] Removing from game session procedure: " << endl;

    addClientToDispatcher(client);
    removeClientFromPlayingUsers(username, address);
    addClientToConnectedUsers(username, address);
}

void BPDispatcher::addClientToConnectedUsers(string username, string address) {
    cout << "\t[+] Adding " + username + " to available users list" << endl;
    connectedUsers->insert(pair<string, string>(username, address));
}

void BPDispatcher::removeClientFromConnectedUsers(string username) {
    cout << "\t[-] Removing " + username + " from available users list" << endl;
    connectedUsers->erase(username);
}

void BPDispatcher::addClientToPlayingUsers(string username, string address) {
    cout << "\t[+] Adding " + username + " to playing users list" << endl;
    playingUsers->insert(pair<string, string>(address, username));
}

void BPDispatcher::removeClientFromPlayingUsers(string username, string address) {
    cout << "\t[-] Removing " + username + " from playing users list" << endl;
    playingUsers->erase(address);
}

void BPDispatcher::returnClientAfterSession(TCPSocket *client) {
    sendToClient(client, "Session ended, returning to lobby");
    removeFromGameSession(client);
}

TCPSocket *BPDispatcher::findOpponent(string username) {
    string opponentIpAddress = connectedUsers->find(username)->second;

    if (opponentIpAddress.size() == 0)
        return NULL;

    else {
        vector<string> ipPort = split(opponentIpAddress, ':');
        cout << "[*] Found opponent: " + username + " " + ipPort[0] + ":" + ipPort[1] << endl;
        return peers->findSocket(ipPort[0], stoi(ipPort[1]));
    }
}

void BPDispatcher::startBrokerBetweenClients(TCPSocket *client, TCPSocket *opponent) {
    cout << "***** Starting broker in BP Dispatcher *****" << endl;
    BPCLientHandler *ch = new BPCLientHandler(client, opponent);
    ch->setUserNames(usernameFromPlayingUsers(client), usernameFromPlayingUsers(opponent));
    Broker *broker = new Broker(client, opponent, this, ch);
    brokers->push_back(broker);
    broker->start();
}

string BPDispatcher::usernameFromConnectedUsers(TCPSocket *client) {
    string clientAddress = getIpPortFromClient(client);
    map<string, string>::iterator it = connectedUsers->begin();
    for (; it != connectedUsers->end(); it++)
        if (it->second == clientAddress)
            return it->first;
}

string BPDispatcher::usernameFromPlayingUsers(TCPSocket *client) {
    string clientAddress = getIpPortFromClient(client);
    return playingUsers->find(clientAddress)->second;
}



