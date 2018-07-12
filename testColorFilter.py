from buildPy import mytrafficsigndetect
import cv2
import numpy as np


l_settings = mytrafficsigndetect.InitSettings(
    "/home/nandi/Workspaces/git/TFSign_new/settings_v3.json")
l_img = cv2.imread('/home/nandi/Workspaces/git/resource/TrafficSignPics/total1.png')
print(l_img.shape)


rangeRed1 = [0, 30]
rangeSat = [0, 255]
rangeV = [0, 255]


l_colorFilter = mytrafficsigndetect.ColorFilter(l_settings)
#
l_res = l_colorFilter.apply(l_img)
#
cv2.imwrite('Blue.png', l_res.getblueMask())
cv2.imwrite('Red.png', l_res.getredMask())
cv2.waitKey()
