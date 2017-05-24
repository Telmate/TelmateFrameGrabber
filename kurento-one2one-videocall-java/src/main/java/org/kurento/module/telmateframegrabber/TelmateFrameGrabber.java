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