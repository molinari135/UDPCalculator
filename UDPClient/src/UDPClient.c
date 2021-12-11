/*
 ============================================================================
 Name        : UDPClient.c
 Author      : Ester Molinari
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : A simple UDP calculator in C (client)
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int main(void) {
	// output stream
	setvbuf(stdout, NULL, _IONBF, 0);

#if defined WIN32
	WSADATA wsa_data;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsa_data);

	if (iResult != 0) {
		errorhandler("Error at WSAStartup()\n");
		return EXIT_FAILURE;
	}
#endif

	// TODO input address and port
	int client_socket;
	struct sockaddr_in echoServerAddr;
	struct sockaddr_in fromAddr;
	int fromSize;
	char echoString[ECHOMAX];
	char echoBuffer[ECHOMAX];
	int echoStringLen;
	int respStringLen;

	// client's socket
	if ((client_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		errorhandler("socket() failed");
		return EXIT_FAILURE;
	}

	// TODO server's address resolution

	// setting server's address and port
	memset(&echoServerAddr, 0, sizeof(echoServerAddr));
	echoServerAddr.sin_family = PF_INET;
	echoServerAddr.sin_port = htons(DEFAULT_PORT);
	echoServerAddr.sin_addr.s_addr = inet_addr(DEFAULT_IP);

	// sending a message to server
	memset(echoString, 0, sizeof(echoString));
	printf("Insert echo string:\n");
	scanf("%s", echoString);

	if ((echoStringLen = strlen(echoString)) > ECHOMAX) {
		errorhandler("echo word too long");
	}

	if (sendto(client_socket, echoString, echoStringLen, 0, (struct sockaddr*)&echoServerAddr, sizeof(echoServerAddr)) != echoStringLen) {
		errorhandler("sento() sent different number of bytes than expected");
	}

	// receiving result from server
	fromSize = sizeof(fromAddr);
	respStringLen = recvfrom(client_socket, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&fromAddr, &fromSize);

	if (echoServerAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr) {
		fprintf(stderr, "Error: received a packet from unknown source.\n");
		return EXIT_FAILURE;
	}

	// TODO find server's canonical name and info

	// closing socket
	closesocket(client_socket);
	clearwinsock();

	return EXIT_SUCCESS;
}
