import threading
from socket import *

serverPort = 12000
serverSocket = socket(AF_INET, SOCK_STREAM)
serverSocket.bind(('', serverPort))
serverSocket.listen(1)
print("The server is ready to receive")

def client_handler(connectionSocket):
    while True:
        sentence = connectionSocket.recv(1024)
        if not sentence:
            break
        text = sentence.decode('utf-8')
        print("Client:", text)

def main():
    while True:
        connectionSocket, addr = serverSocket.accept()
        print("Connected by:", addr)

        client_thread = threading.Thread(target=client_handler, args=(connectionSocket,))
        client_thread.start()

if __name__ == "__main__":
    main()
