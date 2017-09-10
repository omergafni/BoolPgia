//
// Created by root on 9/10/17.
//

#ifndef BOOLPGIA_SCORESAVERSERVICE_H
#define BOOLPGIA_SCORESAVERSERVICE_H

#include <string>
#include <map>
#include <vector>

using namespace std;
class ScoreSaverService {
private:
    string scoreFolder;
public:
    ScoreSaverService(string scoreFolder);
    ~ScoreSaverService();
    map<string,string>* getScores();
    void incScore(string username);

    std::vector<string> split(string str, char delimiter);
};


#endif //BOOLPGIA_SCORESAVERSERVICE_H
