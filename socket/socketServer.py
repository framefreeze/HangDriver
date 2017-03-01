import socket
import cv2
import numpy as np
# HOST = ''
PORT = 10999
s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.bind(('',PORT))
s.listen(1)
cap = cv2.VideoCapture(0)
while True:

    conn, addr = s.accept()
    print('Connected by',addr)
    while True:

        dataRecv=conn.recv(4096);
        #dataRecv.decode()
        #print(dataRecv.decode())
        if dataRecv.decode() == 'get':
            #print(1)
            ret, frame = cap.read()
            #print(frame.shape[0])

            frame = cv2.resize(frame,(2560,720))
            # cv2.imshow('naem', frame)
            # cv2.waitKey()
            # print(frame.shape)
            data = frame.tostring()
            print(len(data))
            # print(data)
            #print(data)
            conn.sendall(data)
            print('ser_done')
            #conn.sendall(data)
            conn.send(bytes('end','UTF-8'))
            # conn.sendall('end')
conn.close()
