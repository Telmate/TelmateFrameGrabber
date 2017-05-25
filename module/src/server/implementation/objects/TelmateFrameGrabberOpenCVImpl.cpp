/* Autogenerated with kurento-module-creator */

#include "TelmateFrameGrabberOpenCVImpl.hpp"
#include <KurentoException.hpp>

namespace pt = boost::posix_time;

namespace kurento {



    TelmateFrameGrabberOpenCVImpl::TelmateFrameGrabberOpenCVImpl() {

        this->thrLoop = true;
        this->frameQueue = new boost::lockfree::queue<VideoFrame *>(0);

        /*boost::asio::io_service::work work(ioService);

        tp.create_thread(
                boost::bind(&boost::asio::io_service::run, &ioService)
        );

        tp.create_thread(
                boost::bind(&boost::asio::io_service::run, &ioService)
        );

        ioService.post(boost::bind(this->queueHandler)); // post to the pool
        */
        this->thr = new boost::thread(boost::bind(&TelmateFrameGrabberOpenCVImpl::queueHandler, this));
        //thr->join();
        GST_ERROR("TelmateFrameGrabberOpenCVImpl::TelmateFrameGrabberOpenCVImpl()");
    }

/*
 * This function will be called with each new frame. mat variable
 * contains the current frame. You should insert your image processing code
 * here. Any changes in mat, will be sent through the Media Pipeline.
 */
    void TelmateFrameGrabberOpenCVImpl::process(cv::Mat &mat) {

        /*boost::posix_time::ptime boost::posix_time::time_t_epoch(date(1970,1,1));
        boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
        boost::posix_time::time_duration diff = now - time_t_epoch;
        */

        VideoFrame *ptrVf = new VideoFrame();
        //cv::Mat ptrMat; //= new cv::Mat();
        ptrVf->mat = mat.clone();

        //std::string *ptrTs = new std::string();
        //*ptrTs = this->getCurrentTimestamp();
        //*ptrVf->ts = *ptrTs;
        this->frameQueue->push(ptrVf);

//  GST_LOG_OBJECT (self, "Stats: %" GST_PTR_FORMAT, stats);
//GST_LOG ("No %s in config file", nodeName);
//        GST_ERROR ("Processing Frame %i",  this->frameQueue->size());
        // FIXME: Implement this
        //throw KurentoException (NOT_IMPLEMENTED, "TelmateFrameGrabberOpenCVImpl::process: Not implemented");

    }
void TelmateFrameGrabberOpenCVImpl::queueHandler()
{
    VideoFrame *vf;
    cv::Mat image;
    std::vector<int> params;
    params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    params.push_back(9);
    //cv::IplImage srcImage;

    while(this->thrLoop) {

        boost::this_thread::sleep( boost::posix_time::seconds(1) );

        if (!this->frameQueue->empty()) {
            /* Handle the frame */

            this->frameQueue->pop(vf);
//            srcImage = vf->mat.clone();
            //dstImage = *vf->mat;
            cv::cvtColor(vf->mat,image,CV_BGR2RGB);
            //std::stringstream a;
            //a << "/tmp/" << getCurrentTimestamp();
            //srcImage = *vf->mat;
            cv::imwrite("/tmp/asd.png",vf->mat,params);
            //imdecode(*vf->mat, CV_LOAD_IMAGE_ANYDEPTH, srcImage);
            //std::cout << this->getCurrentTimestamp();

            //delete vf->mat;
            //delete vf->ts;
            //delete vf->epName;
            delete vf;

            //GST_ERROR("POPed.");

        }

    }
}

std::string TelmateFrameGrabberOpenCVImpl::getCurrentTimestamp()
{
    std::stringstream ss;

    boost::posix_time::ptime time = boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration duration( time.time_of_day() );

    ss << duration.total_milliseconds() << std::endl;
    return ss.str();

}


} /* kurento */