import socket
#import cv2
#import numpy as np
HOST = '192.168.1.100'
PORT = 10999
s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.bind((HOST,PORT))
s.settimeout(50)
s.listen(10)
#cap = cv2.VideoCapture(0)
while True:
    conn, addr = s.accept()
    print('Connected by',addr)
    while True:
        dataRecv=conn.recv(4096);
        print(dataRecv)
        conn.sendall(bytes('end','UTF-8'))
            # conn.sendall('end')
conn.close()
