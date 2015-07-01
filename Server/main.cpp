#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <algorithm>
#include "network.h"
#include "algorithm.h"

int main(int argc, char *argv[])
{	
	if (argc != 3) 
	{
		printf("usage: %s CarID(between 0 and 3(included)) UserIPAddr.\n", argv[0]);
		exit(0);
	}
	else
	{
		Global::ID = std::atoi(argv[1]);
	}
	
	NetworkHelper *network = new NetworkHelper(argv[2]);
	Algorithm *algorithm = new RicartAgrawala(network);
	DisCar *discar = new DisCar();

	discar->SetNetworkHelper(network);
	discar->SetAlgorithm(algorithm);

	usleep(1000);
	
	discar->RunStateMachine();
	
	return 0;
}
