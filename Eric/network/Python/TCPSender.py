import socket
import time


TCP_IP = '149.171.37.126'
TCP_PORT = 8888
UE_IP = '127.0.0.1'
UE_PORT = 64

BUFFER_SIZE = 640 * 360  # Normally 1024, but we want fast response
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((TCP_IP, TCP_PORT))
print('Waiting...')
s.listen(8)
conn, addr = s.accept()
print('Connection address:', addr)

while 1:
    data = conn.recv(BUFFER_SIZE)
    if not data: break
    # print("Received data:", data)
    print("Size received:", len(data))
    s_ue = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s_ue.connect((UE_IP, UE_PORT))
    s_ue.send(data)
    s_ue.close()
    # print("Sent to UE4!")
    # time.sleep(0.5)

conn.close()