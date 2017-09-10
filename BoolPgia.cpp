#include <iostream>
#include "AuthService.h"
#include "test/RunTest.h"

using namespace std;


int main(int argc, char *argv[]) {

    if (argc == 2) {
        string flag = argv[1];
        if (flag == "-test") {
            RunTest test;
            test.start();
            return 0;
        }
    }

    BPServer *server = new BPServer();
    string adminInput;
    server->start();

    while (true) {
        getline(cin, adminInput);
        if (adminInput == "shutdown")
            break;
    }

    server->close();
    delete server;

    return 0;
}
