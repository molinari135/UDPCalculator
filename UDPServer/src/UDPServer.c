/*
 ============================================================================
 Name        : UDPServer.c
 Author      : Tommaso Perniola
 Version     : 1.4
 Copyright   : Your copyright notice
 Description : A simple UDP calculator in C (server)
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void calculator(char query[], char resp[]);

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
	struct hostent* client;
	int clientAddrLen = 0;
	char echoBuffer[ECHOMAX];
	char respBuffer[ECHOMAX];
	char* client_name;
	int recvMsgSize = 0;
	int bufferLen = 0;

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

		clientAddrLen = sizeof(echoClientAddr);
		memset(echoBuffer, 0, sizeof(echoBuffer));
		recvMsgSize = recvfrom(server_socket, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClientAddr, &clientAddrLen);

		// finding client's canonical name
		client = gethostbyaddr((char*)&echoClientAddr.sin_addr, 4, AF_INET);
		client_name = client->h_name;

		// handling math operation
		memset(respBuffer, 0, sizeof(respBuffer));
		calculator(echoBuffer, respBuffer);

		if (echoBuffer[0] != EQUALS) {
			// printing data received
			printf("Query '%s' from client %s, ip %s\n", echoBuffer, client_name, inet_ntoa(echoClientAddr.sin_addr));

			// sending result back to client
			strcpy(echoBuffer, respBuffer);

			printf("Result: %s\n", echoBuffer);

			bufferLen = strlen(echoBuffer);
			if (sendto(server_socket, echoBuffer, bufferLen, 0, (struct sockaddr*)&echoClientAddr, &clientAddrLen) != bufferLen) {
				errorhandler("sendto() sent different number of bytes than expected");
			}
		}
	}
}

void calculator(char query[], char resp[]) {
	char sign[0];
	float res = 0;
	int a = 0, b = 0;

	sscanf(query, "%s %d %d", sign, &a, &b);

	switch (sign[0]) {
	case '+':
		res = add(a, b);
		sprintf(resp, "%d %c %d = %.0f", a, query[0], b, res);
		break;
	case '-':
		res = sub(a, b);
		sprintf(resp, "%d %c %d = %.0f", a, query[0], b, res);
		break;
	case '/':
		res = division(a, b);
		sprintf(resp, "%d %c %d = %.2f", a, query[0], b, res);
		break;
	case '*':
		res = mult(a, b);
		sprintf(resp, "%d %c %d = %.0f", a, query[0], b, res);
		break;
	case '=':
		printf("\nClosing connection\n\n");
		sprintf(resp, "%s", "Connection closed");
		break;
	}
}
