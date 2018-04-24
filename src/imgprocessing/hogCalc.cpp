#include <imgprocessing/hogCalc.hpp>

my::HogCalculation::HogCalculation()
    :m_hog(cv::Size(64,64),cv::Size(8,8),cv::Size(4,4),cv::Size(4,4), 9, 1)
    ,m_imgSize(cv::Size(64,64))
{

}



my::HogCalculation::HogCalculation(const my::Settings&      f_settings)
    :m_hog(f_settings.getHogImageSize(),f_settings.getHogBlockSize(),f_settings.getHogCellSize(),f_settings.getHogCellSize(),f_settings.getHogNrBins(), 1)
    ,m_imgSize(f_settings.getHogImageSize())
{
}



void my::HogCalculation::apply(const cv::Mat&           f_img
                                ,std::vector<float>&    f_descriptors){
    cv::Mat l_gray;
    cv::cvtColor(f_img, l_gray, CV_BGR2GRAY);
    cv::Mat l_resized; 
    cv::resize(l_gray,l_resized,m_imgSize);
    m_hog.compute(l_resized, f_descriptors);

}



