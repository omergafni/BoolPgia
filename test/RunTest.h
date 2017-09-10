/*
 * RunTest.h
 *
 *  Created on: Sep 7, 2017
 *      Author: user
 */

#ifndef RUNTEST_H_
#define RUNTEST_H_

using namespace std;

#include "../BPDispatcher.h"
#include "../BPServer.h"
#include "../BPCLientHandler.h"
#include "TCPClient.h"
#include "TestMessageHandler.h"

class RunTest {
public:
	RunTest();
	virtual ~RunTest();
	bool start();
};

#endif /* RUNTEST_H_ */
