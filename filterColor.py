from buildPy import mytrafficsigndetect as TSD
import cv2


def main():
    print 'Start filter color'

    picsfolderName = '/home/nandi/Workspaces/git/resource/TFSign/Video/newVideo/picsSelected/'
    inputPicsName = 'pics496'
    inputPicsExtension = '.jpg'

    l_img = cv2.imread(picsfolderName+inputPicsName+inputPicsExtension)
    newSize = (int(l_img.shape[1]/2), int(l_img.shape[0]/2))
    l_img = cv2.resize(l_img, newSize)

    l_settings = TSD.InitSettings("settings_v3.json")
    l_colorFilter = TSD.ColorFilter(l_settings)
    masksObj = l_colorFilter.apply(l_img)

    l_redMask = masksObj.getredMask()
    l_blueMask = masksObj.getblueMask()
    print l_redMask.shape

    # cv2.imwrite(picsfolderName+inputPicsName+'blueMask'+inputPicsExtension, l_blueMask)
    # cv2.imwrite(picsfolderName+inputPicsName+'redmask'+inputPicsExtension, l_redMask)
    # cv2.imshow('RedMask', l_redMask)
    # cv2.imshow('BlueMask', l_blueMask)

    # cv2.waitKey()

    print 'End filter color'


if (__name__ == '__main__'):
    main()
