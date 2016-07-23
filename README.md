# cs372
Class work for CS372 Computer Networks
#
chatclient.c: Written in C. This program connects to chatserve.py and can send/receive messages once a connection is established. Can connect to any server with IPv4 address and server port in command line argument\n
\tCompile: gcc -o chatclient chatclient.c\n
\tUsage: chatclient [server ip] [server port]\n
\tQuit: Once connection established, respond to server with '\quit'. Can also use Ctrl+C to kill program.\n
#
chatserve.py: Written in Python. This program opens a server connection that clients can connect to. Once connected, clients
can send/receive messages.\n
\tUsage: python chatserve.py [listening port]\n
\tQuit: Once connection established, respond to client with '\quit'. Can also use Ctrl+C to kill program.\n
#
README.md: Readme file for chatclient.c and chatserve.py\n
