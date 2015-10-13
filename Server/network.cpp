/*
Class name: NetworkHelper
Author: Li Lin
History: 03/2015
Describe: This class is a helper model to provide network socket service.
In the project, UDP protocol is used for its efficient transportation.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//using namespace std;
#include <iostream>
#include "network.h"

using namespace std;

#define DBG //printf

int Global::ID = -1;

void *NetworkHelper::onThread(void *param)
{
	NetworkHelper *inst = (NetworkHelper *)param;
	assert(inst != NULL);
	int carID = Global::ID;

	while(true) 
	{
		char message[24] = {0};
		
		int len = sizeof(struct sockaddr_in);
		int ret = recvfrom(inst->mSockCar[carID], message, 
			sizeof(message), 0, (struct sockaddr *)&inst->mSockAddr[carID], (socklen_t*)&len);
		
		if(ret > 0) 
		{
			DBG("[car%d]Network.onReceived(): %s\n", carID, message);
			
			if(inst->mCallback != NULL) 
			{
				inst->mCallback(message, inst->mUserdata);
			}
		}
	}
}
	
NetworkHelper::NetworkHelper(const char *IPForUI)
{
	mSockForUI = socket(AF_INET, SOCK_DGRAM, 0);
	assert (mSockForUI != -1);
		
	bzero (&mSockAddrForUI, sizeof(mSockAddrForUI));
	mSockAddrForUI.sin_family = AF_INET;
	mSockAddrForUI.sin_port = htons(PORTUI);
	mSockAddrForUI.sin_addr.s_addr = inet_addr(IPForUI);
		
	for (int i = 0; i < MAX_CAR_NUM; i++)
	{
		mSockCar[i] = socket (AF_INET, SOCK_DGRAM, 0);
		assert (mSockCar[i] != -1);
		
		bzero (&mSockAddr[i], sizeof(mSockAddr[i]));
		mSockAddr[i].sin_family = AF_INET;
		mSockAddr[i].sin_port = htons (PORTNO + i);
		if (i != Global::ID) { 
			mSockAddr[i].sin_addr.s_addr = inet_addr (IPADDR);
		}
		else {        
			mSockAddr[i].sin_addr.s_addr = htonl (INADDR_ANY);

			int on = 1;
			int ret = setsockopt (mSockCar[i], SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
			if (-1 == ret) {
				std::cout << "socket set sharing port failed!" << std::endl;
				exit(0);
			}
			
			ret = bind (mSockCar[i], (struct sockaddr *)&mSockAddr[i], sizeof(mSockAddr[i]));
			if (-1 == ret) {
				std::cout << "socket bind failed!" << std::endl;
				exit(0);
			}
		}
	}

	int ret = pthread_create(&mThread, NULL, onThread, (void *)this);
	assert(0 == ret);
}

NetworkHelper::~NetworkHelper()
{
}

void NetworkHelper::SendMessageToCar(int ID, const char *message)
{
	if (ID == Global::ID || mSockCar[ID] == -1) return;
	
	int ret = sendto (mSockCar[ID], message, strlen(message), 0, 
		(struct sockaddr *)&mSockAddr[ID], sizeof(struct sockaddr_in));
	assert(ret >= 0);

	DBG("[car%d]Network.sendto_car%d(%s)\n", Global::ID, ID, message);
}

void NetworkHelper::SendMessageToCar(const char *message)
{
	for (int i = 0; i < MAX_CAR_NUM; i++) 
	{
		if (i != Global::ID && mSockCar[i] != -1) 
		{
			int ret = sendto(mSockCar[i], message, strlen (message), 0, 
				(struct sockaddr *)&mSockAddr[i], sizeof(struct sockaddr_in));
			assert(ret >= 0);
		}
	}

	DBG("[car%d]Network.sendtoAllCars(%s)\n", Global::ID, message);
}

void NetworkHelper::RegisterCarMessageReceiver(NetworkCallback callback, void *userdata)
{
	mCallback = callback;
	mUserdata = userdata;
}

void NetworkHelper::SendMessageToUI(const char *message)
{
	int ret = sendto (mSockForUI, message, strlen(message), 0, 
		(struct sockaddr *)&mSockAddrForUI, sizeof(struct sockaddr_in));
	//assert(ret >= 0);

	//DBG("[car%d]Network.sendtoUI(%s) %s\n", Global::ID, message, (ret>0) ? "success":"failure");
}