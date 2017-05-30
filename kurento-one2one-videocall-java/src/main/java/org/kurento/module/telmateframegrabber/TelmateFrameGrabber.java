/**
 * This file is generated with Kurento-maven-plugin.
 * Please don't edit.
 */
package org.kurento.module.telmateframegrabber;

import org.kurento.client.*;

/**
 *
 * TelmateFrameGrabber interface. Documentation about the module
 *
 **/
@org.kurento.client.internal.RemoteClass
public interface TelmateFrameGrabber extends OpenCVFilter {



/**
 *
 * get Snapshot interval
 * @return Value of current snapshot interval *
 **/
  int getSnapInterval();

/**
 *
 * Asynchronous version of getSnapInterval:
 * {@link Continuation#onSuccess} is called when the action is
 * done. If an error occurs, {@link Continuation#onError} is called.
 * @see TelmateFrameGrabber#getSnapInterval
 *
 **/
    void getSnapInterval(Continuation<Integer> cont);

/**
 *
 * get Snapshot interval
 * @return Value of current snapshot interval *
 **/
    TFuture<Integer> getSnapInterval(Transaction tx);


/**
 *
 * set Snapshot interval.
 *
 * @param snapInterval
 *       Value of snapInterval in milliseconds
 *
 **/
  void setSnapInterval(@org.kurento.client.internal.server.Param("snapInterval") int snapInterval);

/**
 *
 * Asynchronous version of setSnapInterval:
 * {@link Continuation#onSuccess} is called when the action is
 * done. If an error occurs, {@link Continuation#onError} is called.
 * @see TelmateFrameGrabber#setSnapInterval
 *
 * @param snapInterval
 *       Value of snapInterval in milliseconds
 *
 **/
    void setSnapInterval(@org.kurento.client.internal.server.Param("snapInterval") int snapInterval, Continuation<Void> cont);

/**
 *
 * set Snapshot interval.
 *
 * @param snapInterval
 *       Value of snapInterval in milliseconds
 *
 **/
    void setSnapInterval(Transaction tx, @org.kurento.client.internal.server.Param("snapInterval") int snapInterval);


/**
 *
 * get the storage location for snapshots
 * @return path of snapshots location *
 **/
  String getStoragePath();

/**
 *
 * Asynchronous version of getStoragePath:
 * {@link Continuation#onSuccess} is called when the action is
 * done. If an error occurs, {@link Continuation#onError} is called.
 * @see TelmateFrameGrabber#getStoragePath
 *
 **/
    void getStoragePath(Continuation<String> cont);

/**
 *
 * get the storage location for snapshots
 * @return path of snapshots location *
 **/
    TFuture<String> getStoragePath(Transaction tx);


/**
 *
 * set the storage location for snapshots
 *
 * @param path
 *       path of snapshots location
 *
 **/
  void setStoragePath(@org.kurento.client.internal.server.Param("path") String path);

/**
 *
 * Asynchronous version of setStoragePath:
 * {@link Continuation#onSuccess} is called when the action is
 * done. If an error occurs, {@link Continuation#onError} is called.
 * @see TelmateFrameGrabber#setStoragePath
 *
 * @param path
 *       path of snapshots location
 *
 **/
    void setStoragePath(@org.kurento.client.internal.server.Param("path") String path, Continuation<Void> cont);

/**
 *
 * set the storage location for snapshots
 *
 * @param path
 *       path of snapshots location
 *
 **/
    void setStoragePath(Transaction tx, @org.kurento.client.internal.server.Param("path") String path);


/**
 *
 * set the webetc endpoint name (used for storage location of snapshots)
 *
 * @param epName
 *       endpoint name
 *
 **/
  void setWebRtcEpName(@org.kurento.client.internal.server.Param("epName") String epName);

/**
 *
 * Asynchronous version of setWebRtcEpName:
 * {@link Continuation#onSuccess} is called when the action is
 * done. If an error occurs, {@link Continuation#onError} is called.
 * @see TelmateFrameGrabber#setWebRtcEpName
 *
 * @param epName
 *       endpoint name
 *
 **/
    void setWebRtcEpName(@org.kurento.client.internal.server.Param("epName") String epName, Continuation<Void> cont);

/**
 *
 * set the webetc endpoint name (used for storage location of snapshots)
 *
 * @param epName
 *       endpoint name
 *
 **/
    void setWebRtcEpName(Transaction tx, @org.kurento.client.internal.server.Param("epName") String epName);


/**
 *
 * set the image storage format, 0x0=JPEG 0x1=PNG
 *
 * @param outputFormat
 *       output format
 *
 **/
  void setOutputFormat(@org.kurento.client.internal.server.Param("outputFormat") int outputFormat);

/**
 *
 * Asynchronous version of setOutputFormat:
 * {@link Continuation#onSuccess} is called when the action is
 * done. If an error occurs, {@link Continuation#onError} is called.
 * @see TelmateFrameGrabber#setOutputFormat
 *
 * @param outputFormat
 *       output format
 *
 **/
    void setOutputFormat(@org.kurento.client.internal.server.Param("outputFormat") int outputFormat, Continuation<Void> cont);

/**
 *
 * set the image storage format, 0x0=JPEG 0x1=PNG
 *
 * @param outputFormat
 *       output format
 *
 **/
    void setOutputFormat(Transaction tx, @org.kurento.client.internal.server.Param("outputFormat") int outputFormat);





    public class Builder extends AbstractBuilder<TelmateFrameGrabber> {

/**
 *
 * Creates a Builder for TelmateFrameGrabber
 *
 **/
    public Builder(org.kurento.client.MediaPipeline mediaPipeline){

      super(TelmateFrameGrabber.class,mediaPipeline);

      props.add("mediaPipeline",mediaPipeline);
    }

	public Builder withProperties(Properties properties) {
    	return (Builder)super.withProperties(properties);
  	}

	public Builder with(String name, Object value) {
		return (Builder)super.with(name, value);
	}

    }


}