/*
 *
 *
 *
 */

#include <gst/gst.h>
#include "MediaPipeline.hpp"
#include <TelmateFrameGrabberImplFactory.hpp>
#include "TelmateFrameGrabberImpl.hpp"
#include <jsonrpc/JsonSerializer.hpp>
#include <KurentoException.hpp>
#include "MediaPipelineImpl.hpp"

#define GST_CAT_DEFAULT kurento_telmate_frame_grabber_impl
GST_DEBUG_CATEGORY_STATIC (GST_CAT_DEFAULT);
#define GST_DEFAULT_NAME "KurentoTelmateFrameGrabberImpl"

namespace kurento
{
namespace module
{
namespace telmateframegrabber
{

TelmateFrameGrabberImpl::TelmateFrameGrabberImpl (const boost::property_tree::ptree &config, std::shared_ptr<MediaPipeline> mediaPipeline) : OpenCVFilterImpl (config, std::dynamic_pointer_cast<MediaPipelineImpl> (mediaPipeline) )

{
}

int TelmateFrameGrabberImpl::cleanup ()
{
  return TelmateFrameGrabberOpenCVImpl::cleanup ();
}

int TelmateFrameGrabberImpl::getSnapInterval ()
{
  return TelmateFrameGrabberOpenCVImpl::getSnapInterval ();
}

void TelmateFrameGrabberImpl::setSnapInterval (int snapInterval)
{
  return TelmateFrameGrabberOpenCVImpl::setSnapInterval(snapInterval);
}

std::string TelmateFrameGrabberImpl::getStoragePath ()
{
  return TelmateFrameGrabberOpenCVImpl::getStoragePath ();
}

void TelmateFrameGrabberImpl::setStoragePath (const std::string &path)
{
  return TelmateFrameGrabberOpenCVImpl::setStoragePath (path);
}

void TelmateFrameGrabberImpl::setWebRtcEpName (const std::string &epName)
{
  return TelmateFrameGrabberOpenCVImpl::setWebRtcEpName (epName);
}

void TelmateFrameGrabberImpl::setOutputFormat (int outputFormat)
{
  return TelmateFrameGrabberOpenCVImpl::setOutputFormat (outputFormat);
}

int TelmateFrameGrabberImpl::getMonitorTimeoutSec ()
{
  return TelmateFrameGrabberOpenCVImpl::getMonitorTimeoutSec();
}

void TelmateFrameGrabberImpl::setMonitorTimeoutSec (const int timeout)
{
  return TelmateFrameGrabberOpenCVImpl::setMonitorTimeoutSec (timeout);
}

std::string TelmateFrameGrabberImpl::getSessionUUID ()
{
  return TelmateFrameGrabberOpenCVImpl::getSessionUUID ();
}

void TelmateFrameGrabberImpl::setSessionUUID (const std::string &puuid)
{
  return TelmateFrameGrabberOpenCVImpl::setSessionUUID (puuid);
}



MediaObjectImpl *
TelmateFrameGrabberImplFactory::createObject (const boost::property_tree::ptree &config, std::shared_ptr<MediaPipeline> mediaPipeline) const
{
  return new TelmateFrameGrabberImpl (config, mediaPipeline);
}

TelmateFrameGrabberImpl::StaticConstructor TelmateFrameGrabberImpl::staticConstructor;

TelmateFrameGrabberImpl::StaticConstructor::StaticConstructor()
{
  GST_DEBUG_CATEGORY_INIT (GST_CAT_DEFAULT, GST_DEFAULT_NAME, 0,
                           GST_DEFAULT_NAME);
}

} /* telmateframegrabber */
} /* module */
} /* kurento */
