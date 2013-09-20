/*
 * Copyright 2013 BlackBerry Limited.
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

/*
 * features.cpp
 *
 *  Created on: 2013-04-25
 *      Author: Aram
 */
#include "features.hpp"

#include <bb/device/DisplayInfo>

Features::Features()
{}

Features::~Features()
{}

Features& Features::getInstance()
{
    static Features features;
    return features;
}

int Features::getDeviceWidth(){
	bb::device::DisplayInfo display;
	return display.pixelSize().width();
}

int Features::getDeviceHeight(){
	bb::device::DisplayInfo display;
	return display.pixelSize().height();
}

int Features::getTopActionBarHeight(){
	// TODO: Add code which returns required value based on device type
	return ACTION_BAR_HEIGHT;
}

int Features::getTopActionBarSquareButtonHeight(){
	// TODO: Add code which returns required value based on device type
	return ACTION_BAR_SQUARE_BUTTON_HEIGHT;
}

int Features::getTopActionBarSquareButtonWidth(){
	// TODO: Add code which returns required value based on device type
	return ACTION_BAR_SQUARE_BUTTON_WIDTH;
}

int Features::getSignInScreenBackgroundWidth(){
	return SIGNIN_SCREEN_BACKGROUND_WIDTH;
}

int Features::getSignInScreenBackgroundHeight(){
	return SIGNIN_SCREEN_BACKGROUND_HEIGHT;
}

