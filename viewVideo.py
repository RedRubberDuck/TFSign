#!/usr/bin/env python3
import cv2
import time
import os
import numpy as np
import time


import videoProc


def main():
    # source folder
    inputFolder = os.path.realpath(
        "/home/nandi/Workspaces/git/resource/TFSign/Video/newVideo/")
    # 'C:\\Users\\aki5clj\\Documents\\Git\\WorkspaceC_C++\\resource\\Video\\'
    # source file

    inputFileName = '/picam18-06-20-19-16.h264'
    outputSubFolder = '/pics/'

    print('Processing:', inputFolder+inputFileName)
    # Video frame reader object
    videoReader = videoProc.VideoReaderWithResize(inputFolder+inputFileName, 1)
    frameRate = 30.0
    frameDuration = int(1.0/frameRate*1000)

    index = 0
    for frame in videoReader.readFrame():

        cv2.imshow('', frame)
        cv2.imwrite(inputFolder+outputSubFolder +
                    "pics"+str(index)+".jpg", frame)
        if cv2.waitKey() & 0xFF == ord('q'):
            break

        index += 1

    end = time.time()


if __name__ == '__main__':
    main()
