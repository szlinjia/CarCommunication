#ifndef Network_h
#define Network_h

#ifndef MAX_CAR_NUM 
#define MAX_CAR_NUM 4
#endif

#define IPADDR "127.0.0.1"
#define PORTUI 9876
#define PORTNO 9877

class Global
{
public:
	static int ID;
};

typedef void (* NetworkCallback)(const char *message, void *userdata);

class NetworkHelper {
public:
	NetworkHelper(const char *IPForUI);
	~NetworkHelper();
	void SendMessageToCar(const char *message);
	void SendMessageToCar(int ID, const char *message);
	void RegisterCarMessageReceiver(NetworkCallback callback, void *userdata);
	
	void SendMessageToUI(const char *message);

private:
	static void* onThread (void *param);

	int mSockCar[MAX_CAR_NUM];
	struct sockaddr_in mSockAddr[MAX_CAR_NUM];

	int mSockForUI;
	struct sockaddr_in mSockAddrForUI;

	NetworkCallback mCallback;
	void *mUserdata;
	pthread_t mThread;
};

#endif
