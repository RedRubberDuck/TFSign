#ifndef IMAGE_SEGMENTATION_HEADER
#define IMAGE_SEGMENTATION_HEADER

#include <settings/settings.hpp>
#include <imgprocessing/utility.hpp>

#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>




namespace my{
    class ImageSegment{

        public:
            typedef enum{
                BLUE=0,RED,None
            }ColorTypes_t;

            class Segment_t{
                public:
                ColorTypes_t color;
                uint left,top;
                uint width,height;


                bool operator==(Segment_t const& x) const { return (color == x.color
                                                                     && left == x.left 
                                                                     && top == x.top 
                                                                     && width == x.width
                                                                     && height == x.height); }
                bool operator!=(Segment_t const& x) const { return (color != x.color
                                                                     || left != x.left 
                                                                     || top != x.top 
                                                                     || width != x.width
                                                                     || height != x.height); }
            };

            typedef std::vector<Segment_t>& SegmentVector_t;

            ImageSegment(const my::Settings&);
            ImageSegment(const ImgSegGradType_t&,const uint&);
            
            void apply(                 const cv::Mat&
                                        ,const cv::Mat&
                                        ,SegmentVector_t);
            
            void applyColor(             const cv::Mat&
                                        ,const ColorTypes_t&
                                        ,std::vector<Segment_t>&);
            void segmentProc(       const uint&
                                    ,const cv::Mat&
                                    ,const cv::Mat&
                                    ,const cv::Mat&
                                    ,const ColorTypes_t&
                                    ,std::vector<Segment_t>&);
            static void showLabels( const uint&
                                    ,const cv::Mat&
                                    ,const cv::Mat&
                                    );

            void segmenting       ( const cv::Mat&
                                    ,const std::vector<Segment_t>&);
        private:

            void verify(    std::vector<Segment_t>&);

            ImgSegGradType_t            m_gradiantType;
            uint                        m_kernelSize;
            cv::Mat                     m_kernel;
            const double                m_InferiorRate;
            const double                m_SuperiorRate;


    };



};

#endif