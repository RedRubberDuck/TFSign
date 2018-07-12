from buildPy import mytrafficsigndetect as TSD
import cv2
import numpy as np


def main():
    print 'Start segmented test'

    picsfolderName = '/home/nandi/Workspaces/git/resource/TFSign/Video/newVideo/picsSelected/'
    inputPicsName = 'pics533'
    inputPicsExtension = '.jpg'

    l_blue_mask = cv2.imread(picsfolderName+inputPicsName+'blueMask'+inputPicsExtension)
    l_blue_mask = np.array(l_blue_mask[:, :, 0]/255, dtype='uint8')
    print l_blue_mask.shape

    l_red_mask = cv2.imread(picsfolderName+inputPicsName+'redmask'+inputPicsExtension)
    l_red_mask = np.array(l_red_mask[:, :, 0]/255, dtype='uint8')

    l_settings = TSD.InitSettings("settings_v3.json")
    l_segmenter = TSD.ImageSegmenter(l_settings)

    l_segmentVector = TSD.SegmentVector()
    l_segmenter.apply(l_blue_mask, l_red_mask, l_segmentVector)

    l_img = cv2.imread(picsfolderName+inputPicsName+inputPicsExtension)
    newSize = (int(l_img.shape[1]/2), int(l_img.shape[0]/2))
    l_img = cv2.resize(l_img, newSize)

    for l_segment in l_segmentVector:
        print l_segment.top,  l_segment.left, l_segment.width, l_segment.height, l_segment.color
        if l_segment.color == TSD.MaskColorTypes.BLUE:
            cv2.rectangle(l_img, (l_segment.left, l_segment.top),
                          (l_segment.left+l_segment.width, l_segment.top+l_segment.height), (255, 0, 0), 2)
        elif l_segment.color == TSD.MaskColorTypes.RED:
            cv2.rectangle(l_img, (l_segment.left, l_segment.top),
                          (l_segment.left+l_segment.width, l_segment.top+l_segment.height), (0, 0, 255), 2)
        elif l_segment.color == TSD.MaskColorTypes.NONE:
            cv2.rectangle(l_img, (l_segment.left, l_segment.top),
                          (l_segment.left+l_segment.width, l_segment.top+l_segment.height), (255, 255, 255), 2)
            # cv2.imshow('RedMask', l_img)
            # cv2.waitKey()

    cv2.imwrite(picsfolderName+inputPicsName+'segment'+inputPicsExtension, l_img)
    cv2.destroyAllWindows()
    print 'End segmented test'


if (__name__ == '__main__'):
    main()
