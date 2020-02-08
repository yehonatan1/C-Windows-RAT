import socket


def server():
    s = socket.socket()
    s.bind(('127.0.0.1', 9855))

    s.listen(1)
    client_socket, addr = s.accept()
    print("Connection from: " + str(addr))

    message = input('-> ')
    while message != 'q':
        s.send(message.encode('utf-8'))
        data = s.recv(1024).decode('utf-8')
        print('Received from server: ' + data)
        message = input('==> ')
    s.close()


if __name__ == '__main__':
    server()
