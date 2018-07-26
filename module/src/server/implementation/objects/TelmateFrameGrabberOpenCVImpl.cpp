/*
 *
 *
 *
 */

#include "TelmateFrameGrabberOpenCVImpl.hpp"
#include <KurentoException.hpp>

#define GST_CAT_DEFAULT telmate_frame_grabber_opencv_impl
GST_DEBUG_CATEGORY_STATIC(GST_CAT_DEFAULT);
#define GST_DEFAULT_NAME "TelmateFrameGrabberOpenCVImpl"


namespace kurento
{
namespace module
{
namespace telmateframegrabber
{

TelmateFrameGrabberOpenCVImpl::TelmateFrameGrabberOpenCVImpl ()
{

  GST_DEBUG_CATEGORY_INIT(GST_CAT_DEFAULT, GST_DEFAULT_NAME, 0,
                          GST_DEFAULT_NAME);

  this->uuid = "UUID_UNINITIALIZED";
  this->thrLoop = true;
  this->monThreadLoop = false;
  this->snapInterval = 1000;
  this->epName = "EP_NAME_UNINITIALIZED";
  this->storagePath = "/tmp/";
  this->framesCounter = 0;
  this->outputFormat = FGFMT_JPEG;
  this->lastQueueTimeStamp = 0;
  this->queueLength = 0;
  this->frameQueue = new avis_blocking_queue<VideoFrame*>;
  this->monitorTimeoutMs = 60000; // 60 sec.


  this->thr = new boost::thread(boost::bind(
          &TelmateFrameGrabberOpenCVImpl::queueHandler, this));
  this->thr->detach();

  this->wdThr = new boost::thread(boost::bind(
          &TelmateFrameGrabberOpenCVImpl::watchDogThread, this));
  this->wdThr->detach();

  GST_INFO("Constructor was called for %s", this->epName.c_str());

}

TelmateFrameGrabberOpenCVImpl::~TelmateFrameGrabberOpenCVImpl() {

  VideoFrame *ptrVf;
  this->thrLoop = false;
  this->monThreadLoop = false;

  boost::this_thread::sleep_for(boost::chrono::milliseconds(50));


  while(this->queueLength > 0) {
    this->frameQueue->pop(ptrVf); // blocks
    --this->queueLength;
    if(ptrVf != NULL) {
        delete ptrVf;
        ptrVf = NULL;
    }
  }

  delete this->frameQueue;
  this->frameQueue = NULL;


  GST_INFO("Destructor was called for %s", this->epName.c_str());

}

int TelmateFrameGrabberOpenCVImpl::cleanup() {
    /* for now do nothing, we may need to trigger our destructor from this */
  return 1;
}

/*
 * This function will be called for each new frame. The &mat variable
 * contains the current frame. Any image processing code should
 * be performed here and will be sent back to Media Pipline.
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
 * It pops a VideoFrame from the frame queue and saves it to disk.
 *
 */
void TelmateFrameGrabberOpenCVImpl::queueHandler() {

  VideoFrame *ptrVf;
  cv::Mat image;
  std::vector<int> params;
  std::string image_extension;

  try {

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
          GST_INFO("going to create a directory in %s", this->storagePathSubdir.c_str());
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
          /*throw KurentoException(NOT_IMPLEMENTED,
                                 "TelmateFrameGrabberOpenCVImpl::queueHandler() imgwrite() failed. \n");*/
        }

        ptrVf->mat.release();   // release internal memory allocations

        delete ptrVf;
        ptrVf = NULL;
        GST_DEBUG("queueHandler() ep: %s looping...",this->epName.c_str() );

    }

  } catch(...) {

        ptrVf->mat.release();
        delete ptrVf;
        ptrVf = NULL;
        //GST_INFO("!!! queueHandler() of %s Thread exception! exiting...", this->epName.c_str());
        return;

  }

  GST_INFO("queueHandler() of %s Thread exiting...",this->epName.c_str());
  return;
}

void TelmateFrameGrabberOpenCVImpl::watchDogThread() {

  while(this->monThreadLoop) {

    try {

      boost::this_thread::sleep_for(boost::chrono::seconds(10));

      if((this->getCurrentTimestampLong() - this->lastQueueTimeStamp) >=
         this->monitorTimeoutMs && this->queueLength == 0 && this->framesCounter >= 1) {

        GST_INFO("!!! session:%s :: ep: %s is STILL Active after timeout!",
                 this->uuid.c_str(), this->epName.c_str());

      }
      this->lastQueueTimeStamp = this->getCurrentTimestampLong();


    } catch(...) {
        GST_INFO("!!! watchDogThread is exiting prematurely for %s .", this->epName.c_str());

    }

  }

  GST_INFO("watchDogThread Finished for %s. Exiting...", this->epName.c_str());
  return;

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

int TelmateFrameGrabberOpenCVImpl::getSnapInterval() {
  return this->snapInterval;
}

void TelmateFrameGrabberOpenCVImpl::setSnapInterval (int snapInterval) {
    this->snapInterval = snapInterval;
    GST_INFO("Snapshot interval was set to: %d", this->snapInterval);
    return;
}

std::string TelmateFrameGrabberOpenCVImpl::getStoragePath() {
  return this->storagePath;
}

void TelmateFrameGrabberOpenCVImpl::setStoragePath(const std::string &path) {
    this->storagePath = path;
    GST_INFO("Storage Path was set to: %s", this->storagePath.c_str());
    return;
}

void TelmateFrameGrabberOpenCVImpl::setWebRtcEpName(const std::string &epName) {
    this->epName = epName;
    GST_INFO("Endpoint name was set to: %s", this->epName.c_str());
    return;
}

void TelmateFrameGrabberOpenCVImpl::setOutputFormat(int outputFormat) {
    this->outputFormat = outputFormat;
    GST_INFO("Snapshot output format was set to: %d", this->outputFormat);
    return;
}

void TelmateFrameGrabberOpenCVImpl::setMonitorTimeoutSec(int timeout) {
  std::chrono::seconds sec(timeout);
  std::chrono::milliseconds ms;
  ms = std::chrono::duration_cast<std::chrono::milliseconds> (sec);
  this->monitorTimeoutMs = ms.count();
  GST_INFO("Monitoring Thread timeout was set to: %d ms", (int)this->monitorTimeoutMs);
  return;
}

int TelmateFrameGrabberOpenCVImpl::getMonitorTimeoutSec() {
  return (this->monitorTimeoutMs );
}

std::string TelmateFrameGrabberOpenCVImpl::getSessionUUID() {
  return this->uuid;
}

void TelmateFrameGrabberOpenCVImpl::setSessionUUID(const std::string &puuid) {
  this->uuid = puuid;
  GST_INFO("Session UUID was set to: %s", this->uuid.c_str());
  return;
}



} /* telmateframegrabber */
} /* module */
} /* kurento */
