#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

void createMagicPacket(unsigned char packet[],unsigned int macAddress[])
{
	int i;
	//mac aaddress variable 
	unsigned char mac[6];
	
	//6 * 0xff part of the start packet
	for(i=0;i<6;i++)
	{
		packet[i] = 0xff;
		mac[i] = macAddress[i];
	}
	// Rest of the packet is MAC address of the pc
	for(i=1;i<=16;i++)
	{
		memcpy(&packet[i*6],&mac,6*sizeof(unsigned char));
	}
}

int main(int argc, const char* argv[]){
	char broadcastAddress[16] = "192.168.1.255";
	unsigned char packet[102];
	int broadcast = 1;
	int udpSocket;
	//Socket address
	struct sockaddr_in udpClient;
	struct sockaddr_in udpServer;
	int i = 0;
	if(argc < 2){
		printf("Usage:\n./wakeonlan <mac address> (<broadcast address>)\n");
		exit(EXIT_FAILURE);
	}
	if(argc > 2)
	{
		// Parse Broadcast Address
		i = sscanf(argv[2],"%d.%d.%d.%d",&i,&i,&i,&i);
		if(i == 4)
		{
			strcpy(broadcastAddress, argv[2]);
		}
		printf("Broadcast address %s will be used.\n", broadcastAddress);
		createMagicPacket(packet,mac);
		udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
		if(setsockopt(udpSocket, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast) == -1){
			printf("Failed to setup a broadcast socket.\n");
			exit(EXIT_FAILURE);
		}
		udpClient.sin_family = AF_INET;
		udpClient.sin_addr.s_addr = INADDR_ANY;
		udpClient.sin_port = 0;
		//bind mac ip
		bind(udpSocket, (struct sockaddr*) &udpClient, sizeof(udpClient));
		udpServer.sin_family = AF_INET;
		udpServer.sin_addr.s_addr = inet_addr(broadcastAddress);
		udpServer.sin_port = htons(9);
		
		//send packet
		sendto(udpSocket, &packet, sizeof(unsigned char) * 102, 0, (struct sockaddr*) &udpServer, sizeof(udpServer));
	}
	printf("Wake up packet was sent.\n");
	exit(EXIT_SUCCESS);
	return 0;
}
