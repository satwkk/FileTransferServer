#!/usr/bin/python3
import cmd
import socket

class Client(cmd.Cmd):
    intro = "Welcome to file transfer client"
    prompt = "> "
    client_socket = None
    host = None
    port = None

    def __init__(self, host, port, completekey = "tab", stdin = None, stdout = None):
        super().__init__(completekey, stdin, stdout)
        self.host = host
        self.port = port
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.client_socket.connect((host, port))
        print(self.client_socket.recv(1024).decode())

    def do_exit(self, arg):
        self.client_socket.close()
        exit(0)

    def do_list(self, arg):
        print(self.send_command("ls"))

    def do_get(self, arg):
        contents = self.send_command(f"download {arg}")
        print(contents)

        with open(arg, 'w') as file:
            file.write(contents)

    def send_command(self, command):
        self.client_socket.sendall(command.encode())
        return self.client_socket.recv(1024).decode()

def main():
    Client('127.0.0.1', 9001).cmdloop()

if __name__ == '__main__':
    main()
    