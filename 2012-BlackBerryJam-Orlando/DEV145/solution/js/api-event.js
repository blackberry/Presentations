/*
 * Copyright 2012 Research In Motion Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
function doPause() {
	console.log("doPause");
	var newElement, dt = new Date(), outputMessages;
	
	//Get message container:
	outputMessages = document.getElementById("eventMessages");
	
	
	//2. Display message (in bold text) indicating pause event has occurred.
	newElement = document.createElement("b");
	newElement.innerHTML = "Application paused : " + dt + "<br/>";
	
	
	//Add message to page:	
	outputMessages.appendChild(newElement);
}
function doResume() {
	console.log("doResume");
	var newElement, dt = new Date(), outputMessages;
	
	//Get message container:
	outputMessages = document.getElementById("eventMessages");

	
	//3. Display message (in italics text) indicating resume event has occurred.
	newElement = document.createElement("i");
	newElement.innerHTML = "Application resumed : " + dt + "<br/>";

	
	//Add message to page:	
	outputMessages.appendChild(newElement);
}


//1. Create event listeners for the pause and resume events
blackberry.event.addEventListener("resume", doResume);
blackberry.event.addEventListener("pause", doPause);
