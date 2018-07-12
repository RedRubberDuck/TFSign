from buildPy import mytrafficsigndetect as TSD
import cv2
import numpy as np
import glob


def main():
    print 'Start segmented test'

    picsfolderName = '/home/nandi/Workspaces/pythonWorkspace/generateSign/unicGen1/'
    outputFolder = '/home/nandi/Workspaces/pythonWorkspace/generateSign/OutUnicGen1/'
    inputPicsName = 'unic15'
    inputPicsExtension = '.png'

    l_settings = TSD.InitSettings("settings_v3.json")
    l_colorFilter = TSD.ColorFilter(l_settings)
    l_img = cv2.imread(picsfolderName+inputPicsName+inputPicsExtension)

    l_segmenter = TSD.ImageSegmenter(l_settings)
    l_segmentVector = TSD.SegmentVector()

    files = glob.glob(picsfolderName+'*'+inputPicsExtension)
    for fileFullPath in files:
        l1 = fileFullPath.rfind('/')+1
        l2 = fileFullPath.rfind('.')
        fileName = fileFullPath[l1:l2]
        inputPicsName = fileName

        l_img = cv2.imread(picsfolderName+inputPicsName+inputPicsExtension)
        masksObj = l_colorFilter.apply(l_img)
        l_redMask = masksObj.getredMask()
        l_blueMask = masksObj.getblueMask()
        l_segmenter.apply(l_blueMask, l_redMask, l_segmentVector)

        num_labels, labels, stats, centroids = cv2.connectedComponentsWithStats(
            l_redMask, 8, cv2.CV_32S)

        nrSegment = 0
        nrStats = len(stats)
        for statIndex in range(1, nrStats):
            stat = stats[statIndex]
            left = stat[cv2.CC_STAT_LEFT]
            top = stat[cv2.CC_STAT_TOP]
            w = stat[cv2.CC_STAT_WIDTH]
            h = stat[cv2.CC_STAT_HEIGHT]
            segmentBlue = l_img[top:top+h,
                                left:left+w]
            cv2.imwrite(outputFolder+inputPicsName+'segment' +
                        str(nrSegment)+inputPicsExtension, segmentBlue)
            nrSegment += 1

    # cv2.imwrite(picsfolderName+inputPicsName+'segment'+inputPicsExtension, l_img)
    cv2.destroyAllWindows()
    print 'End segmented test'


if (__name__ == '__main__'):
    main()
