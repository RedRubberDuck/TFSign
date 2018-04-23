#ifndef ML_UTILITIES_HPP
#define ML_UTILITIES_HPP


#include<array>
#include<string>

#include <imgprocessing/colorfilter.hpp>
#include <imgprocessing/segmentation.hpp>
#include <imgprocessing/hogCalc.hpp>
#include <file/fileHandler.hpp>
#include <settings/settings.hpp>

#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>

#include <string>
#include <array>
#include <iostream>
#include <sstream>
#include <dirent.h>

namespace my{
    template<uint N>
    void readAndSaveFeatures(       std::array<std::string,N>&
                                    ,std::array<std::string,N>&
                                    ,my::ColorFilter&
                                    ,my::ImageSegment&
                                    ,my::HogCalculation&);
    
    template<uint N>
    void readAndTrainSVM(           const std::array<std::string,N>&
                                    ,const std::string&);
    
    template<uint N>
    void testSVM(                   const std::array<std::string,N>&
                                    ,const std::string&);

    
    void testSVMBigFrame(           const std::string&
                                    ,const std::string&);

    void readFromFolderImages(const std::string &,std::vector<cv::Mat>&);

    
};

#include "utilities.inl"




#endif