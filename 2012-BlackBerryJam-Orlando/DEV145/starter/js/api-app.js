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
function displayAboutInfo() {
	console.log("displayAboutInfo()");
	var name, author, version, license;
	
	//1. Read the application name using blackberry.app API

	//2. Read the application author using blackberry.app API

	//3. Read the application version using blackberry.app API

	//4. Read the application license using blackberry.app API

	
	//Display application info to the page
	document.getElementById("txtAppName").value = name;
	document.getElementById("txtAppAuthor").value = author;
	document.getElementById("txtAppVersion").value = version;
	document.getElementById("txtAppLicense").value = license;
	
}

//Do not change - display the welcome message when this screen has been loaded:
setTimeout(displayAboutInfo, 100);