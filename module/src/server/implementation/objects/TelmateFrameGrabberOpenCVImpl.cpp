/* Autogenerated with kurento-module-creator */

#include "TelmateFrameGrabberOpenCVImpl.hpp"
#include <gst/gst.h>
#include <KurentoException.hpp>
#include <vector>
#include <string>


#define GST_CAT_DEFAULT kurento_telmate_frame_grabber_opencv_impl
GST_DEBUG_CATEGORY_STATIC(GST_CAT_DEFAULT);
#define GST_DEFAULT_NAME "KurentoTelmateFrameGrabberOpenCVImpl"

namespace kurento {

TelmateFrameGrabberOpenCVImpl::TelmateFrameGrabberOpenCVImpl() {

    GST_DEBUG_CATEGORY_INIT(GST_CAT_DEFAULT, GST_DEFAULT_NAME, 0,
                            GST_DEFAULT_NAME);


    this->thrLoop = true;
    this->snapInterval = 1000;
    this->epName = "EP_NAME_UNINITIALIZED";
    this->storagePath = "/tmp";
    this->framesCounter = 0;
    this->outputFormat = FGFMT_JPEG;
    this->lastQueueTimeStamp = 0;
    this->queueLength = 0;
    this->frameQueue = new avis_blocking_queue<VideoFrame*>;

    this->thr = new boost::thread(boost::bind(
            &TelmateFrameGrabberOpenCVImpl::queueHandler, this));
    this->thr->detach();

    GST_INFO("Constructor was called for %s", this->epName.c_str());
}


TelmateFrameGrabberOpenCVImpl::~TelmateFrameGrabberOpenCVImpl() {


    GST_INFO("Destructor was called for %s", this->epName.c_str());


}

void TelmateFrameGrabberOpenCVImpl::cleanup() {

    VideoFrame *ptrVf;

    while(!this->frameQueue->empty()) {
        this->frameQueue->pop(ptrVf); // blocks
        --this->queueLength;
        delete ptrVf;
        ptrVf = NULL;

    }

    this->thrLoop = false;
    boost::this_thread::sleep_for(boost::chrono::milliseconds(250)); /* Give the processing thread some time to exit() */
    GST_INFO("Called release() for %s :: Dequeue completed.", this->epName.c_str());

    delete this->frameQueue;
    this->frameQueue = NULL;

    return;
}


/*
 * This function will be called with each new frame. mat variable
 * contains the current frame. You should insert your image processing code
 * here. Any changes in mat, will be sent through the Media Pipeline.
 */
void TelmateFrameGrabberOpenCVImpl::process(cv::Mat &mat) {
        if ((this->getCurrentTimestampLong() - this->lastQueueTimeStamp) >= this->snapInterval) {

            if(this->thrLoop) { // do not push into the queue if the destructor was called.
                this->lastQueueTimeStamp = this->getCurrentTimestampLong();
                VideoFrame *ptrVf = new VideoFrame();
                ptrVf->mat = mat.clone();
                ptrVf->ts = std::to_string(this->lastQueueTimeStamp);

                this->frameQueue->push(ptrVf);
                ++this->queueLength;
                ++this->framesCounter;
            }
        }
}
/*
 * This function is executed inside the queueHandler thread as a main() function.
 * It pops a VideoFrame from the framequeue and saves it to disk.
 * a boost scoped_lock is implemented to ensure the queue is emptied to disk before
 * the destructor is executed. a 1 second sleep is implemented inside the while() loop
 * to ensure the cpu isn't exhausted while the queue is empty.
 */
void TelmateFrameGrabberOpenCVImpl::queueHandler() {
        VideoFrame *ptrVf;
        cv::Mat image;
        std::vector<int> params;
        std::string image_extension;

        while (this->thrLoop) {

            this->frameQueue->pop(ptrVf); // blocks
            params.clear();     // clear the vector since the last iteration.
            this->lastQueueTimeStamp = this->getCurrentTimestampLong();
            --this->queueLength;

            switch (this->outputFormat) {
                case FGFMT_JPEG:
                    /* Set jpeg params */
                    params.push_back(CV_IMWRITE_JPEG_QUALITY);
                    params.push_back(FG_JPEG_QUALITY);
                    image_extension = ".jpeg";
                    break;
                case FGFMT_PNG:
                    /* Set PNG parameters, compression etc. */
                    params.push_back(CV_IMWRITE_PNG_COMPRESSION);
                    params.push_back(FG_PNG_QUALITY);
                    image_extension = ".png";
                    break;
                default:
                    /* Defaults to jpeg */
                    params.push_back(CV_IMWRITE_JPEG_QUALITY);
                    params.push_back(FG_JPEG_QUALITY);
                    image_extension = ".jpeg";
                    break;
            }

            std::string filename =
                    std::to_string((long) this->framesCounter) + "_" + ptrVf->ts + image_extension;

            if (this->storagePathSubdir.empty()) {
                this->storagePathSubdir = this->storagePath + "/frames_" + this->getCurrentTimestampString();
                boost::filesystem::path dir(this->storagePathSubdir.c_str());
                if (!boost::filesystem::create_directories(dir)) {
                    GST_ERROR("%s create_directories() failed for: %s", this->epName.c_str(),
                              this->storagePathSubdir.c_str());
                }
            }

            std::string fullpath = this->storagePathSubdir + "/" + filename;

            try {
                cv::imwrite(fullpath.c_str(), ptrVf->mat, params);
            }
            catch (...) {
                GST_ERROR("::queueHandler() imgwrite() failed.");
                throw KurentoException(NOT_IMPLEMENTED,
                                       "TelmateFrameGrabberOpenCVImpl::queueHandler() imgwrite() failed. \n");
            }

            ptrVf->mat.release();   // release internal memory allocations

            delete ptrVf;
            ptrVf = NULL;

        }



}


std::string TelmateFrameGrabberOpenCVImpl::getCurrentTimestampString() {
    struct timeval tp;
    long int ms;
    std::stringstream sstr_ts;

    gettimeofday(&tp, NULL);
    ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    sstr_ts << ms;
    return sstr_ts.str();
}

long TelmateFrameGrabberOpenCVImpl::getCurrentTimestampLong() {
    struct timeval tp;

    gettimeofday(&tp, NULL);
    return (tp.tv_sec * 1000 + tp.tv_usec / 1000);
}

}   // namespace kurento
