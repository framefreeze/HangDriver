import cv2
import numpy as np
cap = cv2.VideoCapture(2)
count = 0;
if cap.isOpened() :
    rst,frame = cap.read()
    x,y,dim = frame.shape
    fourcc = cv2.VideoWriter_fourcc(*'VIDE')
is_recording = False
while cap.isOpened() :
    rst,frame = cap.read()
    cv2.imshow('pic_now',frame)
    key = cv2.waitKey(10)
    # print('s')
    if is_recording :
        out.write(frame)
        print('ok')
    if key == 27 : # ESC
        break
    elif key == 115 :
        if not is_recording :
            filename = 'data/video'+'{}'.format(count)+'.mp4'
            out = cv2.VideoWriter(filename,fourcc, 28.0, (x,y))
            count += 1
            print('Record start: press s to end')
            is_recording = True
        else :
            is_recording = False
            out.release()
            print('Done')

cap.release()
out.release()
