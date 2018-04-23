#include <file/fileHandler.hpp>

my::DescriptionSaver::DescriptionSaver(const std::string& f_fileName)
    :m_fileName(f_fileName)
{
}



void my::DescriptionSaver::saveDescription(const std::vector<std::vector<float> >&  f_collectionDescriptors){
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    rapidjson::Value l_descriptionSize_v;
    l_descriptionSize_v.SetUint(f_collectionDescriptors[0].size());
    rapidjson::Value l_nrDescription_v;
    l_nrDescription_v.SetUint(f_collectionDescriptors.size());
    doc.AddMember("descriptionSize",l_descriptionSize_v,allocator);
    doc.AddMember("nrDescription",l_nrDescription_v,allocator);
    
    
    std::ofstream ofsJson((m_fileName+".json").c_str());
    rapidjson::OStreamWrapper osw(ofsJson);
    rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
    doc.Accept(writer);
    ofsJson.close();

    std::FILE* fData;
    fData = fopen((m_fileName+".out").c_str(),"wb");
    std::vector<std::vector<float>>::const_iterator array_it;
    
    for (array_it=f_collectionDescriptors.begin(); array_it < f_collectionDescriptors.end();++array_it){
        std::vector<float>::const_iterator value_it;
        for(value_it=array_it->begin(); value_it < array_it->end();++value_it){
            float s=*value_it;
            std::fwrite(&s,sizeof(s),1,fData);
            
        }
    }
    // std::cout<<" Sizeof float"<<sizeof(float)<<std::endl;
    fclose(fData);
}

void my::DescriptionSaver::loadDescription( const std::string&                 l_fileName
                                            ,std::vector<std::vector<float>>& l_collectionDescriptions){
    
    
    std::ifstream ifs((l_fileName+".json").c_str());
    std::FILE* fData;
    fData = fopen((l_fileName+".out").c_str(),"rb");


    assert(fData!=NULL);
    assert(ifs);

    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document doc;
    doc.ParseStream(isw);

    uint l_descriptionSize = doc["descriptionSize"].GetUint();
    uint l_nrDescription = doc["nrDescription"].GetUint();
    
    ifs.close();
    
    

    for (uint i=0; i<l_nrDescription; ++i){
        std::vector<float> l_vec;
        for (uint j=0; j<l_descriptionSize ;++j){
            float l_value=0;
            // std::cout<<l_value<<" ";
            fread(&l_value,sizeof(float),1,fData);
            l_vec.push_back(l_value);
            
        }
        l_collectionDescriptions.push_back(l_vec);
    }
    std::cout<<" "<<l_descriptionSize<<"-------------"<<l_nrDescription<<std::endl;    
    fclose(fData);

}


void my::DescriptionSaver::loadDescription( const std::string&             l_fileName
                                            ,cv::Mat&                       l_data)
{
    std::ifstream ifs((l_fileName+".json").c_str());
    std::FILE* fData;
    fData = fopen((l_fileName+".out").c_str(),"rb");


    assert(fData!=NULL);
    assert(ifs);

    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document doc;
    doc.ParseStream(isw);

    uint l_descriptionSize = doc["descriptionSize"].GetUint();
    uint l_nrDescription = doc["nrDescription"].GetUint();
    
    ifs.close();
    
    l_data = cv::Mat(l_nrDescription,l_descriptionSize,CV_32F);

    for (uint i=0; i<l_nrDescription; ++i){
        for (uint j=0; j<l_descriptionSize ;++j){
            float l_value=0;
            // std::cout<<l_value<<" ";
            fread(&l_value,sizeof(float),1,fData);
            l_data.at<float>(i,j) = l_value;            
        }
    }
    std::cout<<" "<<l_descriptionSize<<"-------------"<<l_nrDescription<<std::endl;    
    fclose(fData);

}