import socket

client =socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(('192.168.43.190',8080))


def ts(str):
    client.send('26.00'.encode())
    data=''
    data=client.recv(1024).decode()
    print (data)

#client.send("IamAzis\n")
while 2:
    r=input('enter')
    ts(r)

#from_server=client.recv(1024)
client.close()
