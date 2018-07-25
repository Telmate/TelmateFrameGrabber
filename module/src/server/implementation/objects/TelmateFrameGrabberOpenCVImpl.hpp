/*
 *
 *
 *
 */

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
#include <chrono>

#include <gst/gst.h>
#include <KurentoException.hpp>
#include <vector>
#include <string>

#define FGFMT_JPEG  0x0
#define FGFMT_PNG   0x1

#define FG_JPEG_QUALITY 20
#define FG_PNG_QUALITY  9

#define MAX_IDLE_QUEUE_TIME_NS 30000
#define QUEUE_BASE_ELEMENT_ALLOC 1000

namespace kurento
{
namespace module
{
namespace telmateframegrabber
{

class TelmateFrameGrabberOpenCVImpl : public virtual OpenCVProcess
{

public:

  TelmateFrameGrabberOpenCVImpl ();
  virtual ~TelmateFrameGrabberOpenCVImpl ();

  virtual void process (cv::Mat &mat);
  int cleanup();
  int getSnapInterval();
  void setSnapInterval(int snapInterval);
  std::string getStoragePath();
  void setStoragePath(const std::string &path);
  void setWebRtcEpName(const std::string &epName);
  void setOutputFormat(int outputFormat);
  void setMonitorTimeoutSec(int timeout);
  int getMonitorTimeoutSec();
  std::string getSessionUUID();
  void setSessionUUID(const std::string &puuid );

private:
  std::string getCurrentTimestampString();
  int64 getCurrentTimestampLong();
  std::string storagePathSubdir;
  boost::atomic<int> framesCounter;
  int snapInterval;
  std::string storagePath;
  std::string epName;
  int outputFormat;
  avis_blocking_queue<VideoFrame*> *frameQueue;
  boost::thread* thr;
  boost::thread* wdThr;

  std::string uuid;
  boost::atomic<bool> thrLoop;
  boost::atomic<bool> monThreadLoop;
  boost::atomic<int64> lastQueueTimeStamp;
  boost::atomic<int> queueLength;
  int64 monitorTimeoutMs;

  void queueHandler();
  void watchDogThread();


};

} /* telmateframegrabber */
} /* module */
} /* kurento */

#endif /*  __TELMATE_FRAME_GRABBER_OPENCV_IMPL_HPP__ */
