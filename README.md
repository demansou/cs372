# cs372
Class work for CS372 Computer Networks

## chatclient.c

Written in C. This program connects to chatserve.py and can send/receive messages once a connection is established. Can connect to any server with IPv4 address and server port in command line argument

### Compile

```
gcc -o chatclient chatclient.c
```

### Usage

Server will display IP address. Use that to connect chatclient to chatserve

```
chatclient [server ip] [server port]
```

### Quit
Once connection established, respond to server with

```
\quit
```

Can also use Ctrl+C to kill program.

## chatserve.py

Written in Python. This program opens a server connection that clients can connect to. Once connected, clients
can send/receive messages.

### Usage

```
python chatserve.py [listening port]
```

### Quit

Once connection established, respond to client with

```
\quit
```

Can also use Ctrl+C to kill program.

## README.md

Readme file for chatclient.c and chatserve.py

## BONUS POINTS

chatclient.c can connect to chatserve.py on any flip server. Address and IP not hardcoded into either program.
