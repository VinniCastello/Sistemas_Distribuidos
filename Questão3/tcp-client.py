import threading
from socket import *

serverName = 'localhost'
serverPort = 12000
clientSocket = socket(AF_INET, SOCK_STREAM)
clientSocket.connect((serverName, serverPort))

def receive_messages():
    while True:
        response = clientSocket.recv(1024)
        if not response:
            break
        text = response.decode('utf-8')
        print("Server:", text)

receive_thread = threading.Thread(target=receive_messages)
receive_thread.start()

while True:
    sentence = input("You: ")
    clientSocket.send(sentence.encode('utf-8'))

    if sentence.lower() == "exit":
        break

clientSocket.close()
