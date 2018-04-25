

template<uint N>
void my::readAndSaveFeatures(   std::array<std::string,N>&                  f_inputFolders
                            ,std::array<std::string,N>&                 f_outputFileName
                            ,my::ColorFilter&                           f_colorFilter
                            ,my::ImageSegment&                          f_segmenting
                            ,my::HogCalculation&                        f_hogCalc){

    for(uint i=0; i<N ;++i){
        std::string l_inputFolder = f_inputFolders[i];
        std::string l_outputFileName = f_outputFileName[i];

        std::vector<cv::Mat> l_imgContainer;
        my::readFromFolderImages(l_inputFolder,l_imgContainer);

        std::vector<cv::Mat>::const_iterator itImg;
        std::cout << "Nr img.:" << l_imgContainer.size() << std::endl;
        std::vector<std::vector<float>> l_collectionDescriptors;
        long imgIndex=0;
        for (itImg = l_imgContainer.begin(); itImg!=l_imgContainer.end() ;++itImg){
            cv::Mat l_imgresized=(*itImg);
            std::cout << "Img.src size:" << (*itImg).size() << std::endl;
            // cv::resize((*itImg), l_imgresized, cv::Size(100, 100));
            my::ColorFilter::ColorFilter_Data l_FilteredImg = f_colorFilter.apply(l_imgresized);
            std::vector<my::ImageSegment::Segment_t> l_segments;
            l_segments.clear();
            f_segmenting.apply(l_FilteredImg.blueMask,l_FilteredImg.redMask,l_segments);
            std::vector<my::ImageSegment::Segment_t>::iterator it;

            for (it = l_segments.begin(); it!=l_segments.end();++it){
                
                cv::Range l_x(it->left,it->left+it->width);
                cv::Range l_y(it->top,it->top+it->height);
                cv::Mat l_sign=l_imgresized(l_y,l_x);
                // cv::resize(,l_sign,cv::Size(64,64));
                
                std::vector<float> l_descriptors;
                
                // cv::imshow(" ",l_sign);
                // cv::waitKey();
                std::ostringstream s;
                s<<"/home/nandi/Workspaces/git/resource/TFSign/Video/stopSignCrop/"<<"pics"<<imgIndex<<".jpg";
                std::string picsName(s.str());
                cv::imwrite(picsName.c_str(),l_sign);
                ++imgIndex;

                f_hogCalc.apply(l_sign,l_descriptors);
                l_collectionDescriptors.push_back(l_descriptors);
            }
        }
        my::DescriptionSaver l_saver(l_outputFileName);
        l_saver.saveDescription(l_collectionDescriptors);
    }
}



template<uint N>
void my::readAndTrainSVM(           const std::array<std::string,N>&          f_dataFileNames
                                    ,const std::string&                 f_svmFileXML){
    
    std::array<cv::Mat,N> l_dataCol;
    unsigned long l_nrSample = 0;
    unsigned long l_featuresLength = 0;
    for(uint i=0;i<N;++i){
        cv::Mat l_temp;
        std::string l_dataFile=f_dataFileNames[i];
        my::DescriptionSaver::loadDescription(l_dataFile,l_temp);
        l_dataCol[i] = l_temp;
        l_nrSample += l_temp.rows;
    }

    std::cout<<"Nr. samples:"<<l_nrSample<<std::endl;
    l_featuresLength = l_dataCol[0].cols;

    cv::Mat l_samples = cv::Mat(l_nrSample,l_featuresLength,CV_32F);
    cv::Mat l_responses = cv::Mat(l_nrSample,1,CV_32S);

    unsigned long l_samplesIndex=0;
    for (uint i=0; i<N; ++i){
        
        cv::Mat l_sample = l_dataCol[i];
        std::cout<<"Index"<<l_samplesIndex<<" "<<l_sample.rows<<std::endl;
        cv::Mat l_response = cv::Mat::ones(l_sample.rows,1,CV_32S)*i;
        cv::Mat l_tempSample = l_samples(cv::Range(l_samplesIndex,l_samplesIndex+l_sample.rows),cv::Range::all());
        cv::Mat l_tempResponse = l_responses(cv::Range(l_samplesIndex,l_samplesIndex+l_sample.rows),cv::Range::all());
        l_sample.copyTo(l_tempSample);
        l_response.copyTo(l_tempResponse);
        l_samplesIndex += l_sample.rows;
        
    }

    cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
    svm->setType(cv::ml::SVM::C_SVC);
    svm->setKernel(cv::ml::SVM::LINEAR);
    svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 100, 1e-6));


    std::cout<<l_samples.size()<<" "<<l_responses.size();
    cv::Ptr<cv::ml::TrainData>  l_trainData=cv::ml::TrainData::create(l_samples,cv::ml::ROW_SAMPLE,l_responses);
    svm->train(l_trainData);
    svm->save(f_svmFileXML);
}


template<uint N>
void my::testSVM(   const std::array<std::string,N>&  f_testFileNames
                    ,const std::string&             f_svmFileXML){
    
    cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
    svm = svm->load(f_svmFileXML);
    
    for(uint l_dataIndex=0; l_dataIndex<N; ++l_dataIndex){
        cv::Mat l_testData;
        std::cout<<"Data sample index"<<l_dataIndex<<std::endl;
        my::DescriptionSaver::loadDescription(f_testFileNames[l_dataIndex],l_testData);
        for(uint i=0; i<l_testData.rows; i++){
            cv::Mat l_testSample = l_testData.row(i);
            unsigned int l_res =static_cast<unsigned int>(svm->predict(l_testSample));
            std::cout<<"res:"<<l_res<<std::endl;
        }   
    }
}

