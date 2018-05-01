from build import mytrafficsigndetect
import cv2
import numpy as np



l_settings = mytrafficsigndetect.InitSettings("/home/nandi/Workspaces/git/TFSign/setttings.json")
l_img = cv2.imread('/home/nandi/Workspaces/git/resource/TrafficSignPics/red.jpg')

l_colorFilter = mytrafficsigndetect.ColorFilter(l_settings)

l_res = l_colorFilter.apply(l_img)

cv2.imshow('Blue',l_res.getblueMask())
cv2.imshow('Red',l_res.getredMask())
cv2.waitKey()