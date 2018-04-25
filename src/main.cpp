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
    std::string l_str = "..\\setttingsWin.json";
    std::cout<<"Settings file:"<<l_str<<std::endl;
    my::Settings l_settings = my::Settings::readFile(l_str); 

    std::vector<cv::Mat> l_imgContainer;

    my::ColorFilter     l_colorFilter(l_settings);
    my::HogCalculation  l_hog(l_settings);
    my::ImageSegment    l_segment(l_settings);
    
    // ---------------------------------------------------------------------------------------------------------------
    std::array<std::string,3> l_trainFolders({  l_settings.getParkingTrainFolder()
                                                ,l_settings.getStopTrainFolder()
                                                ,l_settings.getNegativTrainFolder()
                                                });
                                                // l_settings.getParkingTrainFolder()
                                                // ,l_settings.getStopTrainFolder()
                                                // ,l_settings.getNegativTrainFolder()
    std::array<std::string,3> l_dataTrainFile({"parkingTrainData","stopTrainData","negativTrainData"});
    // ,"stopTrainData","negativTrainData"
    // ---------------------------------------------------------------------------------------------------------------
    std::array<std::string,3> l_testFolders({  l_settings.getParkingTestFolder()
                                                ,l_settings.getStopTestFolder()
                                                ,l_settings.getNegativTestFolder()});
    std::array<std::string,3> l_dataTestFile({"parkingTestData","stopTestData","negativTestData"});
    

    std::cout<<"Train"<<l_settings.getStopTrainFolder()<<std::endl;
    std::cout<<"Test"<<l_settings.getStopTestFolder()<<std::endl;
    std::cout<<"Train"<<l_settings.getParkingTrainFolder()<<std::endl;
    std::cout<<"Test"<<l_settings.getParkingTestFolder()<<std::endl;
    std::cout<<"Train"<<l_settings.getNegativTrainFolder()<<std::endl;
    std::cout<<"Test"<<l_settings.getNegativTestFolder()<<std::endl;

    my::readAndSaveFeatures<3>(l_trainFolders,l_dataTrainFile,l_colorFilter,l_segment,l_hog);
    my::readAndSaveFeatures<3>(l_testFolders,l_dataTestFile,l_colorFilter,l_segment,l_hog);
    
    my::readAndTrainSVM<3>(l_dataTrainFile,"svm4.xml");
    my::testSVM<3>(l_dataTestFile,"svm4.xml");
    // C:/Users/aki5clj/Documents/Git/WorkspaceC_C++/resource/TFSign/fullFrame/
    // /home/nandi/Workspaces/git/resource/TFSign/fullFrame/
    my::testSVMBigFrame("C:/Users/aki5clj/Documents/Git/WorkspaceC_C++/resource/Video/TrainAndTest/fullframe/","svm4.xml",l_colorFilter,l_segment,l_hog);



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