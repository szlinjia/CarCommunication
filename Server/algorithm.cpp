/*
Class name: RicartAgrawala
Author: Li Lin
History: 03/2015
Describe: This class implements Ricart Agrawal algorithm in a 
decentralize distributed system. Four cars send cross bridge 
request to each other and following the rule to access. There
is no central server to control the process. Each car order its
sequence by using total order.
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
#include <iostream>
#include "network.h"
#include "algorithm.h"

#define DBG printf
using namespace std;
Algorithm::Algorithm(NetworkHelper *network)
{
	mTimestamp = 0;
	mNetwork = network;
}

Algorithm::~Algorithm()
{
}
	
void Algorithm::RefreshTimestamp(int timestamp)
{
	if(-1 == timestamp) 
	{
		mTimestamp++;
	}
	else
	{
		mTimestamp = 1 + std::max(mTimestamp, timestamp);
	}
}

void NetworkReceiptCallback(const char *message, void *userdata)
{
	RicartAgrawala *ra = (RicartAgrawala *)userdata;

	if(NULL == ra) return;

	int fromID, msg, timestamp, direction;
	sscanf(message, "%d%d%d%d", &fromID, &msg, &timestamp, &direction);

	ra->RefreshTimestamp(timestamp);
		
	switch(msg)
	{
	case RicartAgrawala::Request:
		if(ra->Out == ra->mState)
		{
			char message[24];
			sprintf(message, "%d %d %d %d", Global::ID, ra->Ack, ra->mTimestamp, -1);
			ra->mNetwork->SendMessageToCar(message);
		}
		else if(ra->Waiting == ra->mState)
		{
			if(ra->mTimestampMyRequest > timestamp)
			{
				char message[24] = {0};
				sprintf(message, "%d %d %d %d", Global::ID, ra->Ack, ra->mTimestamp, -1);
				ra->mNetwork->SendMessageToCar(message);
			}
			else if(ra->mTimestampMyRequest == timestamp)
			{
				if(Global::ID > fromID)
				{
					char message[24] = {0};
					sprintf(message, "%d %d %d %d", Global::ID, ra->Ack, ra->mTimestamp, -1);
					ra->mNetwork->SendMessageToCar(message);
				}
			}
		}
		else if(ra->In == ra->mState)
		{
			for(int i = 0; i < MAX_CAR_NUM; i++)
			{
				if(ra->mRequestBuffer[i] == -1)
				{
					ra->mRequestBuffer[i] = fromID;
				}
			}
		}
		break;
		
	case RicartAgrawala::Ack:
		if(ra->Waiting == ra->mState)
		{
			ra->mAckedBitsFiled |= (1 << fromID);

			bool ackedFromAll = true;
			for(int i = 0; i < MAX_CAR_NUM; i++)
			{
				if(i != Global::ID)
				{
					if(!(ra->mAckedBitsFiled & (1 << i)))
					{
						ackedFromAll = false;
						break;
					}
				}
			}

			if(ackedFromAll)
			{
				for(int i = 0; i < MAX_CAR_NUM; i++)
				{
					ra->mRequestBuffer[i] = -1;
				}
				ra->mState = ra->In;
			}
		}
		break;

	default:
		break;
	}
}

RicartAgrawala::RicartAgrawala(NetworkHelper *network) : Algorithm(network)
{	
	mAckedBitsFiled = 0;
	mState = Out;
	mTimestampMyRequest = -1;
	
	for(int i = 0; i < MAX_CAR_NUM; i++)
	{
		mRequestBuffer[i] = -1;
	}

	if(mNetwork != NULL)
	{
		mNetwork->RegisterCarMessageReceiver(NetworkReceiptCallback, (void*)this);
	}
}

RicartAgrawala::~RicartAgrawala()
{
}

void RicartAgrawala::RequestCS()
{
	if(NULL == mNetwork || mState != Out) return;

	mTimestampMyRequest = mTimestamp;
	mAckedBitsFiled = 0;
	mState = Waiting;

	char message[24] = {0};
	sprintf(message, "%d %d %d %d", Global::ID, Request, mTimestamp, -1);
	mNetwork->SendMessageToCar(message);
}

void RicartAgrawala::ReleaseCS()
{
	if(NULL == mNetwork || mState != In) return;

	mAckedBitsFiled = 0;
	mTimestampMyRequest = -1;
	mState = Out;

	char message[24] = {0};
	sprintf(message, "%d %d %d %d", Global::ID, Ack, mTimestamp, -1);
	for(int i = 0; i < MAX_CAR_NUM; i++) 
	{
		if(mRequestBuffer[i] != -1) 
		{
			mNetwork->SendMessageToCar(i, message);
		}
	}
	
	for(int i = 0; i < MAX_CAR_NUM; i++)
	{
		mRequestBuffer[i] = -1;
	}
}

bool RicartAgrawala::WaitingCS()
{
	if(Waiting == mState)
	{
		return true;
	}
	else
	{
		return false;
	}
}

