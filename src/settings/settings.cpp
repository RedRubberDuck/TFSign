#include <settings/settings.hpp>


my::Settings::Settings(){
    
}

my::Settings::Settings(
                const std::string&                      f_image    
                ,const std::string&                     f_stopSignTrainFolder    
                ,const std::string&                     f_parkingSignTrainFolder
                ,const std::string&                     f_negativTrainFolder
                
                ,const std::string&                     f_stopSignTestFolder    
                ,const std::string&                     f_parkingSignTestFolder
                ,const std::string&                     f_negativTestFolder

                ,const ::cv::Vec3b&                     f_blueInferior
                ,const ::cv::Vec3b&                     f_blueSuperior
                ,const ::cv::Vec3b&                     f_red1Inferior
                ,const ::cv::Vec3b&                     f_red1Superior
                ,const ::cv::Vec3b&                     f_red2Inferior
                ,const ::cv::Vec3b&                     f_red2Superior
                ,const uint&                            f_dilationSize
                ,const bool&                            f_dilationApply
                ,const uint&                            f_erosionSize
                ,const bool&                            f_erosionApply
                ,const my::ImgSegGradType_t&            f_ImgSegment_gradType
                ,const uint&                            f_ImgSegment_kernelSize
                ,const double&                          f_inferiorSquareRate
                ,const double&                          f_superiorSquareRate
                ,const cv::Size&                        f_imgSize
                ,const cv::Size&                        f_cellSize
                ,const cv::Size&                        f_blockSize
                ,const uint&                            f_nrBins
                ,const std::string&                     f_svmXmlFile)
    :m_image(f_image)
    ,m_stopSignTrainFolder(f_stopSignTrainFolder)
    ,m_parkingSignTrainFolder(f_parkingSignTrainFolder)
    ,m_negativTrainFolder(f_negativTrainFolder)
    ,m_stopSignTestFolder(f_stopSignTestFolder)
    ,m_parkingSignTestFolder(f_parkingSignTestFolder)
    ,m_negativTestFolder(f_negativTestFolder)
    ,m_blueInferior(f_blueInferior)
    ,m_blueSuperior(f_blueSuperior)
    ,m_red1Inferior(f_red1Inferior)
    ,m_red1Superior(f_red1Superior)
    ,m_red2Inferior(f_red2Inferior)
    ,m_red2Superior(f_red2Superior)
    ,m_dilationSize(f_dilationSize)
    ,m_dilationApply(f_dilationApply)
    ,m_erosionSize(f_erosionSize)
    ,m_erosionsApply(f_erosionApply)
    ,m_ImgSegment_gradType(f_ImgSegment_gradType)
    ,m_ImgSegment_kernelSize(f_ImgSegment_kernelSize)
    ,m_InferiorSquareRate(f_inferiorSquareRate)
    ,m_SuperiorSquareRate(f_superiorSquareRate)
    ,m_imgSize(f_imgSize)
    ,m_cellSize(f_cellSize)
    ,m_blockSize(f_blockSize)
    ,m_nrBins(f_nrBins)
    ,m_svmXmlFile(f_svmXmlFile)
{}




my::Settings my::Settings::readFile(const std::string& fileName){
    std::ifstream ifs(fileName.c_str());
    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document doc;
    doc.ParseStream(isw);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    rapidjson::Value& l_colorFilter = doc["ColorFilter"];
    
    cv::Vec3b l_blueInfCv,l_blueSupCv,l_red1Inf,l_red1Sup,l_red2Inf,l_red2Sup;
    my::Settings::setLimit(l_blueInfCv,l_colorFilter["blueInferiorLimit"]);    
    my::Settings::setLimit(l_blueSupCv,l_colorFilter["blueSuperiorLimit"]);

    my::Settings::setLimit(l_red1Inf,l_colorFilter["red1InferiorLimit"]);
    my::Settings::setLimit(l_red1Sup,l_colorFilter["red1SuperiorLimit"]);

    my::Settings::setLimit(l_red2Inf,l_colorFilter["red2InferiorLimit"]);
    my::Settings::setLimit(l_red2Sup,l_colorFilter["red2SuperiorLimit"]);

    std::string l_img = doc["inputImage"].GetString();
    // Train folders
    std::string l_stopTrainFolder = doc["stopSignTrainFolder"].GetString();
    std::string l_negativTrainFolder = doc["negativTrainFolder"].GetString();
    std::string l_parkingTrainFolder = doc["parkingSignTrainFolder"].GetString();
    // Test folders
    std::string l_stopTestFolder = doc["stopSignTestFolder"].GetString();
    std::string l_negativTestFolder = doc["negativTestFolder"].GetString();
    std::string l_parkingTestFolder = doc["parkingSignTestFolder"].GetString();
    
    rapidjson::Value& l_dilationJson = doc["dilation"];
    uint l_dilationSize = l_dilationJson["size"].GetInt();
    bool l_dilationApply = l_dilationJson["apply"].GetBool();

    rapidjson::Value& l_erosionJson = doc["erosion"];
    uint l_erosionSize = l_erosionJson["size"].GetInt();
    bool l_erosionApply = l_erosionJson["apply"].GetBool();
    
    rapidjson::Value&  l_imgSegmentJson = doc["ImageSegment"];
    uint l_ImgSeg_gradType_i = l_imgSegmentJson["GradiantType"].GetInt();
    uint l_ImgSeg_kernelSize = l_imgSegmentJson["kernelSize"].GetInt();
    double l_InferiorSquareRate = l_imgSegmentJson["InferiorSquareRate"].GetDouble();
    double l_SuperiorSquareRate = l_imgSegmentJson["SuperiorSquareRate"].GetDouble();

    my::ImgSegGradType_t l_ImgSeg_gradType;
    switch(l_ImgSeg_gradType_i){
        case 0:
            l_ImgSeg_gradType = my::ImgSegGradType_t::Default;
            break;
        case 1:
            l_ImgSeg_gradType = my::ImgSegGradType_t::Laplacian;
            break;
        case 2:
            l_ImgSeg_gradType = my::ImgSegGradType_t::Morphology;
            break;
        default:
            l_ImgSeg_gradType = my::ImgSegGradType_t::Default;
    }


    // ------------------------------------------------------------------
    // Hog parameters identify
    rapidjson::Value&  l_hogPrameters = doc["hogParams"];
    cv::Size l_imgSize(l_hogPrameters["imageSize"][0].GetInt(),l_hogPrameters["imageSize"][1].GetInt());
    cv::Size l_cellSize(l_hogPrameters["cellSize"][0].GetInt(),l_hogPrameters["cellSize"][1].GetInt());
    cv::Size l_blockSize(l_hogPrameters["blockSize"][0].GetInt()*l_cellSize.width,l_hogPrameters["blockSize"][1].GetInt()*l_cellSize.height);
    uint l_nrBins = l_hogPrameters["nrBins"].GetInt();

    // ------------------------------------------------------------------
    // SVM 
    std::string l_svmFile = doc["svmXml"].GetString();



    my::Settings l_settings(    l_img
                                ,l_stopTrainFolder
                                ,l_parkingTrainFolder
                                ,l_negativTrainFolder
                                ,l_stopTestFolder
                                ,l_parkingTestFolder
                                ,l_negativTestFolder
                                ,l_blueInfCv
                                ,l_blueSupCv
                                ,l_red1Inf
                                ,l_red1Sup
                                ,l_red2Inf
                                ,l_red2Sup
                                ,l_dilationSize
                                ,l_dilationApply
                                ,l_erosionSize
                                ,l_erosionApply
                                ,l_ImgSeg_gradType
                                ,l_ImgSeg_kernelSize
                                ,l_InferiorSquareRate
                                ,l_SuperiorSquareRate
                                ,l_imgSize
                                ,l_cellSize
                                ,l_blockSize
                                ,l_nrBins
                                ,l_svmFile);
    return l_settings;
}


void my::Settings::setLimit(cv::Vec3b& f_colorLimitCv, const rapidjson::Value& f_colorLimitJson){
    for (uint i = 0; i < f_colorLimitJson.Size(); i++){
        f_colorLimitCv[i] = f_colorLimitJson[i].GetInt();
    }

}


cv::Vec3b my::Settings::getblueInferior() const{
    return m_blueInferior;
}

cv::Vec3b my::Settings::getblueSuperior() const{
    return m_blueSuperior;
}


cv::Vec3b my::Settings::getred1Inferior() const{
    return m_red1Inferior;
}

cv::Vec3b my::Settings::getred1Superior() const{
    return m_red1Superior;
}

cv::Vec3b my::Settings::getred2Inferior() const{
    return m_red2Inferior;
}

cv::Vec3b my::Settings::getred2Superior() const{
    return m_red2Superior;
}

uint my::Settings::getdilationSize() const{
    return m_dilationSize;
}
bool my::Settings::getdilationApply() const{
    return m_dilationApply;
}

uint my::Settings::geterosionSize() const{
    return m_erosionSize;
}
bool my::Settings::geterosionApply() const{
    return m_erosionsApply;
}



my::ImgSegGradType_t my::Settings::getImgSegmentGradType() const{
    return m_ImgSegment_gradType;
}

uint my::Settings::getImgSegmentKernelSize() const{
    return m_ImgSegment_kernelSize;
}

double my::Settings::getInferiorSquareRate() const{
    return m_InferiorSquareRate;
}

double my::Settings::getSuperiorSquareRate() const{
    return m_SuperiorSquareRate;
}

std::string my::Settings::getImageName() const{
    return m_image;
}

std::string my::Settings::getStopTrainFolder() const{
    return m_stopSignTrainFolder;
}

std::string my::Settings::getParkingTrainFolder() const{
    return m_parkingSignTrainFolder;
}

std::string my::Settings::getNegativTrainFolder() const{
    return m_negativTrainFolder;
}


std::string my::Settings::getStopTestFolder() const{ return m_stopSignTestFolder;}
std::string my::Settings::getParkingTestFolder() const{ return m_parkingSignTestFolder;}
std::string my::Settings::getNegativTestFolder() const{ return m_negativTestFolder;}


cv::Size my::Settings::getHogImageSize() const{
    return m_imgSize;
}

cv::Size my::Settings::getHogCellSize() const{
    return m_cellSize;
}


cv::Size my::Settings::getHogBlockSize() const{
    return m_blockSize;
}

uint my::Settings::getHogNrBins() const{
    return m_nrBins;
}


std::string my::Settings::getSvmFile() const {return m_svmXmlFile;}