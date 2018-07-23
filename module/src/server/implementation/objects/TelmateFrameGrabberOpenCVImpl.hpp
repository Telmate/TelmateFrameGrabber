/* Autogenerated with kurento-module-creator */

#ifndef __TELMATE_FRAME_GRABBER_OPENCV_IMPL_HPP__
#define __TELMATE_FRAME_GRABBER_OPENCV_IMPL_HPP__

#define NDEBUG 1

#include <ctime>
#include <iostream>
#include <OpenCVProcess.hpp>
#include "TelmateFrameGrabber.hpp"
#include <EventHandler.hpp>
#include <string>

#include "VideoFrame.hpp"

#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>

#include <boost/thread/thread.hpp>

#include "avisqueue.hpp"

#include <boost/atomic.hpp>

#include <opencv2/core/mat.hpp>

#include <boost/thread/mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

#include <boost/filesystem.hpp>

#define FGFMT_JPEG  0x0
#define FGFMT_PNG   0x1

#define FG_JPEG_QUALITY 20
#define FG_PNG_QUALITY  9

#define MAX_IDLE_QUEUE_TIME_NS 30000
#define QUEUE_BASE_ELEMENT_ALLOC 1000

//using namespace moodycamel;




namespace kurento {

class TelmateFrameGrabberOpenCVImpl : public virtual OpenCVProcess {
 public:
    TelmateFrameGrabberOpenCVImpl();

    virtual ~TelmateFrameGrabberOpenCVImpl();

    virtual void process(cv::Mat &mat);

    void release();
    void cleanup();

    boost::atomic<int> framesCounter;
    int snapInterval;
    std::string storagePath;
    std::string epName;
    int outputFormat;     // 0x0=JPEG 0x1=PNG



 protected:

    TelmateFrameGrabberOpenCVImpl* getFrameGrabberPtr() {
        return this;
    }

 private:

    boost::asio::io_service ioService;
    boost::thread_group tp;

    avis_blocking_queue<VideoFrame*> *frameQueue;
    boost::thread* thr;
    boost::atomic<bool> thrLoop;

    boost::atomic<int64> lastQueueTimeStamp;
    boost::atomic<int> queueLength;
    std::string storagePathSubdir;

    void queueHandler();
    std::string getCurrentTimestampString();
    int64 getCurrentTimestampLong();

    boost::mutex workerThreadMutex;

};

}   // namespace kurento

#endif /*  __TELMATE_FRAME_GRABBER_OPENCV_IMPL_HPP__ */
