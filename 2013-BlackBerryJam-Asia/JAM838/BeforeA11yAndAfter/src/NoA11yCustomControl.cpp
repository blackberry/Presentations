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
 * NoA11yCustomControl.cpp
 *
 */

#include "NoA11yCustomControl.h"

#include <bb/cascades/Container>
#include <bb/cascades/Label>
#include <bb/cascades/Color>
#include <bb/cascades/CustomA11yObject>

using namespace bb::cascades;

NoA11yCustomControl::NoA11yCustomControl() {
	Container* c  = Container::create();
	c->setBackground(Color::Yellow);
	c->setPreferredHeight(200.0f);
	c->setPreferredWidth(450.0f);

	Label* l = Label::create("Custom Control, but we do not want the screen reader to say it");
	l->setMultiline(true);
	c->add(l);

	//BAD: accessibility has not been set.  The developer has no control over
	//     how screen reader will interpret the components of this control.

	setRoot(c);
}

NoA11yCustomControl::~NoA11yCustomControl() {
}

