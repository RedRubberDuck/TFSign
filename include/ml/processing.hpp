#ifndef PROCESSING_HPP
#define PROCESSING_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>


#include <imgprocessing/colorfilter.hpp>
#include <imgprocessing/segmentation.hpp>
#include <imgprocessing/hogCalc.hpp>
#include <settings/settings.hpp>
#include <file/fileHandler.hpp>


namespace my{
    class TrafficSignProcessing{
        public:
            typedef enum{
                None=0,StopSign,ParkingSign,Limit30,NoLimit30,Limit50,NoLimit50,NoStop,NoParking,NoVechile
            }TrafficSignTypes_t;

            class TrafficSignSegment{
                public:
                    uint left,top,width,height;
                    TrafficSignTypes_t signType;

                bool operator==(TrafficSignSegment const& x) const {return (signType==x.signType
                                                                            && left==x.left
                                                                            && top==x.top
                                                                            && width==x.width
                                                                            && height==x.height);}

                bool operator!=(TrafficSignSegment const& x) const {return (signType!=x.signType
                                                                            || left!=x.left
                                                                            || top!=x.top
                                                                            || width!=x.width
                                                                            || height!=x.height);}
            };

            TrafficSignProcessing(  const my::ColorFilter&
                                    ,const my::ImageSegment&
                                    ,const my::HogCalculation&
                                    ,const std::string&);
            TrafficSignProcessing(const  my::Settings&);


            void processFrame(cv::Mat,std::vector<TrafficSignSegment>&);
            void processFrameAndDraw(cv::Mat,std::vector<TrafficSignSegment>&);

        private:
            my::ColorFilter         m_colorFilter;
            my::ImageSegment        m_imgSegmenting;
            my::HogCalculation      m_hogCalc;
            cv::Ptr<cv::ml::SVM>    m_svm;

    };
};

#include <ml/utilities.hpp>

#endif
