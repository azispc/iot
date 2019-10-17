import sys
import socket
import selectors
import types
import requests

filename = "hasilsensor.txt"
api_key="KU38QP8KIYDG6M1V"

#fungsi post
def post(a,b):
    datarequest={'api_key': api_key, 'field1': b}
    endpoint="https://api.thingspeak.com/update"
    r=requests.post(url=endpoint,data=datarequest)
    print("Data berhasil dikirim ke ThingSpeak !")

sel = selectors.DefaultSelector()

def accept_wrapper(sock):
    conn, addr=sock.accept()
    print("berhasil koneksi dari", addr)
    conn.setblocking(False)
    data=types.SimpleNamespace(addr=addr, inb=b"", outb=b"")
    events=selectors.EVENT_READ| selectors.EVENT_WRITE
    sel.register(conn, events, data=data)

def service_connection(key, mask):
    sock=key.fileobj
    data=key.data
    if mask & selectors.EVENT_READ:
        recv_data=sock.recv(1024)
        if recv_data:
            data.outb+=recv_data
        else:
            print("Dissconnect from to address:", data.addr)
            sel.unregister(sock)
            sock.close()
        if mask & selectors.EVENT_WRITE:
            if data.outb:
                print("hasil", repr(data.outb), "to", data.addr)
                sent= sock.send(data.outb)
                data.outb=data.outb[sent:]
                #Panggil fungsi masukkan data temperatur kedalam .txt
                append_data_to_file(recv_data.decode("utf-8"));
                print("data berhasil di simpan ke file .txt !")
                post(api_key, recv_data);


#fungsi_buat read data string untuk dimasukkan kedalam file .txt
def append_data_to_file(data):
    f=open(filename, "a+")
    f.write(data)
    f.write("\n")
    f.close()

if len(sys.argv) !=3:
    print("usage:", sys.argv[0], "<host> <port>")
    sys.exit(1)

host,port=sys.argv[1], int(sys.argv[2])
lsock=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
lsock.bind((host,port))
lsock.listen()
print("listening on", (host, port))
lsock.setblocking(False)
sel.register(lsock, selectors.EVENT_READ, data=None)

try:
    while True:
        events =sel.select(timeout=None)
        for key, mask in events:
            if key.data is None:
                accept_wrapper(key.fileobj)
            else:
                service_connection(key, mask)

except KeyboardInterrupt:
    print("caught keyboard interrupt exiting")
finally:
    sel.close()
