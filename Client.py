import subprocess
import socket
import os

count = 0
count_3 = 0


def files_list(path):
    global count, count_3
    files = os.listdir(path)
    try files = os.listdir(path):

        for count in range(0, len(files), 1):
            if os.path.isdir(path + "\\" + files[count]):
                files_list(files[count])
            else:
                print(files[count])
    except FileNotFoundError:
        count += 1
        return files_list(files_list(files[count]))


def client():
    s = socket.socket()
    s.connect(('127.0.0.1', 9855))

    while True:
        data = s.recv(1024).decode('utf-8')
        if not data:
            break
        print('From online user: ' + data)
        data = data.upper()
        s.send(data.encode('utf-8'))
    s.close()


if __name__ == '__main__':
    # client()
    print(files_list("C:\\"))
# files = os.listdir('C:\\')
# files_2 = files[count]
# files_3 = files_2[count_3]
# print(files)
# print(files_2)
# print(files_3)
