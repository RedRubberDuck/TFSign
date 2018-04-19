#include <imgprocessing/colorfilter.hpp>
#include <imgprocessing/segmentation.hpp>
#include <settings/settings.hpp>

#include <opencv2/opencv.hpp>
// #include <opencv2/>

#include <string>
#include <iostream>
#include <sstream>
#include <dirent.h>

void readFromFolderImages(const std::string &,std::vector<cv::Mat>&);


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

void applyhog(cv::Mat l_img);

void saveImage(uint index, const cv::Mat l_img);

void countour(std::vector<my::ImageSegment::Segment_t> l_segments,cv::Mat& l_mask){
    std::vector<my::ImageSegment::Segment_t>::iterator it;
    uint l_kernelSize = 1;
    cv::Mat l_kernel = cv::getStructuringElement(cv::MORPH_CROSS,cv::Size(2 * l_kernelSize + 1, 2 * l_kernelSize + 1),cv::Point(l_kernelSize, l_kernelSize));
    cv::RNG rng(12345);
    uint i = 0;
    for (it=l_segments.begin();it!=l_segments.end();++it){
        
        
        cv::Range l_x(it->left,it->left+it->width);
        cv::Range l_y(it->top,it->top+it->height);

        cv::Mat l_sign;
        cv::resize(l_mask(l_y,l_x),l_sign,cv::Size(64,64));
        applyhog(l_sign);
      
        ++i;
        

        cv::imshow("W",l_sign);
        // // cv::imshow("Count",drawing);
        cv::waitKey();
        // cv::destroyAllWindows();
    }
}


int main(int argc, char** argv )
{
    // std::string l_str = "/home/nandi/Workspaces/git/TFSign/setttings.json";
    std::string l_str = "..\\setttings.json";
    std::cout<<"Settings file:"<<l_str<<std::endl;
    std::vector<cv::Mat> l_imgContainer;
    
    my::Settings l_settings = my::Settings::readFile(l_str); 
    readFromFolderImages(l_settings.getStopFolder(),l_imgContainer);
    my::ColorFilter l_prepocess(l_settings);
    cv::Mat l_img = cv::imread(l_settings.getImageName().c_str(), CV_LOAD_IMAGE_COLOR);
    // cv::Mat l_img = cv::imread("/home/nandi/Roadsigns.jpg", CV_LOAD_IMAGE_COLOR);
    // cv::Mat l_img = cv::imread("/home/nandi/Workspaces/git/resource/TrafficSignPics/Stop.png", CV_LOAD_IMAGE_COLOR);
    my::ColorFilter::ColorFilter_Data l_resData = l_prepocess.apply(l_img);
    // cv::Mat l_img_res = l_prepocess.apply(l_img);

    my::ImageSegment l_segment(l_settings);
    std::vector<my::ImageSegment::Segment_t> l_segments;
    l_segment.apply(l_resData.blueMask,l_resData.redMask,l_segments);
    // l_segment.segmenting(l_img,l_segments);
    
    cv::Mat l_imgFinalRes;
    cv::bitwise_and(l_img, l_img, l_imgFinalRes, l_resData.Mask);
    countour(l_segments,l_img);

    drawSquare(l_segments,l_img);
    cv::imshow("Rectangle",l_img);
    cv::imshow("Test",l_imgFinalRes);
    cv::waitKey();
    
    return 0;
}




void applyhog(cv::Mat l_img){

    cv::cvtColor(l_img, l_img, cv::COLOR_BGR2GRAY);
    cv::HOGDescriptor l_hog(l_img.size(),cv::Size(16,16),cv::Size(8,8),cv::Size(8,8), 9, 1);
    std::vector<float> l_descriptors;
    l_hog.compute(l_img, l_descriptors,cv::Size( 8, 8 ),cv::Size( 0, 0 ));
    std::vector<float>::iterator it;
    std::cout << "Size description:" << l_descriptors.size() << std::endl;
    cv::get_hogdescriptor_visual_image();
    // for (it = l_descriptors.begin();it!=l_descriptors.end();++it){
    //     std::cout << " " << *it << ",";
    // }
}


void saveImage(uint index, const cv::Mat l_img){
    std::ostringstream s;
    s << "img" << index<<".jpg";
    std::string query(s.str());
    cv::imwrite(query.c_str(), l_img);
}



void readFromFolderImages(const std::string & f_folder, std::vector<cv::Mat>& l_imgContainer ){
    DIR *dir  =opendir(f_folder.c_str());
    if(dir){
        struct dirent *ent;
        while((ent = readdir(dir)) !=NULL){
            std::string l_fileName = ent->d_name;
            std::string l_fileExtension = l_fileName.substr(l_fileName.find_last_of(".") + 1);
            if(l_fileExtension == "jpg" || l_fileExtension == "ppm" || l_fileExtension == "png"){
                std::string l_fullpath = f_folder;
                l_fullpath.append(l_fileName);
                cv::Mat l_img = cv::imread(l_fullpath, CV_LOAD_IMAGE_COLOR);
                l_imgContainer.push_back(l_img);
            }
        }
    }
}




// DELETED CODES
// cv::copyMakeBorder( l_mask(l_y,l_x), l_sign, 5, 5, 5, 5, cv::BORDER_CONSTANT,cv::Scalar(255,255,255));
        

// // cv::Mat l_edge;
// // cv::Laplacian(x1,l_edge,CV_8U, 2, 1, 0, cv::BORDER_DEFAULT );
// // cv::morphologyEx(x1,l_edge,cv::MORPH_GRADIENT,l_kernel);

// // std::vector<std::vector<cv::Point> > l_contours;
// // cv::findContours( l_sign, l_contours,cv::RETR_EXTERNAL  ,cv:: CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

// // cv::Mat drawing = cv::Mat::zeros( l_sign.size(), CV_8UC3 );
// // for( size_t i = 0; i< l_contours.size(); i++ )
// // {
// //     cv::Scalar color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
// //     cv::drawContours( drawing, l_contours, (int)i, color);
// // }