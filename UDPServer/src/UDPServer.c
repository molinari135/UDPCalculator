/*
 ============================================================================
 Name        : UDPServer.c
 Author      : Ester Molinari
 Version     : 1.1
 Copyright   : Your copyright notice
 Description : A simple UDP calculator in C (server)
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

float calculator(char query[]) {
	char sign[0];
	int a = 0, b = 0, res = 0;

	sscanf(query, "%s %d %d", sign, &a, &b);

	switch (sign[0]) {
	case '+':
		res = add(a, b);
		break;
	case '-':
		res = sub(a, b);
		break;
	case '/':
		res = division(a, b);
		break;
	case '*':
		res = mult(a, b);
		break;
	case '=':
		printf("\nClosing connection\n\n");
		res = 0;
		break;
	}

	return res;
}

int main(void) {
	// output stream
	setvbuf(stdout, NULL, _IONBF, 0);

#if defined WIN32
	WSADATA wsa_data;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsa_data);
	if (iResult != 0) {
		errorhandler("error at WSAStartup\n");
		return EXIT_FAILURE;
	}
#endif

	int server_socket;
	struct sockaddr_in echoServerAddr;
	struct sockaddr_in echoClientAddr;
	int clientAddrLen = 0;
	char echoBuffer[ECHOMAX];
	char sign[0];
	char* exitMsg = "Closing connection";
	int recvMsgSize = 0;
	int exitMsgLen = 0;
	int bufferLen = 0;
	int result = 0;

	// server's socket
	if ((server_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		errorhandler("socket() failed");
		return EXIT_FAILURE;
	}

	// server's address
	memset(&echoServerAddr, 0, sizeof(echoServerAddr));
	echoServerAddr.sin_family = AF_INET;
	echoServerAddr.sin_port = htons(DEFAULT_PORT);
	echoServerAddr.sin_addr.s_addr = inet_addr(DEFAULT_IP);

	// socket binding
	if ((bind(server_socket, (struct sockaddr*)&echoServerAddr, sizeof(echoServerAddr))) < 0) {
		errorhandler("bind() failed");
		return EXIT_FAILURE;
	}

	// receiving a query from client
	while(1) {
		printf("Waiting for a client to connect...\n");
		// TODO finding client's canonical name

		do {
			clientAddrLen = sizeof(echoClientAddr);
			memset(echoBuffer, 0, sizeof(echoBuffer));
			recvMsgSize = recvfrom(server_socket, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClientAddr, &clientAddrLen);

			// handling math operation
			result = calculator(echoBuffer);

			if (echoBuffer[0] != EQUALS) {
				// printing data received
				printf("Data received from client: %s\n", echoBuffer);

				// sending result back to client
				memset(echoBuffer, 0, sizeof(echoBuffer));
				itoa(result, echoBuffer, 10);
				printf("Result: %s", echoBuffer);

				bufferLen = strlen(echoBuffer);
				if (sendto(server_socket, echoBuffer, bufferLen, 0, (struct sockaddr*)&echoClientAddr, &clientAddrLen) != bufferLen) {
					errorhandler("sendto() sent different number of bytes than expected");
				}
			} else {
				echoBuffer[0] = EQUALS;
			}
		} while (echoBuffer[0] != EQUALS);
	}
}
