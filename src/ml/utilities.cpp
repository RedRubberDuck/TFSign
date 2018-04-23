#include <ml/utilities.hpp>



void my::readFromFolderImages(const std::string & f_folder, std::vector<cv::Mat>& l_imgContainer ){
    DIR *dir  =opendir(f_folder.c_str());
    if(dir){
        struct dirent *ent;
        while((ent = readdir(dir)) !=NULL){
            std::string l_fileName = ent->d_name;
            
            std::string l_fileExtension = l_fileName.substr(l_fileName.find_last_of(".") + 1);
            if(l_fileExtension == "jpg" || l_fileExtension == "ppm" || l_fileExtension == "png"){
                // std::cout << ent->d_name << std::endl;
                std::string l_fullpath = f_folder;
                l_fullpath.append(l_fileName);
                cv::Mat l_img = cv::imread(l_fullpath, CV_LOAD_IMAGE_COLOR);
                l_imgContainer.push_back(l_img);
            }
        }
    }
}


void my::testSVMBigFrame(   const std::string&      f_folderName
                            ,const std::string&     f_svmFileXML
                            ,my::ColorFilter&       f_colorfilter
                            ,my::ImageSegment&      f_segmenting
                            ,my::HogCalculation&    f_hogCalc){
    std::vector<cv::Mat> l_imgContainer;
    readFromFolderImages(f_folderName,l_imgContainer);
    std::vector<cv::Mat>::iterator l_imgIt;

    cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
    svm = svm->load(f_svmFileXML);
    std::vector<my::ImageSegment::Segment_t> l_StopSign;
    std::vector<my::ImageSegment::Segment_t> l_ParkingSign;
    std::vector<my::ImageSegment::Segment_t> l_FalseSign;

    for(l_imgIt= l_imgContainer.begin(); l_imgIt<l_imgContainer.end() ; ++l_imgIt){
        cv::imshow("Img",*l_imgIt);
        my::ColorFilter::ColorFilter_Data l_filterRes = f_colorfilter.apply(*l_imgIt);
        cv::imshow("Filt",l_filterRes.Mask);
        std::vector<my::ImageSegment::Segment_t> l_segments;
        f_segmenting.apply(l_filterRes.blueMask,l_filterRes.redMask,l_segments);
        std::vector<my::ImageSegment::Segment_t>::iterator it;
        std::cout << "nr. segment:" << l_segments.size() << std::endl;
        for (it = l_segments.begin(); it!=l_segments.end();++it){
            cv::Range l_x(it->left,it->left+it->width);
            cv::Range l_y(it->top,it->top+it->height);
            cv::Mat l_sign;
            cv::resize((*l_imgIt)(l_y,l_x),l_sign,cv::Size(64,64));
            
            
            std::vector<float> l_descriptors;
            f_hogCalc.apply(l_sign,l_descriptors);
            cv::Mat l_data(1, l_descriptors.size(), CV_32FC1, l_descriptors.data());
            unsigned int l_res =static_cast<unsigned int>(svm->predict(l_data));
            switch(l_res){
                case 0:
                    std::cout << "Parking sign" << std::endl;
                    l_ParkingSign.push_back(*it);
                    break;
                case 1:
                    std::cout << "Stop sign" << std::endl;
                    l_StopSign.push_back(*it);
                    break;

                case 2:
                    std::cout << "False sign" << std::endl;
                    l_FalseSign.push_back(*it);
                    break;
            }            
            // cv::imshow(" ",l_sign);
            // cv::waitKey();
            
        }



        cv::waitKey();
    }
}


void my::drawRenctangles(   cv::Mat&                                        f_img
                            ,std::vector<my::ImageSegment::Segment_t>       f_StopSign
                            , std::vector<my::ImageSegment::Segment_t>      f_parkingSign
                            , std::vector<my::ImageSegment::Segment_t>      f_falseNegativ){
    uint i= 0;
}