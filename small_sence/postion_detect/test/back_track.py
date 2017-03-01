import numpy as np
import cv2
cap = cv2.VideoCapture('test.m4v')
fgbg = cv2.createBackgroundSubtractorMOG2()
fourcc = cv2.VideoWriter_fourcc('H','2','6','4')
cap_out = cv2.VideoWriter('out.m4v',fourcc , 30.0, (1024, 768))
while(1):
    ret, frame = cap.read()
    fgmask = fgbg.apply(frame)
    cv2.imshow('frame',fgmask)
    # if fgmask.shape[2] != 1 :
    #     fgmask = cv2.cvtColor(fgmask, cv2.COLOR_BGR2GRAY)
    ret,thresh = cv2.threshold(fgmask,127,255,0)
    im2, contours, hierarchy = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
    for cnt in contours :
        if cv2.contourArea(cnt) > 5000 :
            # cv2.drawContours(frame, [cnt], -1, (0,255,0), 3)
            (x,y),radius = cv2.minEnclosingCircle(cnt)
            center = (int(x),int(y))
            radius = int(radius)
            cv2.circle(frame,center,radius,(0,255,0),2)
    cv2.imshow('front', frame)
    cap_out.write(frame)
    k = cv2.waitKey(30) & 0xff
    if k == 27:
        break

cap.release()
cv2.destroyAllWindows()