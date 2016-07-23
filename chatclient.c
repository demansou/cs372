#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>

#define TEST 0

// global variable because
char *userName = NULL;

char **connectionAddress(int argc, char *argv[])
{
	if(argc != 3)
	{
		fprintf(stderr, "[chatclient] ERROR! Correct format 'chatclient [server] [port]'\n");
		exit(2);
	}
	char **connection_address = (char **)calloc((size_t)2, sizeof(char *));
	connection_address[0] = argv[1];
	connection_address[1] = argv[2];
	fprintf(stderr, "Client attempting to connect to: %s:%d\n", connection_address[0], atoi(connection_address[1]));
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
		close(sockfd);
		exit(1);
	}
	return;
}

void makeUserName()
{
	userName = (char *)calloc(16, sizeof(char));
	fprintf(stderr, "Please enter a username (Max 12 characters): ");
	fgets(userName, 13, stdin);
	strtok(userName, "\n");
	strcat(userName, ": ");
#if TEST
	fprintf(stderr, "[DEBUG] Username: %s\n", userName);
#endif
	return;
}

char *getUserName()
{
	return userName;
}

void socketHandshake(int sockfd, char **connection_address)
{
	size_t messageLength = (size_t)strlen(connection_address[1]) + (size_t)strlen(userName);
	char *handshakeMessage = NULL;
	handshakeMessage = (char *)calloc(messageLength, sizeof(char));
	strcpy(handshakeMessage, getUserName());
	strcat(handshakeMessage, connection_address[1]);
	if(write(sockfd, handshakeMessage, messageLength) < 0)
	{
		fprintf(stderr, "[chatclient] ERROR handshaking with server...\n");
		close(sockfd);
		exit(1);
	}
	return;
}

int socketOpen(int argc, char *argv[])
{
	char **connection_address = (char **)calloc((size_t)2, sizeof(char *));
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
	fprintf(stderr, "[DEBUG] getting username...\n");
#endif
	makeUserName();
#if TEST
	fprintf(stderr, "[DEBUG] handshaking with server...\n");
#endif
	socketHandshake(sockfd, connection_address);
	return sockfd;
}

char *readMessage(int sockfd)
{
	char *serverMessage = NULL;
	serverMessage = (char *)calloc((size_t)128, sizeof(char));
	if(read(sockfd, serverMessage, (size_t)128) < 0)
	{
		fprintf(stderr, "[chatclient] ERROR reading message from server...\n");
		close(sockfd);
		exit(1);
	}
	return serverMessage;
}

char *writeMessage(int sockfd)
{
	char *clientMessage = NULL;
	char *userInput = NULL;
	size_t inputSize = 112;
	// messages can be of length 128 including username 
	userInput = (char *)calloc(112, sizeof(char));
	fprintf(stderr, "%s", userName);
	getline(&userInput, &inputSize, stdin);
	//fgets(userInput, sizeof(userInput) - 1, stdin);
	strtok(userInput, "\n");
	clientMessage = (char *)calloc((size_t)128, sizeof(char));
	strcpy(clientMessage, userName);
	strcat(clientMessage, userInput);
	if(write(sockfd, clientMessage, (size_t)128) < 0)
	{
		fprintf(stderr, "[chatclient] ERROR sending message size to server...\n");
		close(sockfd);
		exit(1);
	}
	return clientMessage;
}

void socketConnection(sockfd)
{
	char *serverMessage = NULL;
	char *clientMessage = NULL;
	while(1)
	{
		serverMessage = readMessage(sockfd);
#if TEST
		fprintf(stderr, "[DEBUG] server message: %s\n", serverMessage);
#endif
		if(strstr(serverMessage, "\\quit") != NULL)
		{
			fprintf(stderr, "[chatclient] SERVER has closed the chat connection...\n");
			free(serverMessage);
			break;
		}
		printf("%s\n", serverMessage);
		free(serverMessage);
		clientMessage = writeMessage(sockfd);
#if TEST
		fprintf(stderr, "[DEBUG] client message: %s\n", clientMessage);
#endif
		if(strstr(clientMessage, "\\quit") != NULL)
		{
			fprintf(stderr, "[chatclient] CLIENT has closed the chat connection...\n");
			free(clientMessage);
			break;
		}
		free(clientMessage);
	}
	close(sockfd);
	return;
}

int main(int argc, char *argv[])
{
	int sockfd = socketOpen(argc, argv);
	socketConnection(sockfd);
	return 0;
}
