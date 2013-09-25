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

#ifndef FEATURES_HPP_
#define FEATURES_HPP_

#include <QObject>

// ACTION bar
#define ACTION_BAR_HEIGHT 100
#define ACTION_BAR_SQUARE_BUTTON_HEIGHT 80
#define ACTION_BAR_SQUARE_BUTTON_WIDTH 80

// SIGN IN screen
#define SIGNIN_SCREEN_BACKGROUND_WIDTH 728
#define SIGNIN_SCREEN_BACKGROUND_HEIGHT 828

/*
 * This class was introduced to handle device specific data.
 * Eg: device height, width, etc.
 * */
class Features: public QObject {
    Q_OBJECT
public:
    // Returns instance of this class
    static Features& getInstance();
    virtual ~Features();

    // This function returns current device width
	Q_INVOKABLE
	int getDeviceWidth();

	// This function returns current device height
	Q_INVOKABLE
	int getDeviceHeight();

	// This function returns top bar height
	Q_INVOKABLE
	int getTopActionBarHeight();

	// This function returns buttons height which are on action bar
	Q_INVOKABLE
	int getTopActionBarSquareButtonHeight();
	// This function returns buttons width which are on action bar
	Q_INVOKABLE
	int	getTopActionBarSquareButtonWidth();

	// This function returns signin screen background width
	Q_INVOKABLE
	int getSignInScreenBackgroundWidth();

	// This function returns signin screen background height
	Q_INVOKABLE
	int getSignInScreenBackgroundHeight();

private:
    Features();
};

#endif /* FEATURES_HPP_ */
