import time
from socket import *
serverPort = 12000
serverSocket = socket(AF_INET,SOCK_STREAM)
serverSocket.bind(('',serverPort))
serverSocket.listen(1)
print("The server is ready to receive")
while 1:
     connectionSocket, addr = serverSocket.accept()
     sentence = connectionSocket.recv(1024)
     text = sentence.decode('utf-8')
     capitalizedSentence = text.upper()
     connectionSocket.send(capitalizedSentence.encode('utf-8'))
     connectionSocket.close()
