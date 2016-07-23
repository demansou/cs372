"""
Name: Daniel Mansour
Creation Date: 07-16-2016
Program Name: chatserve.py
Description: Binds to port and listens for client connection request.
             If connection is established, allows chat between client
             and server. If client requests to close connetion with
             '\quit', server will listen for other connections.
"""
import sys
import socket
import re

TEST = False


def serverAddress(argv):
    """
    Checks command line args and assigns IP and Port args to variables
    """
    if len(sys.argv) != 2:
        print >> sys.stderr, "[chatserve] ERROR! Correct format 'chatserve [port]'"
        # exit 2 for command line syntax errors
        sys.exit(2)
    ip = socket.gethostbyname(socket.gethostname())
    port = int(sys.argv[1])
    if TEST:
        print >> sys.stderr, '[DEBUG]Server attempting to listen on %s:%d...' % (ip, port)
    return ip, port


def socketInit():
    """
    Initializes Socket
    """
    return socket.socket(socket.AF_INET, socket.SOCK_STREAM)


def socketBind(sock, ip, port):
    """
    Binds IP and Port to Socket
    """
    try:
        sock.bind((ip, port))
    except:
        print >> sys.stderr, '[chatserve] ERROR binding to localhost port'
        sys.exit(1)
    return sock


def socketOpen(argv):
    """
    Initialize and Bind IP and Port to Socket
    """ 
    ip, port = serverAddress(argv)
    sock = socketInit()
    sock = socketBind(sock, ip, port)
    print >> sys.stderr, 'Server listening on %s:%d' % (socket.gethostbyname(socket.gethostname()), int(sys.argv[1]))
    return sock


def socketListen(sock):
    """
    Listens on Socket Port
    """
    sock.listen(1)
    if TEST:
        print >> sys.stderr, '[DEBUG] listening for incoming connections'
    while True:
        client_connection, client_address = sock.accept()
        if TEST:
            print >> sys.stderr, '[DEBUG] connected to %s' % str(client_address)
        if socketConnection(client_connection) == -1:
            break
    return


def getMessage(connection):
    """
    Listens for data from client. Collects received client data.
    Returns client data.
    """
    client_message = connection.recv(128).decode()
    if not client_message:
        print >> sys.stderr, '[chatserve] ERROR recieving client data...'
        sys.exit(1)
    return client_message


def sendMessage(connection):
    """
    Gathers user input to send to client and sends message.
    Returns user input.
    """
    user_input = raw_input('SERVER: ')[:120]
    server_message = 'SERVER: ' + user_input
    connection.send(server_message)
    return server_message


def socketConnection(connection):
    """
    Holds connection with client. While loop allows exchange of
    messages between server and client. Will stop looping upon
    input of '\quit' from server or client
    """
    exit_message = '\quit'
    while True:
        client_message = ''
        client_message = getMessage(connection)
        print('%s' % client_message)
        if client_message.find(exit_message) != -1:
            print >> sys.stderr, '[chatserve] CLIENT has closed the chat connection...'
            connection.close()
            return 0
        server_message = ''
        server_message = sendMessage(connection)
        if server_message.find(exit_message) != -1:
            print >> sys.stderr, '[chatserve] SERVER has closed the chat connection...'
            connection.close()
            return -1
    connection.close()
    return 0


def main(argv):
    server_sock = socketOpen(argv)
    socketListen(server_sock)
    sys.exit(0)


if __name__ == "__main__":
    main(sys.argv)

