import socket
import cmd
#import pygame
import numpy as np
import cv2
HOST = ''
PORT = 10999
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('127.0.0.1',PORT))
'''
pygame.init()
print('inited')
screen = pygame.display.set_mode((1280,360))
pygame.display.set_caption('Web Camera')
pygame.display.flip()
clock = pygame.time.Clock()
'''


while True:
    iserror = False
    #dataI = input("input cmd")

    # print(dataO)
    #try:
        # data, address = cli_socket.recvfrom(65536)
        # print('a')
    s.sendall(bytes('get', 'UTF-8'))
    print('send')
    # str = s.recv(100)
    # print(len(str))
    str = b''
    while True:
        tmpstr = s.recv(38400)
        #print('')
        if tmpstr == b'end':
            print('done')
            break
        if(len(tmpstr)!=38400):
            iserror = True
            #cv2.waitKey()
            break
         #   print(tmpstr)
            #continue
        #print(len(tmpstr))

        # print(tmpstr)


        str+=tmpstr
        # print(str)

    #dataO = s.recv(8192000000);
    #print(2)
    #except socket.timeout:
    #    continue
    #print(dataO)
    if(iserror):
        print('con')
        continue
    #print(len(str))
    frame = np.fromstring(str,np.uint8)
    frame = frame.reshape(720,2560,3)
    cv2.imshow('abc',frame)
    cv2.waitKey(5)
    '''
    camshot = pygame.image.frombuffer(dataO,(1280,360),'RGB')
    camshot = pygame.transform.scale(camshot, (1280, 360))
    screen.blit(camshot, (0, 0))
    pygame.display.update()
    clock.tick(20)

    '''




s.close()
