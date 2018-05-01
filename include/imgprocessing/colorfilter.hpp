#ifndef COLORFILTER_HEADER
#define COLORFILTER_HEADER

#include <opencv2/opencv.hpp>
#include <settings/settings.hpp>

#include <ctime>

namespace my{
    class ColorFilter{
        public:
            typedef struct{
                ::cv::Mat blueMask;
                ::cv::Mat redMask;
                ::cv::Mat Mask;

                cv::Mat getBlueMask(){return blueMask;}
                cv::Mat getRedMask(){return redMask;}
                cv::Mat getMask(){return Mask;}

            }ColorFilter_Data;


            ColorFilter(  const ::cv::Vec3b&,
                            const ::cv::Vec3b&,
                            const ::cv::Vec3b&,
                            const ::cv::Vec3b&,
                            const ::cv::Vec3b&,
                            const ::cv::Vec3b&,
                            const uint&,
                            const bool&,
                            const uint&,
                            const bool&);
            
            ColorFilter(const my::Settings&);
            ColorFilter_Data apply(const ::cv::Mat&);
        private:

            const ::cv::Vec3b       m_blueInferior;
            const ::cv::Vec3b       m_blueSuperior;
            const ::cv::Vec3b       m_red1Inferior;
            const ::cv::Vec3b       m_red1Superior;
            const ::cv::Vec3b       m_red2Inferior;
            const ::cv::Vec3b       m_red2Superior;

            const ::cv::Mat         m_dilationKernel;
            bool                    m_dilationApply;

            const ::cv::Mat         m_erosionKernel;
            bool                    m_erosionApply;
        
    };
    
};


#endif