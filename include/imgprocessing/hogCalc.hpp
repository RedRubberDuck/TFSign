#ifndef HOG_CALCULATION_HPP
#define HOG_CALCULATION_HPP


#include <opencv2/opencv.hpp>
#include <settings/settings.hpp>
#include <iostream>

namespace my{
    class HogCalculation{
    
        public:
            HogCalculation();
            HogCalculation(const my::Settings& );

            void apply(const cv::Mat &, std::vector<float> &);

          private:
            cv::HOGDescriptor m_hog;
    };

};



#endif