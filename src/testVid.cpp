#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>
#include <ml/utilities.hpp>

#include <settings/settings.hpp>

#include <time.h>
#include <iostream>



TEST(TestTrafficSign, Stop) {
    std::string l_settings_str = "../settings_v3.json";
    my::Settings l_settings = my::Settings::readFile(l_settings_str);
    std::vector<my::TrafficSignProcessing::TrafficSignSegment> l_signs;
    my::TrafficSignProcessing l_trafficProcessing(l_settings);

    std::string l_fileStop("../testVid/stop.h264");
    cv::VideoCapture l_videoCapture(l_fileStop);
    ASSERT_TRUE(l_videoCapture.isOpened());

    ::cv::Mat l_frame;

    uint l_nrFrame=0;
    uint l_nrDetected=0;
    uint l_wrongDetection=0;
    while(true){
        l_videoCapture>>l_frame;
        if(l_frame.empty()) break;
        cv::Mat l_fram;
        cv::resize(l_frame,l_fram,cv::Size(640,480));
        l_trafficProcessing.processFrameAndDraw(l_fram,l_signs);
        // std::cout<<l_signs.size()<<std::endl;
        
        std::vector<my::TrafficSignProcessing::TrafficSignSegment>::iterator l_it;
        bool l_stopDetected=false;
        for(l_it=l_signs.begin();l_it<l_signs.end();++l_it){
            if(l_it->signType==my::TrafficSignProcessing::TrafficSignTypes_t::StopSign){
                if(!l_stopDetected){
                    l_stopDetected=true;
                    ++l_nrDetected;
                }else{
                    FAIL();
                }
                
            }else if(l_it->signType!=my::TrafficSignProcessing::TrafficSignTypes_t::None){
                l_wrongDetection++;
            }
        }
        
        l_signs.clear();
        ++l_nrFrame;

        cv::imshow("Video",l_fram);
        char c=(char)cv::waitKey();
        if(c==27)
        break;
    }
    std::cout<<"Nr.frame:"<<l_nrFrame<<"   Nr.detected:"<<l_nrDetected<<"   Correctness[%]"<<static_cast<float>(l_nrDetected)/l_nrFrame*100.0<<std::endl;
    std::cout<<"Nr. wrong sign:"<<l_wrongDetection<<"   Wrongness[%]"<<static_cast<float>(l_wrongDetection)/l_nrFrame*100.0<<std::endl;
    ASSERT_NEAR(l_nrFrame,l_nrDetected,l_nrFrame*0.3);
    
}


TEST(TestTrafficSign, Parking) {
    std::string l_settings_str = "../settings_v3.json";
    my::Settings l_settings = my::Settings::readFile(l_settings_str);
    std::vector<my::TrafficSignProcessing::TrafficSignSegment> l_signs;
    my::TrafficSignProcessing l_trafficProcessing(l_settings);

    std::string l_fileStop("../testVid/parking.h264");
    cv::VideoCapture l_videoCapture(l_fileStop);
    ASSERT_TRUE(l_videoCapture.isOpened());

    ::cv::Mat l_frame;

    uint l_nrFrame=0;
    uint l_nrDetected=0;
    uint l_wrongDetection=0;
    while(true){
        l_videoCapture>>l_frame;
        if(l_frame.empty()) break;
        cv::Mat l_fram;
        cv::resize(l_frame,l_fram,cv::Size(640,480));
        l_trafficProcessing.processFrameAndDraw(l_fram,l_signs);
        
        // std::cout<<l_signs.size()<<std::endl;
        
        std::vector<my::TrafficSignProcessing::TrafficSignSegment>::iterator l_it;
        bool l_parkingDetected=false;
        for(l_it=l_signs.begin();l_it<l_signs.end();++l_it){
            if(l_it->signType==my::TrafficSignProcessing::TrafficSignTypes_t::ParkingSign){
                if(!l_parkingDetected){
                    l_parkingDetected=true;
                    ++l_nrDetected;
                }else{
                    // FAIL();
                }
                
            }else if(l_it->signType!=my::TrafficSignProcessing::TrafficSignTypes_t::None){
                l_wrongDetection++;
            }
        }
        
        l_signs.clear();
        ++l_nrFrame;
   
        cv::imshow("Video",l_fram);
        char c=(char)cv::waitKey();
        if(c==27)
        break;
    }

    

    std::cout<<"Nr.frame:"<<l_nrFrame<<"   Nr:detected:"<<l_nrDetected<<"   Correctness[%]"<<static_cast<float>(l_nrDetected)/l_nrFrame*100.0<<std::endl;
    std::cout<<"Nr. wrong sign:"<<l_wrongDetection<<"   Wrongness[%]"<<static_cast<float>(l_wrongDetection)/l_nrFrame*100.0<<std::endl;
    ASSERT_NEAR(l_nrFrame,l_nrDetected,l_nrFrame*0.3);
    
}
    
int main(int argc, char **argv) {
    srand (time(NULL));
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}