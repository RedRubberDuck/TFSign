#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <string>
#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include <iostream>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <opencv2/opencv.hpp>
#include <imgprocessing/utility.hpp>


namespace my{
    class Settings{
        public:
        Settings();
        Settings(const ::cv::Vec3b&,
                const ::cv::Vec3b&,
                const ::cv::Vec3b&,
                const ::cv::Vec3b&,
                const ::cv::Vec3b&,
                const ::cv::Vec3b&,
                const uint&,
                const bool&,
                const uint&,
                const bool&,
                const my::ImgSegGradType_t&,
                const uint&,
                const double&,
                const double&);

        static Settings readFile(const std::string&);
        static void setLimit(cv::Vec3b&, const rapidjson::Value&);


        ::cv::Vec3b getblueInferior() const;
        ::cv::Vec3b getblueSuperior() const;
        ::cv::Vec3b getred1Inferior() const;
        ::cv::Vec3b getred1Superior() const;
        ::cv::Vec3b getred2Inferior() const;
        ::cv::Vec3b getred2Superior() const;

        uint getdilationSize() const;
        bool getdilationApply() const;

        uint geterosionSize() const;
        bool geterosionApply() const;

        my::ImgSegGradType_t getImgSegmentGradType() const;
        uint getImgSegmentKernelSize() const;

        double getInferiorSquareRate() const;
        double getSuperiorSquareRate() const;
        
        private:
            //----------------------------------------------------
            //              Limits parameters
            ::cv::Vec3b                 m_blueInferior;
            ::cv::Vec3b                 m_blueSuperior;
            ::cv::Vec3b                 m_red1Inferior;
            ::cv::Vec3b                 m_red1Superior;
            ::cv::Vec3b                 m_red2Inferior;
            ::cv::Vec3b                 m_red2Superior;
            //----------------------------------------------------
            //      Dilation and erision parameters
            uint                        m_dilationSize;
            bool                        m_dilationApply;
            uint                        m_erosionSize;
            bool                        m_erosionsApply;
            //----------------------------------------------------
            //      Image segmentation parameters
            my::ImgSegGradType_t        m_ImgSegment_gradType;
            uint                        m_ImgSegment_kernelSize;
            double                      m_InferiorSquareRate;
            double                      m_SuperiorSquareRate;      
    };

};


#endif