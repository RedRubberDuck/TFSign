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

#include <iostream>
#include <sstream>
#include <dirent.h>

#include <ml/processing.hpp>

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
                                    ,const std::string&
                                    ,my::ColorFilter&
                                    ,my::ImageSegment&
                                    ,my::HogCalculation&);

    void readFromFolderImages(const std::string &,std::vector<cv::Mat>&);

    // void drawRenctangles(cv::Mat &, std::vector<my::ImageSegment::Segment_t>, std::vector<my::ImageSegment::Segment_t>, std::vector<my::ImageSegment::Segment_t>, std::vector<my::ImageSegment::Segment_t>);
    void drawRenctangles(cv::Mat &
                        ,const std::vector< my::TrafficSignProcessing::TrafficSignSegment>&);

    void testVideo( const std::string&
                    ,const std::string&
                    ,const std::string&);
};

#include "utilities.inl"




#endif
