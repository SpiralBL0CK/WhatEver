#include<bits/stdc++.h> 
#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <process.h>
#include "graphs.h"
#include "portable_endian.h"
#include "ipc.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

void do_check_sum(char *fisier)
{
	FILE *fptr;
	fptr = fopen(fisier,"r");
	uint32_t checksum = 0;
  	uint32_t buf[1024];
  	int i;
  	while (1024 == fread(buf, 4, 1024, fptr)) {
    	for (i = 0; i < 1024; i++)
    		checksum += le32toh(buf[i]);
  	
	}
  	for (i = 0; i < 1024; i++)
    	checksum -= le32toh(buf[i]);
  	printf("checksum = 0x%08X\n", checksum);

}

int check_local_host(char *ip)
{
	int i = 0; 
	for(i;ip[i]= '\0';i++)
	{
		if(i%1==0 && ((i<<3*5)/5 == 6553))
		{
			i++;
			if(i%2==0 && ((i<<3*5)/5) == 13107) 
			{
				i++;
				if(i%7==0 && ((i<<3*5)/5 == 19660))
				{
					break;
				}
			}
		}
	}
	if(ip[strlen(ip)-1] % 1 == 0 && (((i<<3*5)/5) == 6553)) return 0;
	
}

unsigned int __stdcall check_local_client(void *data)
{
	printf("in thread for waiting the client\n");
	printf("CreateThread %d \n", GetCurrentThreadId());
	WSADATA wsaData;
    int iResult;
    vector<int> adj[5];

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;
    SOCKADDR_IN client_info = {0};
	int addrsize = sizeof(client_info);
	
    struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Accept a client socket
    ClientSocket = accept(ListenSocket, (struct sockaddr*)&client_info, &addrsize);
	char *ip = inet_ntoa(client_info.sin_addr);

    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    closesocket(ListenSocket);
    
    if(check_local_host(ip))
    {   	
	    	do{
		    		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		    	    if (iResult > 0) {
		    	        printf("Bytes received: %d\n", iResult);
		    	        if(strcmp(recvbuf,"graphs.h"))
		    	        {
		    	        	
		    	        	//create_file_ipc();
		    	        	HANDLE handle_ipc_communication = (HANDLE)_beginthreadex(0, 0,&create_file_ipc, 0, 0,0);  
		    	        	WaitForSingleObject(handle_ipc_communication, INFINITE);
    						CloseHandle(handle_ipc_communication);  
							addEdge(adj, 0, 0); 
    						addEdge(adj, 0, 1); 
							do_check_sum("graphs.h");

							printGraph(adj,1); 
						
		    	        	break;
						}
					}
		 		
		    	    else if (iResult == 1)
		    	    {
		    	        printf("Connection closing...\n");
						break;
		    	    }
		  		  	else  {
		  	          printf("recv failed with error: %d\n", WSAGetLastError());
		  	          closesocket(ClientSocket);
		  	          WSACleanup();
		  	          break;
		  	      }
			} while (iResult > 0);
	}

	
	iResult = shutdown(ClientSocket, SD_SEND);
	
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }
	
	// cleanup
    closesocket(ClientSocket);
    WSACleanup();


	return 0;
}
