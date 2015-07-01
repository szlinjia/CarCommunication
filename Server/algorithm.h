#ifndef Algorithm_h
#define Algorithm_h

#ifndef MAX_CAR_NUM 
#define MAX_CAR_NUM 4
#endif

class Algorithm
{
public:
	Algorithm(NetworkHelper *network);
	~Algorithm();
	
	virtual void RequestCS()=0;
	virtual void ReleaseCS()=0;
	virtual bool WaitingCS()=0;

	void RefreshTimestamp(int timestamp);
	
protected:
	int mTimestamp;
	NetworkHelper* mNetwork;
};

class RicartAgrawala: public Algorithm
{
public:
	RicartAgrawala(NetworkHelper *network);
	~RicartAgrawala();

	static const int Out = 0;
	static const int In  = 1;
	static const int Waiting = 2;

	static const int Request = 1;
	static const int Ack = 2;

	virtual void RequestCS();
	virtual void ReleaseCS();
	virtual bool WaitingCS();
	
protected:
	int mTimestampMyRequest;
	int mAckedBitsFiled;
	int mRequestBuffer[MAX_CAR_NUM];
	int mState;

	friend void NetworkReceiptCallback(const char *message, void *userdata);
};

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
	static const int TimeSec_StopOutBridge  = 2;
};
#endif
