import socket

HEADERSIZE = 10

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((socket.gethostname(), 1234))

while True:
    fullMessage = ''
    newMessage = True
    while True:        
        msg = s.recv(16)
        if newMessage:
            print(f'new message length: {msg[:HEADERSIZE]}')
            msglen = int(msg[:HEADERSIZE])
            newMessage = False
        fullMessage += msg.decode("utf-8")
        if len(fullMessage)-HEADERSIZE == msglen:
            print("full msg recived")
            print(fullMessage[HEADERSIZE:])
            newMessage = True
            fullMessage = ''
            
    print(msg.decode("utf-8"))