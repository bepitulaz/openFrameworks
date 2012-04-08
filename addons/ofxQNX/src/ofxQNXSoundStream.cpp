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

#include "ofSoundStream.h"
#include "ofMath.h"
#include "ofUtils.h"

#include <sys/asoundlib.h>

#include "ofxQNXSoundStream.h"

static int     bsize;

//------------------------------------------------------------------------------
static pthread_t thread;
static void *playbackCallback(void *arg);

static snd_pcm_t *pcm_handle;
static snd_mixer_t *mixer_handle;

static int nInputChannels;
static int nOutputChannels;
static int sampleRate;
static int bufferSize;

static float * out_float_buffer	= NULL;
static short * out_buffer		= NULL;

static bool							isSetup			= false;
static bool							isRunning		= false;
static ofBaseSoundInput *			soundInputPtr	= NULL;
static ofBaseSoundOutput *			soundOutputPtr	= NULL;

//------------------------------------------------------------------------------

static int openQNXAudio()
{
	fprintf(stderr, "ofxQNXSoundStream::openQNXAudio\n");

	int     rtn;
	int     card = -1;
	int     dev = 0;

	snd_pcm_channel_info_t pi;
	snd_pcm_channel_params_t pp;
	snd_mixer_group_t group;
	snd_pcm_channel_setup_t setup;

	// Change?
	//#define FRAGNUM 16
	//#define FRAGSIZE 1024
	int     fragsize = -1;
	int     num_frags = -1;

// Open PCM
	if ((rtn = snd_pcm_open_preferred (&pcm_handle, &card, &dev, SND_PCM_OPEN_PLAYBACK)) < 0)
	{
		fprintf(stderr, "device open");
		return -1;
	}

// disabling mmap is not actually required in this example but it is included to
// demonstrate how it is used when it is required.
	if ((rtn = snd_pcm_plugin_set_disable (pcm_handle, PLUGIN_DISABLE_MMAP)) < 0)
	{
		fprintf(stderr, "snd_pcm_plugin_set_disable failed: %s\n", snd_strerror (rtn));
		return -1;
	}

// get pcm info
	memset (&pi, 0, sizeof (pi));
	pi.channel = SND_PCM_CHANNEL_PLAYBACK;
	if ((rtn = snd_pcm_plugin_info (pcm_handle, &pi)) < 0)
	{
		fprintf(stderr, "snd_pcm_plugin_info failed: %s\n", snd_strerror (rtn));
		return -1;
	}

// setup pcm channel
	memset (&pp, 0, sizeof (pp));
	pp.mode = SND_PCM_MODE_BLOCK;
	pp.channel = SND_PCM_CHANNEL_PLAYBACK;
	pp.start_mode = SND_PCM_START_FULL;
	pp.stop_mode = SND_PCM_STOP_STOP;
	pp.buf.block.frag_size = pi.max_fragment_size;	// bufferSize; // use value set by user instead of what snd_pcm_plugin_info() reports?

	if (fragsize != -1)
	{
		pp.buf.block.frag_size = fragsize;
	}
	pp.buf.block.frags_max = 6; // setting this to 4 seems to lower the latency, if value doesnt works, increase it  // num_frags;
	pp.buf.block.frags_min = 1;

	pp.format.interleave = 1;
	pp.format.rate = sampleRate;
	pp.format.voices = nOutputChannels;
	pp.format.format = SND_PCM_SFMT_S16_LE;	// Signed 16-bit Little Endian

	strcpy (pp.sw_mixer_subchn_name, "Wave playback channel");
	if ((rtn = snd_pcm_plugin_params (pcm_handle, &pp)) < 0)
	{
		fprintf(stderr, "snd_pcm_plugin_params failed: %s\n", snd_strerror (rtn));
		return -1;
	}

	if ((rtn = snd_pcm_plugin_prepare (pcm_handle, SND_PCM_CHANNEL_PLAYBACK)) < 0) {
		fprintf(stderr, "snd_pcm_plugin_prepare failed: %s\n", snd_strerror (rtn));
		return -1;
	}

	memset (&setup, 0, sizeof (setup));
	memset (&group, 0, sizeof (group));
	setup.channel = SND_PCM_CHANNEL_PLAYBACK;
	setup.mixer_gid = &group.gid;

	if ((rtn = snd_pcm_plugin_setup (pcm_handle, &setup)) < 0)
	{
		fprintf(stderr, "snd_pcm_plugin_setup failed: %s\n", snd_strerror (rtn));
		return -1;
	}

	if (group.gid.name[0] == 0)
	{
		fprintf(stderr, "Mixer Pcm Group [%s] Not Set \n", group.gid.name);
		return -1;
	}

	if ((rtn = snd_mixer_open (&mixer_handle, card, setup.mixer_device)) < 0)
	{
		fprintf(stderr, "snd_mixer_open failed: %s\n", snd_strerror (rtn));
		return -1;
	}

	fprintf(stderr, "Format %s \n", snd_pcm_get_format_name (setup.format.format));
	fprintf(stderr, "Frag Size %d \n", setup.buf.block.frag_size);
	fprintf(stderr, "Total Frags %d \n", setup.buf.block.frags);
	fprintf(stderr, "Rate %d \n", setup.format.rate);
	fprintf(stderr, "Voices %d \n", setup.format.voices);
	fprintf(stderr, "Mixer Pcm Group [%s]\n", group.gid.name);

	// final buffer size
	bsize = setup.buf.block.frag_size;

	out_float_buffer = new float[bsize * nOutputChannels];
	out_buffer = new short[bsize * nOutputChannels];

	// Start audio thread
	pthread_create (&thread, NULL, playbackCallback, NULL);
	pthread_detach (thread);

	return 0;
}

static void updateQNXAudio() {

	soundOutputPtr->audioOut(out_float_buffer, bsize, nOutputChannels);

	for(int i=0;i<bsize*nOutputChannels ;i++){
		float tempf = (out_float_buffer[i] * 32767.5f) - 0.5f;
		out_buffer[i] = tempf;
	}

	int written = snd_pcm_plugin_write (pcm_handle, out_buffer, bsize);
	//fprintf(stderr, "written %d\n", written);
}

static void closeQNXAudio() {
	fprintf(stderr, "ofxQNXSoundStream::closeQNXAudio\n");

	snd_pcm_plugin_flush (pcm_handle, SND_PCM_CHANNEL_PLAYBACK);
	snd_mixer_close (mixer_handle);
	snd_pcm_close (pcm_handle);
}

static void *playbackCallback(void *arg)
{
	while(true) {
		updateQNXAudio();
	}

	return 0;
}

//------------------------------------------------------------------------------

ofxQNXSoundStream::ofxQNXSoundStream()
{
	fprintf(stderr, "ofxQNXSoundStream::ofxQNXSoundStream()\n");
}

ofxQNXSoundStream::~ofxQNXSoundStream()
{
	fprintf(stderr, "ofxQNXSoundStream::~ofxQNXSoundStream()\n");
	//closeQNXAudio(); // causes segfault
}

void ofxQNXSoundStream::setInput(ofBaseSoundInput * soundInput)
{
	fprintf(stderr, "ofxQNXSoundStream::setInput(ofBaseSoundInput)\n");
	soundInputPtr = soundInput;
}

void ofxQNXSoundStream::setOutput(ofBaseSoundOutput * soundOutput)
{
	fprintf(stderr, "ofxQNXSoundStream::setInput(ofBaseSoundOutput)\n");
	soundOutputPtr = soundOutput;
}

bool ofxQNXSoundStream::setup(int outChannels, int inChannels, int _sampleRate, int _bufferSize, int nBuffers)
{
	fprintf(stderr, "ofxQNXSoundStream::setup(outChannels %d, inChannels %d, sampleRate %d, bufferSize %d, nBuffers %d)\n", outChannels, inChannels, _sampleRate, _bufferSize, nBuffers);

	nInputChannels = inChannels;
	nOutputChannels = outChannels;
	tickCount = 0;
	sampleRate = _sampleRate;
	bufferSize = _bufferSize;

	// not used.
	nBuffers = 1;

	// Setup QNX
	openQNXAudio();

	if(isRunning) {
		stop();
		close();
	}

	isSetup = true;
	ofSoundStreamStart();

	return true;
}

bool ofxQNXSoundStream::setup(ofBaseApp * app, int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers)
{
	fprintf(stderr, "ofxQNXSoundStream::setup(ofBaseApp)\n");
	setInput(app);
	setOutput(app);
	return setup(outChannels, inChannels, sampleRate, bufferSize, nBuffers);
}

void ofxQNXSoundStream::start()
{
	fprintf(stderr, "ofxQNXSoundStream::start\n");
	// TODO
	if(isRunning)
		ofSoundStreamStop();
}

void ofxQNXSoundStream::stop()
{
	fprintf(stderr, "ofxQNXSoundStream::stop\n");
	// TODO
}

void ofxQNXSoundStream::close()
{
	fprintf(stderr, "ofxQNXSoundStream::close\n");
	// TODO
}

long unsigned long ofxQNXSoundStream::getTickCount()
{
	return tickCount;
}

int ofxQNXSoundStream::getNumInputChannels(){
	return nInputChannels;
}

int ofxQNXSoundStream::getNumOutputChannels(){
	return nOutputChannels;
}
