/*
Class name: Discar
Author: Li Lin
History: 03/2015
Describe: This class is car core logic. There are four status in 
a car instance:stop, request,moving and waiting. Each status is 
control by recieving message from other car.
*/

#include "discar.h"

#define DBG printf
DisCar::DisCar()
{
	mState = State_Stop;
	if (Global::ID % 2)
	{
		mDirection = Direction_LeftToRight;
	}
	else
	{
		mDirection = Direction_RightToLeft;
	}
}

DisCar::~DisCar()
{
}

void DisCar::RunStateMachine()
{
	assert(mNetwork != NULL);
	assert(mAlgorithm != NULL);

	if (NULL == mNetwork || NULL == mAlgorithm) return;

	while (true)
	{
		mAlgorithm->RefreshTimestamp(-1);

		int state = mState;

		switch (state)
		{
		case State_Stop:
			sleep(TimeSec_StopOutBridge);
			DBG("[%d]StateMac: State_Stop -- > State_RequestToCross.\n", Global::ID);
			mState = State_RequestToCross;
			break;

		case State_RequestToCross:
			mAlgorithm->RequestCS();
			DBG("[%d]StateMac: State_RequestToCross -- > State_Waiting.\n", Global::ID);
			mState = State_Waiting;
			break;

		case State_Waiting:
			if (!mAlgorithm->WaitingCS())
			{
				DBG("[%d]StateMac: State_Waiting -- > State_Moving.\n", Global::ID);
				mState = State_Moving;
			}
			break;

		case State_Moving:
			sleep(TimeSec_MovingOnBridge);
			mAlgorithm->ReleaseCS();
			mDirection = 1 - mDirection;
			DBG("[%d]StateMac: State_Moving -- > State_Stop.\n", Global::ID);
			mState = State_Stop;
			break;

		default:
			assert(0);
			break;
		}

		if (mState != state)
		{
			char message[24] = { 0 };
			sprintf(message, "%d %d %d", Global::ID, mState, mDirection);
			mNetwork->SendMessageToUI(message);
		}
	}
}

void DisCar::SetNetworkHelper(NetworkHelper* network)
{
	mNetwork = network;
}

void DisCar::SetAlgorithm(Algorithm* algorithm)
{
	mAlgorithm = algorithm;
}
