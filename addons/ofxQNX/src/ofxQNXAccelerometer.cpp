/*
	ofxQNX - BlackBerry PlayBook and BlackBerry 10 add-on for openFrameworks

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

#include "ofxAccelerometer.h"
#include "ofxQNXApp.h"
#include <bps/sensor.h>

static const int ACCELEROMETER_RATE = 25000;

void ofxAccelerometerHandler::setup(){
	fprintf(stderr, "ofxAccelerometerHandler::setup()\n");
	// Initialize the accelerometer by setting the rates at which the
	// accelerometer values will be updated from hardware

	sensor_set_rate(SENSOR_TYPE_ACCELEROMETER, ACCELEROMETER_RATE);
	sensor_set_skip_duplicates(SENSOR_TYPE_ACCELEROMETER, true);

	sensor_request_events(SENSOR_TYPE_ACCELEROMETER);
}

void ofxAccelerometerHandler::exit() {

}
