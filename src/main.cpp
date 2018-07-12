#include <imgprocessing/colorfilter.hpp>
#include <imgprocessing/segmentation.hpp>
#include <imgprocessing/hogCalc.hpp>
#include <settings/settings.hpp>
#include <file/fileHandler.hpp>
#include <ml/utilities.hpp>


#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>
// #include <opencv2/>

#include <string>
#include <array>
#include <iostream>
#include <sstream>
#include <dirent.h>



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
    // std::string l_str = "/home/nandi/Workspaces/git/TFSign/setttings.json";
    std::string l_str = "../settings_v3.json";
    std::cout<<"Settings file:"<<l_str<<std::endl;
    my::Settings l_settings = my::Settings::readFile(l_str);

    std::vector<cv::Mat> l_imgContainer;

    my::ColorFilter     l_colorFilter(l_settings);
    my::HogCalculation  l_hog(l_settings);
    my::ImageSegment    l_segment(l_settings);

    // ---------------------------------------------------------------------------------------------------------------
    // std::array<std::string,4> l_trainFolders({  l_settings.getNegativTrainFolder()
    //                                             ,l_settings.getParkingTrainFolder()
    //                                             ,l_settings.getStopTrainFolder()
    //
    //                                             });
    // std::array<std::string,7> l_trainFolders({  "/mnt/0782B895192F2812/Workspaces/work/trafficSign/limit30/train/"
    //                                             ,"/mnt/0782B895192F2812/Workspaces/work/trafficSign/nolimit30/train/"
    //                                             ,"/mnt/0782B895192F2812/Workspaces/work/trafficSign/limit50/train/"
    //                                             ,"/mnt/0782B895192F2812/Workspaces/work/trafficSign/nolimit50/train/"
    //                                             ,"/mnt/0782B895192F2812/Workspaces/work/trafficSign/noparking/train/"
    //                                             ,"/mnt/0782B895192F2812/Workspaces/work/trafficSign/nostop/train/"
    //                                             ,"/mnt/0782B895192F2812/Workspaces/work/trafficSign/circle/train/"
    //                                             });
    //
    //                                             // l_settings.getParkingTrainFolder()
    //                                             // ,l_settings.getStopTrainFolder()
    //                                             // ,l_settings.getNegativTrainFolder()
    // // std::array<std::string,4> l_dataTrainFile({"negativTrainData","parkingTrainData","stopTrainData","limit30TrainData"});
    // std::array<std::string,7> l_dataTrainFile({"limit30TrainData","nolimit30TrainData","limit50TrainData","nolimit50TrainData","noparkingTrainData","nostopTrainData","circleTrainData"});
    // std::array<std::string,10> l_dataTrainFile({"negativTrainData","parkingTrainData","stopTrainData","limit30TrainData","nolimit30TrainData","limit50TrainData","nolimit50TrainData","noparkingTrainData","nostopTrainData","circleTrainData"});
    // ,"stopTrainData","negativTrainData"
    // ---------------------------------------------------------------------------------------------------------------
    // std::array<std::string,4> l_testFolders({  l_settings.getNegativTestFolder()
    //                                             ,l_settings.getParkingTestFolder()
    //                                             ,l_settings.getStopTestFolder()
    //                                             ,"/mnt/0782B895192F2812/Workspaces/work/trafficSign/limit30/test/"
    //                                           });
    // std::array<std::string,7> l_testFolders({  "/mnt/0782B895192F2812/Workspaces/work/trafficSign/limit30/test/"
    //                                             ,"/mnt/0782B895192F2812/Workspaces/work/trafficSign/nolimit30/test/"
    //                                             ,"/mnt/0782B895192F2812/Workspaces/work/trafficSign/limit50/test/"
    //                                             ,"/mnt/0782B895192F2812/Workspaces/work/trafficSign/nolimit50/test/"
    //                                             ,"/mnt/0782B895192F2812/Workspaces/work/trafficSign/noparking/test/"
    //                                             ,"/mnt/0782B895192F2812/Workspaces/work/trafficSign/nostop/test/"
    //                                             ,"/mnt/0782B895192F2812/Workspaces/work/trafficSign/circle/test/"
    //                                             });
    // std::array<std::string,7> l_dataTestFile({"limit30TestData","nolimit30TestData","limit50TestData","nolimit50TestData","noparkingTestData","nostopTestData","circleTestData"});
    // std::array<std::string,4> l_dataTestFile({"negativTestData","parkingTestData","stopTestData","limit30TestData"});
    // std::array<std::string,3> l_dataTestFile({"nolimit30TestData","limit50TestData","nolimit50TestData"});
    // std::array<std::string,10> l_dataTestFile({"negativTestData","parkingTestData","stopTestData","limit30TestData","nolimit30TestData","limit50TestData","nolimit50TestData","noparkingTrainData","nostopTrainData","circleTrainData"});


    // std::cout<<"Train"<<l_settings.getStopTrainFolder()<<std::endl;
    // std::cout<<"Test"<<l_settings.getStopTestFolder()<<std::endl;
    // std::cout<<"Train"<<l_settings.getParkingTrainFolder()<<std::endl;
    // std::cout<<"Test"<<l_settings.getParkingTestFolder()<<std::endl;
    // std::cout<<"Train"<<l_settings.getNegativTrainFolder()<<std::endl;
    // std::cout<<"Test"<<l_settings.getNegativTestFolder()<<std::endl;

    // my::readAndSaveFeatures<7>(l_trainFolders,l_dataTrainFile,l_colorFilter,l_segment,l_hog);
    // my::readAndSaveFeatures<7>(l_testFolders,l_dataTestFile,l_colorFilter,l_segment,l_hog);
    //
    // my::readAndTrainSVM<4>(l_dataTrainFile,"svmA1.xml");
    // my::testSVM<4>(l_dataTestFile,"svmA1.xml");

    // my::readAndTrainSVM<10>(l_dataTrainFile,"svmB4.xml");
    // my::testSVM<10>(l_dataTestFile,"svmB4.xml");
    // C:/Users/aki5clj/Documents/Git/WorkspaceC_C++/resource/TFSign/fullFrame/
    // /home/nandi/Workspaces/git/resource/TFSign/fullFrame/
    // C:/Users/aki5clj/Documents/Git/WorkspaceC_C++/resource/Video/TrainAndTest/fullframe/
    // my::testSVMBigFrame("/home/nandi/Workspaces/git/new_resource/videoPhone/picsOut/","svm0.xml",l_colorFilter,l_segment,l_hog);
    // /home/nandi/Workspaces/git/resource/TFSign/Video/InitialVideo/
    my::testVideo("/mnt/0782B895192F2812/Workspaces/work/trafficSign/testVideo/","svmA3.xml",l_str);

    // "negativFolder":"/home/nandi/Workspaces/git/resource/positivStopTest/",
    // "negativFolder":"/home/nandi/Workspaces/git/resource/TFSign/Belga/00045/",

    // applyAllSaveDescription(l_imgContainer, l_colorFilter,l_segment,l_hog);
    // readAndTrainSVM();
    // testSVM(l_colorFilter,l_segment,l_hog);
    // cv::waitKey();

    return 0;
}





void saveImage(uint index, const cv::Mat l_img){
    std::ostringstream s;
    s << "img" << index<<".jpg";
    std::string query(s.str());
    cv::imwrite(query.c_str(), l_img);
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
