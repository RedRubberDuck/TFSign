#include <ml/utilities.hpp>



void my::readFromFolderImages(const std::string & f_folder, std::vector<cv::Mat>& l_imgContainer ){
    DIR *dir  =opendir(f_folder.c_str());
    if(dir){
        struct dirent *ent;
        while((ent = readdir(dir)) !=NULL){
            std::string l_fileName = ent->d_name;

            std::string l_fileExtension = l_fileName.substr(l_fileName.find_last_of(".") + 1);
            if(l_fileExtension == "jpg" || l_fileExtension == "ppm" || l_fileExtension == "png" || l_fileExtension  == "jp2"){
                // std::cout << ent->d_name << std::endl;
                std::string l_fullpath = f_folder;
                l_fullpath.append(l_fileName);
                cv::Mat l_img = cv::imread(l_fullpath, cv::IMREAD_COLOR);
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
    std::vector<my::ImageSegment::Segment_t> l_UnicSign;
    uint iImg=0;
    for(l_imgIt= l_imgContainer.begin(); l_imgIt<l_imgContainer.end() ; ++l_imgIt,++iImg){
        my::ColorFilter::ColorFilter_Data l_filterRes = f_colorfilter.apply(*l_imgIt);
        std::vector<my::ImageSegment::Segment_t> l_segments;
        f_segmenting.apply(l_filterRes.blueMask,l_filterRes.redMask,l_segments);
        std::vector<my::ImageSegment::Segment_t>::iterator it;
        std::cout << "nr. segment:" << l_segments.size() << std::endl;
        for (it = l_segments.begin(); it!=l_segments.end();++it){
            cv::Range l_x(it->left,it->left+it->width);
            cv::Range l_y(it->top,it->top+it->height);
            cv::Mat l_sign=(*l_imgIt)(l_y,l_x);
            // cv::resize(,l_sign,cv::Size(64,64));


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
                case 3:
                    std::cout << "Unic sign" << std::endl;
                    l_UnicSign.push_back(*it);
                    break;
            }
            // cv::imshow(" ",l_sign);
            // cv::waitKey();

        }

        // my::drawRenctangles(*l_imgIt,l_StopSign,l_ParkingSign,l_FalseSign,l_UnicSign);
        std::string l_str=std::to_string(iImg);
        cv::imwrite("img"+l_str+".png",*l_imgIt);
        cv::imwrite("Filt"+l_str+".png",l_filterRes.Mask);
        cv::imshow(" ",*l_imgIt);
        cv::waitKey();

        l_StopSign.clear();
        l_ParkingSign.clear();
        l_FalseSign.clear();
    }
}


void my::drawRenctangles(   cv::Mat&                                        f_img
                            ,const std::vector< my::TrafficSignProcessing::TrafficSignSegment>& f_signs){
                          // ,std::vector<my::TrafficSignProcessing::TrafficSignSegment>&       f_trafficsigns){

    std::vector<my::TrafficSignProcessing::TrafficSignSegment>::const_iterator it;

    for (it = f_signs.begin();it != f_signs.end();++it){
          cv::Point l_p1(it->left,it->top);
          cv::Point l_p2(it->left+it->width,it->top+it->height);
          cv::Scalar color;
          std::string text;
          switch(it->signType){
                  case my::TrafficSignProcessing::TrafficSignTypes_t::StopSign:
                      text="Stop";
                      color=cv::Scalar(255,0,0);
                      break;
                  case my::TrafficSignProcessing::TrafficSignTypes_t::ParkingSign:
                      text="Parking";
                      color= cv::Scalar(0,0,255);
                      break;
                  case my::TrafficSignProcessing::TrafficSignTypes_t::Limit30:
                      text="Limit30";
                      color= cv::Scalar(0,255,0);
                      break;
                  case my::TrafficSignProcessing::TrafficSignTypes_t::NoLimit30:
                      text="NoLimit30";
                      color= cv::Scalar(255,255,0);
                      break;
                  case my::TrafficSignProcessing::TrafficSignTypes_t::Limit50:
                      text="Limit50";
                      color= cv::Scalar(255,0,255);
                      break;
                  case my::TrafficSignProcessing::TrafficSignTypes_t::NoLimit50:
                      text="NoLimit50";
                      color= cv::Scalar(0,255,255);
                      break;
                  case my::TrafficSignProcessing::TrafficSignTypes_t::NoStop:
                      text="NoStop";
                      color= cv::Scalar(255,255,255);
                      break;
                  case my::TrafficSignProcessing::TrafficSignTypes_t::NoParking:
                      text="NoParking";
                      color= cv::Scalar(125,125,0);
                      break;
                  case my::TrafficSignProcessing::TrafficSignTypes_t::NoVechile:
                      text="NoVechile";
                      color= cv::Scalar(125,0,125);
                      break;
                  default:
                      color= cv::Scalar(0,0,0);
                      text="";
                      break;
          }
          cv::rectangle(f_img,l_p1,l_p2,color,1);
          cv::putText(f_img,text,cv::Point(it->left+4,it->top-4),cv::FONT_HERSHEY_PLAIN,1,color);
    }

    // std::vector<my::ImageSegment::Segment_t>::iterator l_segmentIt;
    // for(l_segmentIt=f_ParkingSign.begin(); l_segmentIt<f_ParkingSign.end(); ++l_segmentIt){
    //     cv::Point l_p1(l_segmentIt->left,l_segmentIt->top);
    //     cv::Point l_p2(l_segmentIt->left+l_segmentIt->width,l_segmentIt->top+l_segmentIt->height);
    //     cv::rectangle(f_img,l_p1,l_p2,cv::Scalar(0,0,255),3);
    //     cv::putText(f_img,"Parking",cv::Point(l_segmentIt->left+4,l_segmentIt->top-4),cv::FONT_HERSHEY_PLAIN,1,cv::Scalar(0,0,255));
    // }
    //
    // for(l_segmentIt=f_StopSign.begin(); l_segmentIt<f_StopSign.end(); ++l_segmentIt){
    //     cv::Point l_p1(l_segmentIt->left,l_segmentIt->top);
    //     cv::Point l_p2(l_segmentIt->left+l_segmentIt->width,l_segmentIt->top+l_segmentIt->height);
    //     cv::rectangle(f_img,l_p1,l_p2,cv::Scalar(255,0,0),3);
    //     cv::putText(f_img,"Stop",cv::Point(l_segmentIt->left+4,l_segmentIt->top-4),cv::FONT_HERSHEY_PLAIN,1,cv::Scalar(255,0,0));
    // }
    //
    // for(l_segmentIt=f_FalseSign.begin(); l_segmentIt<f_FalseSign.end(); ++l_segmentIt){
    //     cv::Point l_p1(l_segmentIt->left,l_segmentIt->top);
    //     cv::Point l_p2(l_segmentIt->left+l_segmentIt->width,l_segmentIt->top+l_segmentIt->height);
    //     cv::rectangle(f_img,l_p1,l_p2,cv::Scalar(0,0,0),3);
    //     // cv::putText(f_img,"Stop",cv::Point(l_segmentIt->left+4,l_segmentIt->top-4),cv::FONT_HERSHEY_PLAIN,1,cv::Scalar(255,0,0));
    // }
    //
    // for(l_segmentIt=f_UnicSign.begin(); l_segmentIt<f_UnicSign.end(); ++l_segmentIt){
    //     cv::Point l_p1(l_segmentIt->left,l_segmentIt->top);
    //     cv::Point l_p2(l_segmentIt->left+l_segmentIt->width,l_segmentIt->top+l_segmentIt->height);
    //     cv::rectangle(f_img,l_p1,l_p2,cv::Scalar(0,255,255),3);
    //     // cv::putText(f_img,"Stop",cv::Point(l_segmentIt->left+4,l_segmentIt->top-4),cv::FONT_HERSHEY_PLAIN,1,cv::Scalar(255,0,0));
    // }
}



void my::testVideo(     const std::string&              f_folderPath
                        ,const std::string&             f_svmFileName
                        ,const std::string&             f_settingsFileName){
    DIR *dir  =opendir(f_folderPath.c_str());
    std::vector<std::string> l_VideoFiles;
    // --------------------------------------------------------------------------------------------------------------------------
    if(dir){
        struct dirent *ent;
        while((ent = readdir(dir)) !=NULL){
            std::string l_fileName = ent->d_name;

            std::string l_fileExtension = l_fileName.substr(l_fileName.find_last_of(".") + 1);
            if(l_fileExtension == "h264" || l_fileExtension == "mp4" ){
                // std::cout << ent->d_name << std::endl;
                std::string l_fullpath = f_folderPath;
                l_fullpath.append(l_fileName);
                l_VideoFiles.push_back(l_fullpath);
            }
        }
    }
    // --------------------------------------------------------------------------------------------------------------------------
    if(l_VideoFiles.size()==0){
        std::cout<<"Cannot find any video file.";
        return;
    }
    my::Settings l_settings = my::Settings::readFile(f_settingsFileName);


    std::vector<my::TrafficSignProcessing::TrafficSignSegment> l_signs;
    my::TrafficSignProcessing l_trafficProcessing(l_settings);
    std::vector<std::string>::iterator it;

    for (it=l_VideoFiles.begin(); it<l_VideoFiles.end(); ++it){
        std::cout<<"File:"<<(*it)<<std::endl;
        cv::VideoCapture l_videoCapture((*it));
        if(!l_videoCapture.isOpened()){
            std::cout<<"Cannot open the file!"<<std::endl;
            continue;
        }

        cv::Mat l_frame;
        while(true){
            l_videoCapture>>l_frame;
            if(l_frame.empty()) break;
            l_trafficProcessing.processFrameAndDraw(l_frame,l_signs);
            l_signs.clear();


            cv::imshow("Video",l_frame);
            char c=(char)cv::waitKey(25);
            if(c==27)
            break;
        }

    }
}
