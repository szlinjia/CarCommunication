#ifndef _DISCAR_H
#define _DISCAR_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include "network.h"
#include "algorithm.h"

using namespace std;
class DisCar
{
public:
	DisCar();
	~DisCar();

	void RunStateMachine();

	void SetNetworkHelper(NetworkHelper* network);
	void SetAlgorithm(Algorithm* algorithm);

private:
	static const int State_Stop = 0;
	static const int State_Moving = 1;
	static const int State_RequestToCross = 2;
	static const int State_Waiting = 3;
	int mState;

	static const int Direction_LeftToRight = 0;
	static const int Direction_RightToLeft = 1;
	int mDirection;

	NetworkHelper* mNetwork;
	Algorithm *mAlgorithm;

	static const int TimeSec_MovingOnBridge = 9;
	static const int TimeSec_StopOutBridge = 2;
};
#endif // !_DISCAR_H
