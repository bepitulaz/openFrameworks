/*
	ofxQNX - BlackBerry PlayBook add-on for openFrameworks

	Copyright (c) 2012, Laurence Muller (www.multigesture.net)
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

		* Redistributions of source code must retain the above copyright
		  notice, this list of conditions and the following disclaimer.
		* Redistributions in binary form must reproduce the above copyright
		  notice, this list of conditions and the following disclaimer in the
		  documentation and/or other materials provided with the distribution.
		* Neither the name of the author nor the
		  names of its contributors may be used to endorse or promote products
		  derived from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
	ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef OFXQNXSOUNDSTREAM_H_
#define OFXQNXSOUNDSTREAM_H_

#include "ofConstants.h"
#include "ofBaseSoundStream.h"
#include "ofTypes.h"
#include <sys/asoundlib.h>

#include "ofMain.h"

class ofxQNXSoundStream : public ofBaseSoundStream, public ofThread {
	public:
		ofxQNXSoundStream();
		virtual ~ofxQNXSoundStream();

		void listDevices();
		void setDeviceID(int deviceID);

		bool setup(int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers);
		bool setup(ofBaseApp * app, int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers);

		void setInput(ofBaseSoundInput * soundInput);
		void setOutput(ofBaseSoundOutput * soundOutput);

		void start();
		void stop();
		void close();

		long unsigned long getTickCount();

		int getNumInputChannels();
		int getNumOutputChannels();

		int openQNXAudio();
		void updateQNXAudio();
		void closeQNXAudio();

		// audio thread
		void threadedFunction() {
			while( isThreadRunning() != 0 ){
				//if( lock() ) {
					updateQNXAudio();
				//	unlock();
					//ofSleepMillis(10);
				//}
			}
		}

	private:
		long unsigned long	tickCount;
		int					nInputChannels;
		int					nOutputChannels;
		int					sampleRate;

		// Playbook specific
		snd_pcm_t *pcm_handle;
		snd_mixer_t *mixer_handle;

		float * out_float_buffer;
		short * out_buffer;
		int     bsize;
};

#endif /* OFXQNXSOUNDSTREAM_H_ */
