#ifndef PROCESSING_HPP
#define PROCESSING_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>


#include <imgprocessing/colorfilter.hpp>
#include <imgprocessing/segmentation.hpp>
#include <imgprocessing/hogCalc.hpp>
#include <settings/settings.hpp>
#include <file/fileHandler.hpp>
#include <ml/utilities.hpp>

namespace my{
    class TrafficSignProcessing{
        public:
            TrafficSignProcessing(  const my::ColorFilter&
                                    ,const my::ImageSegment&
                                    ,const my::HogCalculation&
                                    ,const std::string&);


            void processFrameAndDraw(cv::Mat&);
        private:
            my::ColorFilter         m_colorFilter;
            my::ImageSegment        m_imgSegmenting;
            my::HogCalculation      m_hogCalc;
            cv::Ptr<cv::ml::SVM>    m_svm;

    };
};


#endif