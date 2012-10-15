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
function displayCurrentOrientation() {
	console.log("displayCurrentOrientation()");
	var orientationRaw, orientationTxt;
	
	//2. Read the current orientation value
	orientationRaw = window.orientation;
	
	//3. Interpret the orientation value (0 = Top side up, 90 = Right side up)
	if (!orientationRaw) {
		orientationTxt = "Undefined";
	}
	else if ((orientationRaw == 0) || (orientationRaw == 180)) {
		orientationTxt = "Portrait";
	} else {
		orientationTxt = "Landscape";
	}
	
	//4. Output a message indicating whether user is in portrait or landscape mode:
	document.getElementById("txtOrientation").value = orientationTxt;
}

//1. Create an event listener for the orientation change event
window.addEventListener("orientationchange", displayCurrentOrientation, true);
window.addEventListener("deviceorientation", displayCurrentOrientation, true);

//Do not change - display the current orientation automatically when this screen has been loaded:
setTimeout(displayCurrentOrientation, 100);