import time
from socket import *

serverPort = 12000
serverSocket = socket(AF_INET, SOCK_STREAM)
serverSocket.bind(('', serverPort))
serverSocket.listen(1)
print("The server is ready to receive")

while True:
    connectionSocket, addr = serverSocket.accept()
    print("Connected by:", addr)

    while True:
        sentence = connectionSocket.recv(1024)
        if not sentence:
            break
        text = sentence.decode('utf-8')
        print("Client:", text)
        
        response = input("You: ")
        connectionSocket.send(response.encode('utf-8'))
    
    connectionSocket.close()
