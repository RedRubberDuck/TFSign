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

void countour(std::vector<my::ImageSegment::Segment_t> l_segments,cv::Mat& l_mask){
    std::vector<my::ImageSegment::Segment_t>::iterator it;
    uint l_kernelSize = 1;
    cv::Mat l_kernel = cv::getStructuringElement(cv::MORPH_CROSS,cv::Size(2 * l_kernelSize + 1, 2 * l_kernelSize + 1),cv::Point(l_kernelSize, l_kernelSize));
    cv::RNG rng(12345);
    for (it=l_segments.begin();it!=l_segments.end();++it){
        
        
        cv::Range l_x(it->left,it->left+it->width);
        cv::Range l_y(it->top,it->top+it->height);

        cv::Mat l_sign;
        // =cv::Mat::zeros(it->width+10,it->height+10,CV_8UC3);

        cv::copyMakeBorder( l_mask(l_y,l_x), l_sign, 5, 5, 5, 5, cv::BORDER_CONSTANT,cv::Scalar(0,0,0));
        
        
        // cv::Mat l_edge;
        // cv::Laplacian(x1,l_edge,CV_8U, 2, 1, 0, cv::BORDER_DEFAULT );
        // cv::morphologyEx(x1,l_edge,cv::MORPH_GRADIENT,l_kernel);

        std::vector<std::vector<cv::Point> > l_contours;
        cv::findContours( l_sign, l_contours,cv::RETR_EXTERNAL  ,cv:: CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

        cv::Mat drawing = cv::Mat::zeros( l_sign.size(), CV_8UC3 );
        for( size_t i = 0; i< l_contours.size(); i++ )
        {
            cv::Scalar color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
            cv::drawContours( drawing, l_contours, (int)i, color);
        }

        cv::imshow("W",l_sign);
        cv::imshow("Count",drawing);
        cv::waitKey();
        cv::destroyAllWindows();
    }
}


int main(int argc, char** argv )
{
    // std::string l_str = "/home/nandi/Workspaces/git/TFSign/setttings.json";
    std::string l_str = "C:\\Users\\aki5clj\\Documents\\Git\\WorkspaceC_C++\\TFSign\\setttings.json";
    std::cout<<"Settings file:"<<l_str<<std::endl;

    my::Settings l_settings = my::Settings::readFile(l_str); 

    printSettings(l_settings);
    
    my::ColorFilter l_prepocess(l_settings);
    cv::Mat l_img = cv::imread("C:\\Users\\aki5clj\\Documents\\Git\\WorkspaceC_C++\\resource\\traffic_signs.png", CV_LOAD_IMAGE_COLOR);
    std::cout << l_img.size() << std::endl;
    // cv::Mat l_img = cv::imread("/home/nandi/Roadsigns.jpg", CV_LOAD_IMAGE_COLOR);
    my::ColorFilter::ColorFilter_Data l_resData = l_prepocess.apply(l_img);
    // cv::Mat l_img_res = l_prepocess.apply(l_img);

    my::ImageSegment l_segment(l_settings);
    std::vector<my::ImageSegment::Segment_t> l_segments;
    l_segment.apply(l_resData.blueMask,l_resData.redMask,l_segments);
    // l_segment.segmenting(l_img,l_segments);
    
    cv::Mat l_imgFinalRes;
    cv::bitwise_and(l_img, l_img, l_imgFinalRes, l_resData.Mask);
    countour(l_segments,l_resData.Mask);

    drawSquare(l_segments,l_img);
    cv::imshow("Rectangle",l_img);
    cv::imshow("Test",l_imgFinalRes);
    cv::waitKey();
    
    return 0;
}