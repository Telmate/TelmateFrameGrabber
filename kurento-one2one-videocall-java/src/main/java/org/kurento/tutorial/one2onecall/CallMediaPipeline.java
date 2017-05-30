/*
 * (C) Copyright 2014 Kurento (http://kurento.org/)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

package org.kurento.tutorial.one2onecall;

import org.kurento.client.KurentoClient;
import org.kurento.client.MediaPipeline;
import org.kurento.client.WebRtcEndpoint;
import org.kurento.module.telmateframegrabber.TelmateFrameGrabber;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.kurento.module.telmateframegrabber.TelmateFrameGrabber;

/**
 * Media Pipeline (WebRTC endpoints, i.e. Kurento Media Elements) and connections for the 1 to 1
 * video communication.
 * 
 * @author Boni Garcia (bgarcia@gsyc.es)
 * @author Micael Gallego (micael.gallego@gmail.com)
 * @since 4.3.1
 */
public class CallMediaPipeline {

  private MediaPipeline pipeline;
  private WebRtcEndpoint callerWebRtcEp;
  private WebRtcEndpoint calleeWebRtcEp;

  private static final Logger TheLogger = LoggerFactory.getLogger(CallMediaPipeline.class);  

  private TelmateFrameGrabber TelmateFrameGrabber1 = null;
  private TelmateFrameGrabber TelmateFrameGrabber2 = null;

  private int snapinterval = 3000;
  private String stoagePath = "/tmp/";
  public CallMediaPipeline(KurentoClient kurento) {

    try {
      pipeline = kurento.createMediaPipeline();
      callerWebRtcEp = new WebRtcEndpoint.Builder(pipeline).build();
      calleeWebRtcEp = new WebRtcEndpoint.Builder(pipeline).build();
      TelmateFrameGrabber1 = new TelmateFrameGrabber.Builder(pipeline).build();
      TelmateFrameGrabber2 = new TelmateFrameGrabber.Builder(pipeline).build();

      /* First endpoint */
      callerWebRtcEp.connect(TelmateFrameGrabber1);
      TelmateFrameGrabber1.connect(calleeWebRtcEp);

      TelmateFrameGrabber1.setSnapInterval(snapinterval);
      TelmateFrameGrabber1.setWebRtcEpName("TelmateFrameGrabber1");
      TelmateFrameGrabber1.setStoragePath(stoagePath + "/TelmateFrameGrabber1");


      /* Second endpoint */
      calleeWebRtcEp.connect(TelmateFrameGrabber2);
      TelmateFrameGrabber2.connect(calleeWebRtcEp);

      TelmateFrameGrabber2.setSnapInterval(snapinterval);
      TelmateFrameGrabber2.setWebRtcEpName("TelmateFrameGrabber2");
      TelmateFrameGrabber2.setStoragePath(stoagePath + "/TelmateFrameGrabber2");
      
    } catch (Throwable t) {
      if (this.pipeline != null) {
        pipeline.release();
      }
    }
  }
  

  public String generateSdpAnswerForCaller(String sdpOffer) {
    return callerWebRtcEp.processOffer(sdpOffer);
  }

  public String generateSdpAnswerForCallee(String sdpOffer) {
    return calleeWebRtcEp.processOffer(sdpOffer);
  }

  public void release() {
    if (pipeline != null) {
      pipeline.release();
    }
  }

  public WebRtcEndpoint getCallerWebRtcEp() {
    return callerWebRtcEp;
  }

  public WebRtcEndpoint getCalleeWebRtcEp() {
    return calleeWebRtcEp;
  }

}
