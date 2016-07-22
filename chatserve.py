import sys
import socket

DEBUG = True


def serverAddress(argv):
    """
    Checks command line args and assigns IP and Port args to variables
    """
    if len(sys.argv) != 2:
        print >> sys.stderr, "[chatserve] ERROR! Correct format 'chatserve [port]'"
        # exit 2 for command line syntax errors
        sys.exit(2)
    ip = 'localhost'
    port = int(sys.argv[1])
    if DEBUG:
        print >> sys.stderr, '[serverAddress()] %s:%d' % (ip, port)
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
    server_address = (ip, port)
    sock.bind(server_address)
    return sock


def socketOpen(argv):
    """
    Initialize and Bind IP and Port to Socket
    """ 
    ip, port = serverAddress(argv)
    sock = socketInit()
    sock = socketBind(sock, ip, port)
    return sock


def socketListen(sock):
    """
    Listens on Socket Port
    """
    sock.listen(1)
    while True:
        client_connection, client_address = sock.accept()
        response = socketConnection(client_connection, client_address)
        if response == -1:
            sock.shutdown(SHUT_RDWR)
            sock.close()
            break
    return


def socketConnection(connection, address):
    """
    Establishes TCP connection with connection data and client address
    """
    try:
        if DEBUG:
            # print to stderr in case of output redirect
            sys.stderr.write('TCP connection request from %s...' % client_addr)
        while True:
            client_data = connection.recv(4096)
            if client_data:
                print("%s" % client_data)
                server_data = input('Server reply: ')
                server_data = 'SERVER: ' + server_data
                connection.sendall(server_data)
                if re.match(r'\Quit$', client_data) or re.match(r'\Quit$', server_data):
                    print >> sys.stderr, 'TCP connection closing'
                    break
    finally:
        connection.close()


def main(argv):
    server_sock = socketOpen(argv)
    socketListen(server_sock)
    sys.exit(0)


if __name__ == "__main__":
    main(sys.argv)

