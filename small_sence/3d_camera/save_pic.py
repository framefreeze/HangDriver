import cv2
import numpy as np
cap = cv2.VideoCapture(0)
count = 0;
while cap.isOpened() :
    rst,frame = cap.read()
    imshow('pic_now',frame)
    key = waitKey()
    if key == 27 :
        break
    elif ke
