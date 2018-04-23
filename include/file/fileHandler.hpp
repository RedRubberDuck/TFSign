#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP

#include <string>
#include <rapidjson/document.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <assert.h> 

#include <opencv2/opencv.hpp>


namespace my{
    class DescriptionSaver{
        public:
            DescriptionSaver(const std::string&);

            void saveDescription(const std::vector<std::vector<float> >&);
            
            static void loadDescription(    const std::string&
                                            ,std::vector<std::vector<float>>&);
            static void loadDescription(    const std::string&
                                            ,cv::Mat&);
        private:
            std::string m_fileName;

    };

};


#endif