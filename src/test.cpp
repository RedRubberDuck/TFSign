#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>


#include <imgprocessing/colorfilter.hpp>
#include <imgprocessing/segmentation.hpp>
#include <settings/settings.hpp>

#include <time.h>
#include <iostream>


#define FILTER_NR_TEST 50
#define FILTER_NR_NEGATIV_TEST 20



void generateRandomPixelValue( ::cv::Mat& f_mat,uint8_t f_i,::cv::Vec3b f_inf,::cv::Vec3b f_sup){
    ::cv::Vec3b l_size = f_sup-f_inf;
    if(l_size[0]==0 && l_size[1]==0 && l_size[1]==0) return;
    for(uint8_t i=0;i<FILTER_NR_TEST;++i){
        uint8_t  l_h,l_s,l_v;
        if(l_size[0]!=0) l_h =  rand()%l_size[0]+f_inf[0];
        else l_h = f_inf[0]+1;

        if(l_size[1]!=0) l_s =  rand()%l_size[1]+f_inf[1];
        else l_s = f_inf[1]+1;
        if(l_size[2]!=0) l_v =  rand()%l_size[2]+f_inf[2];
        else l_v = f_inf[2]+1;
        // cv::Vec3b l(l_h,l_s,l_v);
        f_mat.at<cv::Vec3b>(f_i,i)[0]=l_h;
        f_mat.at<cv::Vec3b>(f_i,i)[1]=l_s;
        f_mat.at<cv::Vec3b>(f_i,i)[2]=l_v;

    }
}

TEST(TestColorFilter, FilterTest) { 
    std::string l_str = "../settings_v3.json";

    my::Settings l_settings = my::Settings::readFile(l_str);

    my::ColorFilter     l_colorFilter(l_settings);
    cv::Vec3b           l_inf(0,0,0);
    cv::Vec3b           l_sup(180,255,255);

    ::cv::Mat l_testImg(6, FILTER_NR_TEST, CV_8UC3);
    generateRandomPixelValue(l_testImg,0,l_settings.getblueInferior(),l_settings.getblueSuperior());
    generateRandomPixelValue(l_testImg,1,l_inf,l_settings.getblueInferior());
    generateRandomPixelValue(l_testImg,2,l_settings.getblueSuperior(),l_sup);
    
    generateRandomPixelValue(l_testImg,3,l_settings.getred1Inferior(),l_settings.getred1Superior()-cv::Vec3b(1,1,1));
    generateRandomPixelValue(l_testImg,4,l_settings.getred2Inferior(),l_settings.getred2Superior()-cv::Vec3b(1,1,1));
    generateRandomPixelValue(l_testImg,5,l_settings.getred1Superior(),l_settings.getred2Inferior()-cv::Vec3b(1,1,1));
    
    cv::Mat l_imgBGR;
    cv::cvtColor(l_testImg,l_imgBGR,cv::COLOR_HSV2BGR);
    my::ColorFilter::ColorFilter_Data l_res = l_colorFilter.apply(l_imgBGR);


    for(uint8_t i=0;i<FILTER_NR_TEST;++i){
        ASSERT_EQ(255, l_res.blueMask.at<uint8_t>(0,i));
        ASSERT_EQ(0, l_res.blueMask.at<uint8_t>(1,i));
        ASSERT_EQ(0, l_res.blueMask.at<uint8_t>(2,i));

        ASSERT_EQ(255, l_res.redMask.at<uint8_t>(3,i));
        ASSERT_EQ(255, l_res.redMask.at<uint8_t>(4,i));
        ASSERT_EQ(0, l_res.redMask.at<uint8_t>(5,i));
    }
}

TEST(TestSegmenting,SimpleBlue) { 
    ::cv::Mat l_testBlue(400, 400, CV_8UC1,cv::Scalar(0));
    ::cv::Mat l_testRed(400, 400, CV_8UC1,cv::Scalar(0));
    // cv::bitwise_or(l_imgBlueMask,l_SummedRedMask,l_SummedMask);
    ::cv::rectangle(l_testBlue,cv::Point(50,50),cv::Point(350,350),255);
    std::string l_str = "../settings_v3.json";
    my::Settings l_settings = my::Settings::readFile(l_str);
    my::ImageSegment    l_segment(l_settings);

    std::vector<my::ImageSegment::Segment_t> l_segments;
    l_segment.apply(l_testBlue,l_testRed,l_segments);
    std::vector<my::ImageSegment::Segment_t>::iterator l_it;
    ASSERT_EQ(1,l_segments.size());
    my::ImageSegment::Segment_t l_imgsegment = l_segments[0];
    ASSERT_EQ(50,l_imgsegment.top);
    ASSERT_EQ(50,l_imgsegment.left);
    ASSERT_EQ(301,l_imgsegment.width);
    ASSERT_EQ(301,l_imgsegment.height);
    ASSERT_EQ(my::ImageSegment::ColorTypes_t::BLUE,l_imgsegment.color);
    
    // for (l_it=l_segments.begin();l_it!=l_segments.end();++l_it)
    //     std::cout<<l_it->color<<" "<<l_it->left<<" "<<l_it->top<<" "<<l_it->width<<" "<<l_it->height<<std::endl;

}


TEST(TestSegmenting,SimpleRED) { 
    ::cv::Mat l_testBlue(400, 400, CV_8UC1,cv::Scalar(0));
    ::cv::Mat l_testRed(400, 400, CV_8UC1,cv::Scalar(0));
    // cv::bitwise_or(l_imgBlueMask,l_SummedRedMask,l_SummedMask);
    ::cv::rectangle(l_testRed,cv::Point(50,50),cv::Point(350,350),255);
    std::string l_str = "../settings_v3.json";
    my::Settings l_settings = my::Settings::readFile(l_str);
    my::ImageSegment    l_segment(l_settings);

    std::vector<my::ImageSegment::Segment_t> l_segments;
    l_segment.apply(l_testBlue,l_testRed,l_segments);
    std::vector<my::ImageSegment::Segment_t>::iterator l_it;
    ASSERT_EQ(1,l_segments.size());
    my::ImageSegment::Segment_t l_imgsegment = l_segments[0];
    ASSERT_EQ(50,l_imgsegment.top);
    ASSERT_EQ(50,l_imgsegment.left);
    ASSERT_EQ(301,l_imgsegment.width);
    ASSERT_EQ(301,l_imgsegment.height);
    ASSERT_EQ(my::ImageSegment::ColorTypes_t::RED,l_imgsegment.color);
}



TEST(TestSegmenting,DoubleBlue) { 
    ::cv::Mat l_testBlue(400, 400, CV_8UC1,cv::Scalar(0));
    ::cv::Mat l_testRed(400, 400, CV_8UC1,cv::Scalar(0));
    // cv::bitwise_or(l_imgBlueMask,l_SummedRedMask,l_SummedMask);
    ::cv::rectangle(l_testBlue,cv::Point(50,50),cv::Point(350,350),255);
    ::cv::rectangle(l_testRed,cv::Point(150,150),cv::Point(250,250),255);
    std::string l_str = "../settings_v3.json";
    my::Settings l_settings = my::Settings::readFile(l_str);
    my::ImageSegment    l_segment(l_settings);

    std::vector<my::ImageSegment::Segment_t> l_segments;
    l_segment.apply(l_testBlue,l_testRed,l_segments);
    std::vector<my::ImageSegment::Segment_t>::iterator l_it;
    ASSERT_EQ(1,l_segments.size());
    my::ImageSegment::Segment_t l_imgsegment = l_segments[0];
    ASSERT_EQ(50,l_imgsegment.top);
    ASSERT_EQ(50,l_imgsegment.left);
    ASSERT_EQ(301,l_imgsegment.width);
    ASSERT_EQ(301,l_imgsegment.height);
    ASSERT_EQ(my::ImageSegment::ColorTypes_t::BLUE,l_imgsegment.color);
}


TEST(TestSegmenting,DoubleRed) { 
    ::cv::Mat l_testBlue(400, 400, CV_8UC1);
    ::cv::Mat l_testRed(400, 400, CV_8UC1);
    // cv::bitwise_or(l_imgBlueMask,l_SummedRedMask,l_SummedMask);
    ::cv::rectangle(l_testRed,cv::Point(50,50),cv::Point(350,350),255);
    ::cv::rectangle(l_testBlue,cv::Point(150,150),cv::Point(250,250),255);
    std::string l_str = "../settings_v3.json";
    my::Settings l_settings = my::Settings::readFile(l_str);
    my::ImageSegment    l_segment(l_settings);

    std::vector<my::ImageSegment::Segment_t> l_segments;
    l_segment.apply(l_testBlue,l_testRed,l_segments);
    std::vector<my::ImageSegment::Segment_t>::iterator l_it;
    ASSERT_EQ(1,l_segments.size());
    my::ImageSegment::Segment_t l_imgsegment = l_segments[0];
    ASSERT_EQ(50,l_imgsegment.top);
    ASSERT_EQ(50,l_imgsegment.left);
    ASSERT_EQ(301,l_imgsegment.width);
    ASSERT_EQ(301,l_imgsegment.height);
    ASSERT_EQ(my::ImageSegment::ColorTypes_t::RED,l_imgsegment.color);
}


int main(int argc, char **argv) {
    srand (time(NULL));
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}