#include <imgprocessing/colorfilter.hpp>
#include <imgprocessing/segmentation.hpp>
#include <settings/settings.hpp>

#include <opencv2/opencv.hpp>

#include <string>
#include <iostream>

void printSettings( const my::Settings& f_settings){
    
    // Printed the blue range
    std::cout<<"Blue inferior:"<<(float)f_settings.getblueInferior()[0]<<","<<f_settings.getblueInferior()[1]/255.0<<","<<f_settings.getblueInferior()[2]/255.0<<std::endl;
    std::cout<<"Blue superior:"<<(float)f_settings.getblueSuperior()[0]<<","<<f_settings.getblueSuperior()[1]/255.0<<","<<f_settings.getblueSuperior()[2]/255.0<<std::endl;
    // Printed the fist red range
    std::cout<<"Red1 inferior:"<<(float)f_settings.getred1Inferior()[0]<<","<<f_settings.getred1Inferior()[1]/255.0<<","<<f_settings.getred1Inferior()[2]/255.0<<std::endl;
    std::cout<<"Red1 superior:"<<(float)f_settings.getred1Superior()[0]<<","<<f_settings.getred1Superior()[1]/255.0<<","<<f_settings.getred1Superior()[2]/255.0<<std::endl;
    // Printed the second red range
    std::cout<<"Red2 inferior:"<<(float)f_settings.getred2Inferior()[0]<<","<<f_settings.getred2Inferior()[1]/255.0<<","<<f_settings.getred2Inferior()[2]/255.0<<std::endl;
    std::cout<<"Red2 superior:"<<(float)f_settings.getred2Superior()[0]<<","<<f_settings.getred2Superior()[1]/255.0<<","<<f_settings.getred2Superior()[2]/255.0<<std::endl;
}


void drawSquare(    std::vector<my::ImageSegment::Segment_t>&   f_segments,
                    cv::Mat&                                    f_img)
{
    std::vector<my::ImageSegment::Segment_t>::iterator it;
    for (it=f_segments.begin() ; it!=f_segments.end();++it ){
        uint left,top,right,bottom;
        left = it->left;
        top  = it->top;
        right = it->left + it->width;
        bottom = it->top + it->height;

        cv::Point p1(left,top), p2(right,bottom);
        
        cv::Scalar col;
        switch(it->color){
            case my::ImageSegment::ColorTypes_t::BLUE:
                col = cv::Scalar(255,0,0);
                break;
            case my::ImageSegment::ColorTypes_t::RED:
                col = cv::Scalar(0,0,255);
                break;
            default:
                col = cv::Scalar(255,255,255);
        }

        cv::rectangle(f_img,p1,p2,col);
    }
}

int main(int argc, char** argv )
{
    std::string l_str = "/home/nandi/Workspaces/git/TFSign/setttings.json";
    std::cout<<"Settings file:"<<l_str<<std::endl;

    my::Settings l_settings = my::Settings::readFile(l_str); 

    printSettings(l_settings);
    
    my::ColorFilter l_prepocess(l_settings);
    cv::Mat l_img = cv::imread("/home/nandi/Roadsigns.jpg", CV_LOAD_IMAGE_COLOR);
    my::ColorFilter::ColorFilter_Data l_resData = l_prepocess.apply(l_img);
    // cv::Mat l_img_res = l_prepocess.apply(l_img);

    my::ImageSegment l_segment(l_settings);
    std::vector<my::ImageSegment::Segment_t> l_segments;
    l_segment.apply(l_resData.blueMask,l_resData.redMask,l_segments);
    // l_segment.segmenting(l_img,l_segments);
    
    cv::Mat l_imgFinalRes;
    cv::bitwise_and(l_img, l_img, l_imgFinalRes, l_resData.Mask);
    drawSquare(l_segments,l_img);
    cv::imshow("Rectangle",l_img);
    cv::imshow("Test",l_imgFinalRes);
    cv::waitKey();
    
    return 0;
}