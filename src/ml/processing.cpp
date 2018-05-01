#include <ml/processing.hpp>


my::TrafficSignProcessing::TrafficSignProcessing(   const my::ColorFilter&          f_colorFilter
                                                    ,const my::ImageSegment&        f_imgSegmeting
                                                    ,const my::HogCalculation&      f_hogCalc
                                                    ,const std::string&             f_SvmFileName)
    :m_colorFilter(f_colorFilter)
    ,m_imgSegmenting(f_imgSegmeting)
    ,m_hogCalc(f_hogCalc)
    ,m_svm(cv::ml::SVM::create())
{
    m_svm = m_svm->load(f_SvmFileName);
}

my::TrafficSignProcessing::TrafficSignProcessing( const my::Settings&               f_settings)
    :m_colorFilter(f_settings)
    ,m_imgSegmenting(f_settings)
    ,m_hogCalc(f_settings)
    ,m_svm(cv::ml::SVM::create())
{
    m_svm = m_svm->load(f_settings.getSvmFile());
}

void my::TrafficSignProcessing::processFrameAndDraw(cv::Mat f_frame){
    std::vector<my::ImageSegment::Segment_t> l_StopSign;
    std::vector<my::ImageSegment::Segment_t> l_ParkingSign;
    std::vector<my::ImageSegment::Segment_t> l_FalseSign;
    my::ColorFilter::ColorFilter_Data l_filterRes = m_colorFilter.apply(f_frame);
    // --------------------------------------------------------------------------------------
    std::vector<my::ImageSegment::Segment_t> l_segments;
    m_imgSegmenting.apply(l_filterRes.blueMask,l_filterRes.redMask,l_segments);
    // --------------------------------------------------------------------------------------
    std::vector<my::ImageSegment::Segment_t>::iterator it;
    std::cout << "nr. segment:" << l_segments.size() << std::endl;
    for (it = l_segments.begin(); it!=l_segments.end();++it){
        cv::Range l_x(it->left,it->left+it->width);
        cv::Range l_y(it->top,it->top+it->height);
        cv::Mat l_sign=f_frame(l_y,l_x);

        std::vector<float> l_descriptors;
        m_hogCalc.apply(l_sign,l_descriptors);
        cv::Mat l_data(1, l_descriptors.size(), CV_32FC1, l_descriptors.data());
        unsigned int l_res =static_cast<unsigned int>(m_svm->predict(l_data));
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
    }

    my::drawRenctangles(f_frame,l_StopSign,l_ParkingSign,l_FalseSign);
    // return f_frame;

}


void my::TrafficSignProcessing::processFrame(   cv::Mat                                 f_frame
                                                ,std::vector<TrafficSignSegment>&       f_trafficsigns)
{
    my::ColorFilter::ColorFilter_Data l_filterRes = m_colorFilter.apply(f_frame);
    // --------------------------------------------------------------------------------------
    std::vector<my::ImageSegment::Segment_t> l_segments;
    m_imgSegmenting.apply(l_filterRes.blueMask,l_filterRes.redMask,l_segments);
    // --------------------------------------------------------------------------------------
    std::vector<my::ImageSegment::Segment_t>::iterator it;
    std::cout << "nr. segment:" << l_segments.size() << std::endl;
    for (it = l_segments.begin(); it!=l_segments.end();++it){
        cv::Range l_x(it->left,it->left+it->width);
        cv::Range l_y(it->top,it->top+it->height);
        cv::Mat l_sign=f_frame(l_y,l_x);

        std::vector<float> l_descriptors;
        m_hogCalc.apply(l_sign,l_descriptors);
        cv::Mat l_data(1, l_descriptors.size(), CV_32FC1, l_descriptors.data());
        unsigned int l_res =static_cast<unsigned int>(m_svm->predict(l_data));
        my::TrafficSignProcessing::TrafficSignSegment l_trafficSign;
        l_trafficSign.top = it->top;
        l_trafficSign.left = it->left;
        l_trafficSign.width = it->width;
        l_trafficSign.height = it->height;
        switch(l_res){
                case 0:
                    l_trafficSign.signType=my::TrafficSignProcessing::TrafficSignTypes_t::ParkingSign;
                    f_trafficsigns.push_back(l_trafficSign);
                    break;
                case 1:
                    l_trafficSign.signType=my::TrafficSignProcessing::TrafficSignTypes_t::StopSign;
                    f_trafficsigns.push_back(l_trafficSign);
                    break;
        } 
    }
}