from socket import *

serverName = 'localhost'
serverPort = 12000
clientSocket = socket(AF_INET, SOCK_STREAM)
clientSocket.connect((serverName, serverPort))

while True:
    sentence = input("You: ")
    clientSocket.send(sentence.encode('utf-8'))
    
    response = clientSocket.recv(1024)

    if sentence.lower() == "exit":
        break

    if not response:
        break
    text = response.decode('utf-8')
    print("Server:", text)

clientSocket.close()
