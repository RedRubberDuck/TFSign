import cv2

cap = cv2.VideoCapture(
    '/home/nandi/Workspaces/git/new_resource/videoPhone/picam18-06-26-21-24.h264')


outputFolder = '/home/nandi/Workspaces/git/new_resource/videoPhone/picsIn/'
imgIndex = 0
while (cap.isOpened()):
    ret, frame = cap.read()
    cv2.imwrite(outputFolder+'pics'+str(imgIndex)+'.png', frame)
    imgIndex += 1

cap.release()
cv2.destroyAllWindows()
