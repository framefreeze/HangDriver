import cv2
import numpy as np
cap = cv2.VideoCapture(0)
count = 0;
while cap.isOpened() :
    rst,frame = cap.read()
    cv2.imshow('pic_now',frame)
    key = cv2.waitKey()
    # print('s')
    if key == 27 : # ESC
        break
    elif key == 115 :
        filename = 'data/pic'+'{}'.format(count)+'.jpg'
        count += 1
        # print(filename)
        cv2.imwrite(filename, frame)

