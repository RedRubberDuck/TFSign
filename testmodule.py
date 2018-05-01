from build import mytrafficsigndetect
import cv2
import numpy as np


l_settings = mytrafficsigndetect.InitSettings("/home/nandi/Workspaces/git/TFSign/setttings.json")

l_img = cv2.imread('/home/nandi/Workspaces/git/resource/TFSign/Video/InitialVideo/ParkingVid1/picsPark64.jpg')

l_colorFilter = mytrafficsigndetect.ColorFilter(l_settings)
l_ImageSegmenting = mytrafficsigndetect.ImageSegmenter(l_settings)
l_hogCalc = mytrafficsigndetect.HogCalculation(l_settings)
l_trafficSignDetection = mytrafficsigndetect.TrafficSignProcessing(l_colorFilter,l_ImageSegmenting,l_hogCalc,'svm4.xml')
l_res1 = l_colorFilter.apply(l_img)
l_segments = mytrafficsigndetect.SegmentVector()
# l_segmentList = [l_segment]

l_ImageSegmenting.apply(l_res1.getblueMask(),l_res1.getredMask(),l_segments)

print('Len:',len(l_segments))

# for i in range(len(l_segments)):
#     l_segment = l_segments[i]
#     p1 = (l_segment.left,l_segment.top)
#     p2 = (l_segment.left + l_segment.width,l_segment.top + l_segment.height)
#     l_segmentImg = l_img[p1[1]:p2[1],p1[0]:p2[0]]
#     cv2.rectangle(l_img,p1,p2,(255,0,0))
#     l_featues = mytrafficsigndetect.HogFeatures()
#     l_hogCalc.apply(l_segmentImg,l_featues)


#     cv2.imshow('Segment',l_segmentImg)
#     cv2.waitKey()
    
l_trafficSignDetection.processFrameAndDraw(l_img)

cv2.imshow("init",l_img)
cv2.waitKey()


l_trafficSignsVec = mytrafficsigndetect.TrafficSignsVector() 
l_trafficSignDetection.processFrame(l_img,l_trafficSignsVec)

for i in range(len(l_trafficSignsVec)):
    print("Segment",l_trafficSignsVec[i].signType)