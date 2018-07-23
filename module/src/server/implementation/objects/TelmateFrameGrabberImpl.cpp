/* Autogenerated with kurento-module-creator */

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

TelmateFrameGrabberImpl::TelmateFrameGrabberImpl (const boost::property_tree::ptree &config,
                                                  std::shared_ptr<MediaPipeline> mediaPipeline) :
        OpenCVFilterImpl (config, std::dynamic_pointer_cast<MediaPipelineImpl> (mediaPipeline) )

{

    g_object_set (element, "filter-factory", "telmateframegrabberopencvimpl", NULL);
    g_object_get (G_OBJECT (element), "filter", &telmateframegrabberopencvimpl, NULL);

    if (telmateframegrabberopencvimpl == nullptr) {
        throw KurentoException (MEDIA_OBJECT_NOT_AVAILABLE,
                                "Media Object not available");
    }

    g_object_set (telmateframegrabberopencvimpl, "target-object",
                  static_cast<kurento::TelmateFrameGrabberOpenCVImpl *> (this), NULL);

    g_object_unref (telmateframegrabberopencvimpl);

    pTelmateFrameGrabberOpenCVImpl = (TelmateFrameGrabberOpenCVImpl*) telmateframegrabberopencvimpl;

}


int TelmateFrameGrabberImpl::cleanup()
{
    pTelmateFrameGrabberOpenCVImpl->cleanup();
    return 1;
}

int TelmateFrameGrabberImpl::getSnapInterval ()
{
    return pTelmateFrameGrabberOpenCVImpl->snapInterval;
}

void TelmateFrameGrabberImpl::setSnapInterval (int snapInterval)
{
    pTelmateFrameGrabberOpenCVImpl->snapInterval = snapInterval;
    return;
}

std::string TelmateFrameGrabberImpl::getStoragePath ()
{
    return pTelmateFrameGrabberOpenCVImpl->storagePath;
}

void TelmateFrameGrabberImpl::setStoragePath (const std::string &path)
{
    pTelmateFrameGrabberOpenCVImpl->storagePath = path;
}

void TelmateFrameGrabberImpl::setWebRtcEpName (const std::string &epName)
{
    pTelmateFrameGrabberOpenCVImpl->epName = epName;
    return;
}

void TelmateFrameGrabberImpl::setOutputFormat (int outputFormat)
{
    pTelmateFrameGrabberOpenCVImpl->outputFormat = outputFormat;
    return;
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
