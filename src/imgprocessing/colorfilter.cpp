#include <imgprocessing/colorfilter.hpp>
// #include <opencv2/opencv.hpp>



my::ColorFilter::ColorFilter(
                const ::cv::Vec3b&          f_blueInferior
                ,const ::cv::Vec3b&         f_blueSuperior
                ,const ::cv::Vec3b&         f_red1Inferior
                ,const ::cv::Vec3b&         f_red1Superior
                ,const ::cv::Vec3b&         f_red2Inferior
                ,const ::cv::Vec3b&         f_red2Superior
                ,const uint&                f_dilationSize
                ,const bool&                f_dilationApply
                ,const uint&                f_erosionSize
                ,const bool&                f_erorionApply)
    :m_blueInferior(f_blueInferior)
    ,m_blueSuperior(f_blueSuperior)
    ,m_red1Inferior(f_red1Inferior)
    ,m_red1Superior(f_red1Superior)
    ,m_red2Inferior(f_red2Inferior)
    ,m_red2Superior(f_red2Superior)
    ,m_dilationKernel(cv::getStructuringElement(cv::MORPH_CROSS,cv::Size(2 * f_dilationSize + 1, 2 * f_dilationSize + 1),cv::Point(f_dilationSize, f_dilationSize)))
    ,m_dilationApply(f_dilationApply)
    ,m_erosionKernel(cv::getStructuringElement(cv::MORPH_CROSS,cv::Size(2 * f_erosionSize + 1, 2 * f_erosionSize + 1),cv::Point(f_erosionSize, f_erosionSize)))
    ,m_erosionApply(f_erorionApply)
{
}


my::ColorFilter::ColorFilter(const my::Settings& f_settings)
    :m_blueInferior(f_settings.getblueInferior())
    ,m_blueSuperior(f_settings.getblueSuperior())
    ,m_red1Inferior(f_settings.getred1Inferior())
    ,m_red1Superior(f_settings.getred1Superior())
    ,m_red2Inferior(f_settings.getred2Inferior())
    ,m_red2Superior(f_settings.getred2Superior())
    ,m_dilationKernel(cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(2 * f_settings.getdilationSize() + 1, 2 * f_settings.getdilationSize() + 1),cv::Point(f_settings.getdilationSize(), f_settings.getdilationSize())))
    ,m_dilationApply(f_settings.getdilationApply())
    ,m_erosionKernel(cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(2 * f_settings.geterosionSize() + 1, 2 * f_settings.geterosionSize() + 1),cv::Point(f_settings.geterosionSize(), f_settings.geterosionSize())))
    ,m_erosionApply(f_settings.geterosionApply())
{
}



my::ColorFilter::ColorFilter_Data my::ColorFilter::apply(const ::cv::Mat& f_img){
    cv::Mat l_imgHSV;
    cv::cvtColor(f_img,l_imgHSV,cv::COLOR_BGR2HSV);
    
    cv::Mat l_imgBlueMask;
    cv::inRange(l_imgHSV, m_blueInferior, m_blueSuperior, l_imgBlueMask);
    
    cv::Mat l_imgRed1Mask;
    cv::inRange(l_imgHSV, m_red1Inferior, m_red1Superior, l_imgRed1Mask);

    cv::Mat l_imgRed2Mask;
    cv::inRange(l_imgHSV, m_red2Inferior, m_red2Superior, l_imgRed2Mask);
    
    cv::Mat l_SummedMask;
    cv::Mat l_SummedRedMask;
    cv::bitwise_or(l_imgRed1Mask,l_imgRed2Mask,l_SummedRedMask);
    

    if( m_erosionApply && m_dilationApply && m_erosionKernel.size()==m_dilationKernel.size()){
        // cv::morphologyEx(l_SummedMask,l_SummedMask,cv::MORPH_OPEN,m_dilationKernel);    
        cv::morphologyEx(l_SummedRedMask,l_SummedRedMask,cv::MORPH_OPEN,m_dilationKernel);
        cv::morphologyEx(l_imgBlueMask,l_imgBlueMask,cv::MORPH_OPEN,m_dilationKernel);        
    }else{
        if(m_erosionApply){
            // cv::erode(l_SummedMask,l_SummedMask,m_erosionKernel);
            cv::erode(l_SummedRedMask,l_SummedRedMask,m_erosionKernel);
            cv::erode(l_imgBlueMask,l_imgBlueMask,m_erosionKernel);
        }

        if(m_dilationApply){
            // cv::dilate(l_SummedMask,l_SummedMask,m_dilationKernel);
            cv::dilate(l_SummedRedMask,l_SummedRedMask,m_dilationKernel);
            cv::dilate(l_imgBlueMask,l_imgBlueMask,m_dilationKernel);
        }
    }

    cv::bitwise_or(l_imgBlueMask,l_SummedRedMask,l_SummedMask);
    

    my::ColorFilter::ColorFilter_Data l_resData;
    l_resData.blueMask = l_imgBlueMask;
    l_resData.redMask = l_SummedRedMask;
    l_resData.Mask = l_SummedMask;

    return l_resData;
} 

