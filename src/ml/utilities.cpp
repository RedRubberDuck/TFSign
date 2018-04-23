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
                            ,const std::string&     f_svmFileXML){
    std::vector<cv::Mat> l_imgContainer;
    readFromFolderImages(f_folderName,l_imgContainer);
    std::vector<cv::Mat>::iterator l_imgIt;
    for(l_imgIt= l_imgContainer.begin(); l_imgIt<l_imgContainer.end() ; ++l_imgIt){
        cv::imshow("Img",*l_imgIt);
        cv::waitKey();
    }
}