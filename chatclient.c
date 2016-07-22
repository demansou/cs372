#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>

#define TEST 1

char **connectionAddress(int argc, char *argv[])
{
	if(argc != 3)
	{
		fprintf(stderr, "[chatclient] ERROR! Correct format 'chatclient [server] [port]'\n");
		exit(2);
	}
	char **connection_address = (char **)calloc(2, sizeof(char *));
	connection_address[0] = argv[1];
	connection_address[1] = argv[2];
	fprintf(stderr, "[connectionAddress()] server connection: %s:%d\n", connection_address[0], atoi(connection_address[1]));
	return connection_address;
}

int socketInit()
{
	return socket(AF_INET, SOCK_STREAM, 0);
}

void socketConnect(int sockfd, char **connection_address)
{
	struct sockaddr_in serv_addr;
	struct hostent *server;
	server = gethostbyname(connection_address[0]);
	if(sockfd < 0)
	{
		fprintf(stderr, "[chatclient] ERROR opening socket...\n");
		exit(1);
	}
	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(atoi(connection_address[1]));
	if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		fprintf(stderr, "[chatclient] ERROR connecting to server port...\n");
		exit(1);
	}
	return;
}

void socketHandshake(int sockfd, char **connection_address)
{
	size_t message_length = (size_t)strlen(connection_address[1]);
	if(write(sockfd, connection_address[1], message_length) < 0)
	{
		fprintf(stderr, "[chatclient] ERROR handshaking with server...\n");
		exit(1);
	}
	return;
}

int socketOpen(int argc, char *argv[])
{
	char **connection_address = (char **)calloc(2, sizeof(char *));
	connection_address = connectionAddress(argc, argv);
#if TEST
	fprintf(stderr, "[DEBUG] creating socket...\n");
#endif
	int sockfd = socketInit();
#if TEST
	fprintf(stderr, "[DEBUG] connecting to server...\n");
#endif
	socketConnect(sockfd, connection_address);
#if TEST
	fprintf(stderr, "[DEBUG] handshaking with server...\n");
#endif
	socketHandshake(sockfd, connection_address);
	return sockfd;
}

int main(int argc, char *argv[])
{
	int sockfd = socketOpen(argc, argv);
	return 0;
}
