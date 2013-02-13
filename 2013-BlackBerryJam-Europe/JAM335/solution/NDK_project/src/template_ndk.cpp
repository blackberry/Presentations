/*
 * Copyright 2013 Research In Motion Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string>
#include <sstream>
#include <json/reader.h>
#include <json/writer.h>
#include <pthread.h>
#include "template_ndk.hpp"
#include "template_js.hpp"

#include <errno.h>
#include <bps/led.h>
#include <bps/bps.h>

namespace webworks {

TemplateNDK::TemplateNDK(TemplateJS *parent) {
	m_pParent = parent;
	templateProperty = 50;
	templateThreadCount = 1;
	m_thread = 0;
	pthread_cond_t cond  = PTHREAD_COND_INITIALIZER;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	threadHalt = true;
	blinkCount = 3;
	bps_initialize();	//BPS is only required for LED functionality
}

TemplateNDK::~TemplateNDK() {
	bps_shutdown();		//BPS is only required for LED functionality
}



// These methods are the true native code we intend to reach from WebWorks



/** BEGIN LED CODE **/

std::string TemplateNDK::startLED(std::string arg) {
	//creates a reference id for user
	stringstream ss;
	ss << clock();
	string id = ss.str();

	//default green
	int colorCode = LED_COLOR_GREEN;

	// attempt to convert hexcolor string passed in from JavaScript to integer
	char * p;
	long n = strtoul(arg.c_str(), &p, 16);

	if (*p != 0) {
		fprintf(stderr, "Invalid color. default red");
		//red will be used as an error color
		colorCode = LED_COLOR_RED;
	} else {
		colorCode = (int) n;
		fprintf(stdout, "hex color converted to int");
	}

	//http://origin-developer.blackberry.com/native/reference/bb10/com.qnx.doc.bps.lib_ref/com.qnx.doc.bps.lib_ref/topic/led_request_rgb.html

	int status;
	status = led_request_rgb(id.c_str(), colorCode, blinkCount);



	if (status == BPS_SUCCESS) {
		fprintf(stdout, "success led_request_rgb");
		return id;
	} else {
		fprintf(stderr, "error in led_request_rgb");
		fprintf(stderr, strerror(errno));
		return strerror(errno);
	}

}
std::string TemplateNDK::stopLED(std::string id) {

	int status;
	status = led_cancel(id.c_str());



	if (status == BPS_SUCCESS) {
		fprintf(stdout, "led_cancel");
		return "BPS_SUCCESS";
	} else {
		fprintf(stderr, strerror(errno));
		return strerror(errno);
	}
}
std::string TemplateNDK::getBlinkCountProperty() {
	//convert the integer into a string:
	stringstream ss;
	ss << blinkCount;
	return ss.str();
}
void TemplateNDK::setBlinkCountProperty(const std::string& inputString) {
	//convert a string into an integer:
	blinkCount = (int) strtoul(inputString.c_str(), NULL, 10);
}
/** END LED CODE **/



//
// The following are not used by this Lab.  However they are good examples about
//	how to implement different types of functions & properties in an extension.
//
std::string TemplateNDK::templateTestString() {
	return "Template Test Function";
}

// Take in input and return a value
std::string TemplateNDK::templateTestString(const std::string& inputString) {
	return "Template Test Function, got: " + inputString;
}

// Get an integer property
std::string TemplateNDK::getTemplateProperty() {
	stringstream ss;
	ss << templateProperty;
	return ss.str();
}

// set an integer property
void TemplateNDK::setTemplateProperty(const std::string& inputString) {
	templateProperty = (int) strtoul(inputString.c_str(), NULL, 10);
}

// Asynchronous call
void TemplateNDK::templateTestAsync() {
	templateCallback();
}

// Asynchronous call returning JSON data
void TemplateNDK::templateTestAsyncJSON() {
	templateCallbackJSON();
}

// Asynchronous callback
void TemplateNDK::templateCallback() {
	std::string event = "community.templateExt.aSyncCallback";
	m_pParent->NotifyEvent(event);
}

// Asynchronous callback with JSON data object
void TemplateNDK::templateCallbackJSON() {
	Json::FastWriter writer;
	Json::Value root;
	root["templateJSONString"] = "JSON String";
	root["templateJSONInt"] = 85;

	std::string event = "community.templateExt.aSyncJSONCallback";
	m_pParent->NotifyEvent(event + " " + writer.write(root));
}

// Asynchronous callback with JSON data input and output
void TemplateNDK::templateCallbackJSONio(const std::string& inputString) {
	std::string event = "community.templateExt.aSyncJSONCallbackResult";

	// Parse the arg string as JSON
	Json::FastWriter writer;
	Json::Reader reader;
	Json::Value root;
	bool parse = reader.parse(inputString, root);

	if (!parse) {
		Json::Value error;
		error["result"] = "Cannot parse JSON object";
		m_pParent->NotifyEvent(event + " " + writer.write(error));
	} else {
		root["result"] = root["value1"].asInt() + root["value2"].asInt();
		m_pParent->NotifyEvent(event + " " + writer.write(root));
	}
}

// Thread functions
// The following functions are for controlling a Thread in the extension

// The actual thread (must appear before the startThread method)
// Loops and runs the callback method
void* TemplateThread(void* parent) {
	TemplateNDK *pParent = static_cast<TemplateNDK *>(parent);

	// Loop calls the callback function and continues until stop is set
	while (!pParent->isThreadHalt()) {
		pParent->templateThreadCallback();
		sleep(1);
	}

	return NULL;
}

// Starts the thread and returns a message on status
std::string TemplateNDK::templateStartThread() {
	if (!m_thread) {
		int rc;
	    rc = pthread_mutex_lock(&mutex);
	    threadHalt = false;
	    rc = pthread_cond_signal(&cond);
	    rc = pthread_mutex_unlock(&mutex);

		pthread_attr_t thread_attr;
		pthread_attr_init(&thread_attr);
		pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);

		pthread_create(&m_thread, &thread_attr, TemplateThread,
				static_cast<void *>(this));
		pthread_attr_destroy(&thread_attr);
		return "Thread Started";
	} else {
		return "Thread Running";
	}
}

// Sets the stop value
std::string TemplateNDK::templateStopThread() {
	int rc;
	// Request thread to set prevent sleep to false and terminate
	rc = pthread_mutex_lock(&mutex);
	threadHalt = true;
	rc = pthread_cond_signal(&cond);
	rc = pthread_mutex_unlock(&mutex);

    // Wait for the thread to terminate.
    void *exit_status;
    rc = pthread_join(m_thread, &exit_status) ;

	// Clean conditional variable and mutex
	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mutex);

	m_thread = 0;
	threadHalt = true;
	return "Thread stopped";
}

// The callback method that sends an event through JNEXT
void TemplateNDK::templateThreadCallback() {
	std::string event = "community.templateExt.jsonThreadCallback";
	Json::FastWriter writer;
	Json::Value root;
	root["threadCount"] = templateThreadCount++;
	m_pParent->NotifyEvent(event + " " + writer.write(root));
}

// getter for the stop value
bool TemplateNDK::isThreadHalt() {
	int rc;
	bool isThreadHalt;
	rc = pthread_mutex_lock(&mutex);
	isThreadHalt = threadHalt;
	rc = pthread_mutex_unlock(&mutex);
	return isThreadHalt;
}

} /* namespace webworks */
