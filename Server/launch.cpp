#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

#define MAX_CAR_NUM 4

static void simulateGUI(void)
{
	struct sockaddr_in sad;
	
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	assert (sockfd != -1);
		
	bzero (&sad, sizeof(sad));
	sad.sin_family = AF_INET;
	sad.sin_port = htons(9876);
	sad.sin_addr.s_addr = htonl (INADDR_ANY);

	int on = 1;
	/* allow multiple sockets to use the same PORT number */
	int ret = setsockopt (sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if (-1 == ret) 
	{
		std::cout << "simuUI: socket set sharing port failed!" << std::endl;
		exit(0);
	}
	
	/* bind address and port to socket */
	ret = bind (sockfd, (struct sockaddr *)&sad, sizeof(sad));
	if (-1 == ret) 
	{
		std::cout << "simuUI: socket bind failed!" << std::endl;
		exit(0);
	}

	while(true)
	{
		char message[24] = {0};
		
		int len = sizeof(struct sockaddr_in);
		int ret = recvfrom(sockfd, message, sizeof(message), 0, 
			(struct sockaddr *)&sad, (socklen_t*)&len);
		
		if(ret > 0) 
		{
			//printf("simulatedUI.MessageReceived(): %s\n", message);
		}
	}
}

int main(int argc, char *argv[])
{
	if (argc != 2) 
	{
		printf("usage: %s UserIPAddr.\n", argv[0]);
		exit(0);
	}
	
	printf("This process is to launch 4 distributed cars!\n");

	// kill all existing car processes, if any
	if(0 == fork()) 
	{
		execlp("/usr/bin/killall", "killall", "-9", "discar", NULL);
		exit(0);
	}

	sleep(2);

	// to launch 4 car processes now
	for(int i = 0; i < MAX_CAR_NUM; i++) 
	{
		char ID[8] = {0};
		sprintf(ID, "%d", i);
		if(0 == fork()) 
		{
			execlp("./discar", "discar", ID, argv[1], NULL);
			printf("fatal error happened! if you see this.\n");
			exit(-1);
		}

		printf("DisCar %s is created successfully!\n", ID);
	}

#if 0
	// for test without Android side.
	simulateGUI();
#else
	// parent process has nothing to do, goes infinite loop
	while(true) {};
#endif
	// this return will not ever happen
	printf("fatal error happened! if you see this, too.\n");
	return 0;
}