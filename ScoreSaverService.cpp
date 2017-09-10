//
// Created by root on 9/10/17.
//

#include <fstream>
#include "ScoreSaverService.h"
#include "BPDispatcher.h"

ScoreSaverService::ScoreSaverService(string scoreFolder) {
    this->scoreFolder = scoreFolder;
}

ScoreSaverService::~ScoreSaverService() {

}

map<string, string> *ScoreSaverService::getScores() {

    map<string,string>* scores = new map<string,string>();
    string score;
    ifstream read(scoreFolder + "/scores.txt");


    while(getline(read, score)){
        vector<string> parsedScore = split(score, ' ');
        scores->insert(pair<string,string>(parsedScore[0],parsedScore[1]));
    }

    return scores;
}

void ScoreSaverService::incScore(string username) {

    map<string,string>* scores = getScores();

    if(scores->find(username) == scores->end()){
        scores->insert(pair<string,string>(username,to_string(1)));
        ofstream file;
        file.open(scoreFolder + "/scores.txt");

        map<string,string>::iterator it = scores->begin();
        for(;it!=scores->end();it++)
            file << it->first << ' ' << it->second << endl;

        file.close();
        delete scores;
        return;
    }

    int newScore = stoi(scores->find(username)->second);

    newScore++;
    scores->erase(username);
    scores->insert(pair<string,string>(username,to_string(newScore)));

    ofstream file;
    file.open(scoreFolder + "/scores.txt");

    map<string,string>::iterator it = scores->begin();
    for(;it!=scores->end();it++)
        file << it->first << ' ' << it->second << endl;

    file.close();
    delete scores;
}

vector<string> ScoreSaverService::split(string str, char delimiter){

    vector<string> internal;
    stringstream ss(str); // Turn the string into a stream.
    string tok;

    while(getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }

    return internal;
}