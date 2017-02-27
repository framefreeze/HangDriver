import cv2
import numpy as np
cap = cv2.VideoCapture(0)
count = 0;
while cap.isOpened() :
    rst,frame = cap.read()
    cv2.imshow('pic_now',frame)
    key = cv2.waitKey()
    print(key)
    if key == 27 : # ESC
        break
    elif chr(key) == 's' :
        filename = 'data/pic'+'{}'.format(count)+'.jpg'
        count += 1
        # print(filename)
        cv2.imwrite(filename, frame)

