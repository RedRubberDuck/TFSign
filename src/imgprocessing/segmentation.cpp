#include <imgprocessing/segmentation.hpp>


my::ImageSegment::ImageSegment( const ImgSegGradType_t&             f_gradiantType
                                ,const uint&                        f_kernelSize)
    :m_gradiantType(f_gradiantType)
    ,m_kernelSize(f_kernelSize * 2 + 1)
    ,m_kernel(cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(2 * f_kernelSize+ 1, 2 * f_kernelSize + 1),cv::Point(f_kernelSize, f_kernelSize)))
    ,m_InferiorRate(0.8)
    ,m_SuperiorRate(1.2)
{
}


my::ImageSegment::ImageSegment( const my::Settings&     f_settings)
    :m_gradiantType(f_settings.getImgSegmentGradType())
    ,m_kernelSize(f_settings.getImgSegmentKernelSize() * 2 + 1)
    ,m_kernel(cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(2 * f_settings.getImgSegmentKernelSize()+ 1, 2 * f_settings.getImgSegmentKernelSize() + 1),cv::Point(f_settings.getImgSegmentKernelSize(), f_settings.getImgSegmentKernelSize())))
    ,m_InferiorRate(f_settings.getInferiorSquareRate())
    ,m_SuperiorRate(f_settings.getSuperiorSquareRate())
{
}



void my::ImageSegment::apply(       const ::cv::Mat&                                        f_blueMask
                                    ,const ::cv::Mat&                                       f_redMask
                                    ,std::vector<::my::ImageSegment::Segment_t>&            f_segments){
    // std::cout<<"Nr segments:"<<f_segments.size()<<std::endl;
    applyColor(f_blueMask,::my::ImageSegment::ColorTypes_t::BLUE,f_segments);
    applyColor(f_redMask,::my::ImageSegment::ColorTypes_t::RED,f_segments);
    // std::cout << "Verify";
    // std::cout<<"Nr segments:"<<f_segments.size()<<std::endl;
    verify(f_segments);
}

void my::ImageSegment::applyColor(   const ::cv::Mat&                                       f_mask
                                    ,const ::my::ImageSegment::ColorTypes_t&                f_color
                                    ,std::vector<::my::ImageSegment::Segment_t>&            f_segments){
    cv::Mat l_edgeMask;
    switch(m_gradiantType){
        case ImgSegGradType_t::Laplacian:
            cv::Laplacian(f_mask,l_edgeMask,CV_8U, m_kernelSize, 1, 0, cv::BORDER_DEFAULT );
            break;
        case ImgSegGradType_t::Morphology:     
            cv::morphologyEx(f_mask,l_edgeMask,cv::MORPH_GRADIENT,m_kernel);
            break;
        default:
            l_edgeMask = f_mask;

    }
    cv::Mat l_labels, l_centroids,l_stats;
    int nccomps = cv::connectedComponentsWithStats(l_edgeMask,l_labels,l_stats,l_centroids,8,CV_32S);
    
    // my::ImageSegment::showLabels(nccomps,l_labels,l_stats);
    segmentProc(nccomps,l_labels,l_stats,l_centroids,f_color,f_segments);
}


void my::ImageSegment::segmentProc(     const uint&                                     f_nccomps
                                        ,const cv::Mat&                                 f_labels
                                        ,const cv::Mat&                                 f_stats
                                        ,const cv::Mat&                                 f_centroids
                                        ,const ColorTypes_t&                            f_color  
                                        ,std::vector<my::ImageSegment::Segment_t>&      f_segments){
    for (uint i = 1; i < f_nccomps; ++i){
        uint width = f_stats.at<int>(i, cv::CC_STAT_WIDTH);
        uint height = f_stats.at<int>(i, cv::CC_STAT_HEIGHT);
        uint left = f_stats.at<int>(i, cv::CC_STAT_LEFT);
        uint top = f_stats.at<int>(i, cv::CC_STAT_TOP);

        if(f_stats.at<int>(i,cv::CC_STAT_AREA)<20){
            continue;
        }
        double l_rate = (double)width/height;
        uint l_size = width * height;
        
        if ( (m_InferiorRate < l_rate && l_rate < m_SuperiorRate) && (l_size>60*60) ){
            my::ImageSegment::Segment_t l_segment;
            l_segment.color = f_color;
            l_segment.left = left;
            l_segment.top = top;
            l_segment.width = width;
            l_segment.height = height;
            f_segments.push_back(l_segment);
        }
        // else if( 1.95 < l_rate && l_rate>2.05){
        //     my::ImageSegment::Segment_t l_segment;
        //     l_segment.color = f_color;
        //     l_segment.left = left;
        //     l_segment.top = top;
        //     l_segment.width = width/2;
        //     l_segment.height = height;
        //     f_segments.push_back(l_segment);

            
        //     l_segment.left = left + width/2;
            
        //     f_segments.push_back(l_segment);

        // }else if( 0.45 < l_rate && l_rate<0.55){
        //     my::ImageSegment::Segment_t l_segment;
        //     l_segment.color = f_color;
        //     l_segment.left = left;
        //     l_segment.top = top;
        //     l_segment.width = width;
        //     l_segment.height = height/2;
        //     f_segments.push_back(l_segment);

            
        //     l_segment.top = top+height/2;

        //     f_segments.push_back(l_segment);
        // }
    }
}




void my::ImageSegment::verify( std::vector<my::ImageSegment::Segment_t>& l_segments){

    std::vector<my::ImageSegment::Segment_t>::iterator it1, it2;
    if(l_segments.size()<2)
        return;
    for (it1=l_segments.begin() ; it1 < l_segments.end() - 1 ;){
        bool deleted=false;
        for (it2 = it1+1 ; it2 < l_segments.end()  ; ){
            bool l_inX12 = it1->left > it2->left && it1->left < it2->left+it2->width;
            bool l_inY12 = it1->top > it2->top && it1->top < it2->top+it2->height;
            
            bool l_inX21 = it2->left > it1->left && it2->left < it1->left+it1->width;
            bool l_inY21 = it2->top > it1->top && it2->top < it1->top+it1->height;
            if( l_inX21 && l_inY21 ){
                
                // std::cout<<"Inside"<<std::endl;
                // std::cout<<(int)(it2-l_segments.begin())<<" "<<(int)(it1-l_segments.begin())<<std::endl;
                
                l_segments.erase(it2);
            }else if(l_inX12 && l_inY12) {
                // std::cout<<"Inside"<<std::endl;
                // std::cout<<(int)(it1-l_segments.begin())<<" "<<(int)(it2-l_segments.begin())<<std::endl;
                deleted=true;
                break;
            }else{
                ++it2;
            }
        }
        if(!deleted){
            ++it1;
        }else{
            l_segments.erase(it1);
        }
    }
}

void my::ImageSegment::showLabels(  const uint&                  f_nccomps
                                    ,const cv::Mat&              f_labels                
                                    ,const cv::Mat&              f_stats
                                    ){
    // std::cout<<"Nr comps:"<<f_nccomps <<std::endl;
    std::vector<cv::Vec3b> colors(f_nccomps+1);
    colors[0] = cv::Vec3b(0,0,0); // background pixels remain black.
    for(uint i = 1; i <= f_nccomps; i++ ) {
        colors[i] = cv::Vec3b(rand()%256, rand()%256, rand()%256);
        // if( f_stats.at<int>(i-1, cv::CC_STAT_AREA) < 100 )
        //     colors[i] = cv::Vec3b(0,0,0); // small regions are painted with black too.
    }

    cv::Mat img_color = cv::Mat::zeros(f_labels.size(), CV_8UC3);
    for(int y = 0; y < img_color.rows; y++ )
        for(int x = 0; x < img_color.cols; x++ )
        {
            int label = f_labels.at<int>(y, x);
            // CV_Assert(0 <= label && label <= f_nccomps);
            img_color.at<cv::Vec3b>(y, x) = colors[label];
        }
    

    cv::imshow("Label",img_color);
    cv::waitKey();

}


void my::ImageSegment::segmenting(  const cv::Mat&                      f_img
                                    ,const std::vector<Segment_t>&      f_segments){
    std::vector<cv::Mat> l_col;
    std::vector<Segment_t>::const_iterator it;
    for (it = f_segments.begin(); it!= f_segments.end() ; ++it){
        cv::Range xRange(it->left,it->left+it->width);
        cv::Range yRange(it->top,it->top+it->height);
        cv::Mat l_roi=f_img(yRange,xRange);

        cv::imshow("Label",l_roi);
        cv::waitKey();
    }
}