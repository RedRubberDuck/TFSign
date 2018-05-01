#define PY_ARRAY_UNIQUE_SYMBOL pbcvt_ARRAY_API

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <pyboostcvconverter/pyboostcvconverter.hpp>

#include <imgprocessing/colorfilter.hpp>
#include <imgprocessing/segmentation.hpp>
#include <imgprocessing/hogCalc.hpp>
#include <settings/settings.hpp>
#include <file/fileHandler.hpp>
#include <ml/utilities.hpp>

namespace my {

    using namespace boost::python;
    // typedef std::vector<std::string> Words;

    cv::Mat dot2(cv::Mat leftMat, cv::Mat rightMat) {
        auto c1 = leftMat.cols, r2 = rightMat.rows;
        if (c1 != r2) {
            PyErr_SetString(PyExc_TypeError,
                            "Incompatible sizes for matrix multiplication.");
            throw_error_already_set();
        }
        cv::Mat result = leftMat * rightMat;
        return result;
    }


#if (PY_VERSION_HEX >= 0x03000000)

        static void *init_ar() {
#else
        static void init_ar(){
#endif
        Py_Initialize();

        import_array();
        return NUMPY_IMPORT_ARRAY_RETVAL;
    }

    BOOST_PYTHON_MODULE (mytrafficsigndetect) {
        //using namespace XM;
        init_ar();

        //initialize converters
        to_python_converter<cv::Mat,
                pbcvt::matToNDArrayBoostConverter>();
        pbcvt::matFromNDArrayBoostConverter();

        def("dot2", dot2);
        
        
        class_<my::Settings>("TrafficSignSettings");
        def("InitSettings",my::Settings::readFile,args("fileName"));

        class_<my::ColorFilter::ColorFilter_Data>("ColorFilter_Data")
                                        .def("getblueMask",&my::ColorFilter::ColorFilter_Data::getBlueMask)
                                        .def("getredMask",&my::ColorFilter::ColorFilter_Data::getRedMask)
                                        .def("getMask",&my::ColorFilter::ColorFilter_Data::getMask);

        class_<my::ColorFilter>("ColorFilter",init<const my::Settings&>())
                                .def("apply",&my::ColorFilter::apply);

        enum_<my::ImageSegment::ColorTypes_t>("MaskColorTypes")
                                            .value("BLUE",my::ImageSegment::ColorTypes_t::BLUE)
                                            .value("RED",my::ImageSegment::ColorTypes_t::RED)
                                            .value("NONE",my::ImageSegment::ColorTypes_t::None);
        
        class_<my::ImageSegment::Segment_t>("ImageSegment")
                                    .def_readwrite("left",&my::ImageSegment::Segment_t::left)
                                    .def_readwrite("top",&my::ImageSegment::Segment_t::top)
                                    .def_readwrite("width",&my::ImageSegment::Segment_t::width)
                                    .def_readwrite("height",&my::ImageSegment::Segment_t::height)
                                    .def_readwrite("color",&my::ImageSegment::Segment_t::color);
        
        class_<std::vector<my::ImageSegment::Segment_t> >("SegmentVector")
                                    .def(vector_indexing_suite<std::vector<my::ImageSegment::Segment_t>>() );


        class_<my::ImageSegment>("ImageSegmenter",init<const my::Settings&>())
                                .def("apply",&my::ImageSegment::apply);

        class_<std::vector<float>>("HogFeatures")
                                    .def(vector_indexing_suite<std::vector<float>>() );

        class_<my::HogCalculation>("HogCalculation",init<const my::Settings&>())
                                .def("apply",&my::HogCalculation::apply);

        enum_<my::TrafficSignProcessing::TrafficSignTypes_t>("TrafficSignTypes")
                                .value("None",my::TrafficSignProcessing::TrafficSignTypes_t::None)
                                .value("StopSign",my::TrafficSignProcessing::TrafficSignTypes_t::StopSign)
                                .value("ParkingSign",my::TrafficSignProcessing::TrafficSignTypes_t::ParkingSign);

        class_<my::TrafficSignProcessing::TrafficSignSegment>("TrafficSignSegment")
                                .def_readwrite("left",&my::TrafficSignProcessing::TrafficSignSegment::left)
                                .def_readwrite("top",&my::TrafficSignProcessing::TrafficSignSegment::top)
                                .def_readwrite("width",&my::TrafficSignProcessing::TrafficSignSegment::width)
                                .def_readwrite("height",&my::TrafficSignProcessing::TrafficSignSegment::height)
                                .def_readwrite("signType",&my::TrafficSignProcessing::TrafficSignSegment::signType);
                                
        class_<std::vector<my::TrafficSignProcessing::TrafficSignSegment> >("TrafficSignsVector")
                                    .def(vector_indexing_suite<std::vector<my::TrafficSignProcessing::TrafficSignSegment>>() );

        class_<my::TrafficSignProcessing>("TrafficSignProcessing",init<const my::Settings&>())
                                .def(init<const my::ColorFilter&,const my::ImageSegment&,const my::HogCalculation&,const std::string&>())
                                .def("processFrameAndDraw",&my::TrafficSignProcessing::processFrameAndDraw)
                                .def("processFrame",&my::TrafficSignProcessing::processFrame);
        

    }

} //end namespace pbcvt
