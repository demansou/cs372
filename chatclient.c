#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>

#define DEBUG 1

char **connectionAddress(int argc, char *argv[])
{
	if(argc != 2)
	{
		fprintf(stderr, "[chatclient] ERROR! Correct format 'chatclient [port]'\n");
		exit(2);
	}
	char **connection_address = (char **)calloc(2, sizeof(char *));
	connection_address[0] = "localhost";
	connection_address[1] = argv[1];
#if DEBUG
	fprintf(stderr, "[connectionAddress()] server connect: %s:%d\n", connection_address[0], atoi(connection_address[1]));
#endif
	return connection_address;
}

int socketInit()
{
	return socket(AF_INET, SOCK_STREAM, 0);
}

void socketConnect(int sockfd, char **connection_address)
{
	struct sockaddr_in serv_addr;
	if(sockfd < 0)
	{
		fprintf(stderr, "[chatclient] ERROR opening socket...\n");
		exit(1);
	}
	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(connection_address[0]);
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
	int sockfd = socketInit();
	socketConnect(sockfd, connection_address);
	socketHandshake(sockfd, connection_address);
	return sockfd;
}

int main(int argc, char *argv[])
{
	int sockfd = socketOpen(argc, argv);
	return 0;
}
