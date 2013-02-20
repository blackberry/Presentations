/*
 * Copyright (c) 2013 Research In Motion.
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
#ifndef JAM321_HPP_
#define JAM321_HPP_

#include <QObject>
#include <QTimer>

#include "graphics/scope.hpp"
#include "log/GateFactory.hpp"
#include "log/Gate.hpp"
#include "bt/BlueToothManager.hpp"

namespace bb {
namespace cascades {
class Application;
}
}

/*!
 * @brief Application pane object
 *
 *Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class JAM321: public QObject, public GateFactory {
Q_OBJECT
public:
	JAM321(bb::cascades::Application *app, Scope * scope, BlueToothManager * btm);
	virtual ~JAM321();

public slots:
	void onRandomTimeout();
private:
	Scope * scope;
	QTimer * timer;
	BlueToothManager * blueToothManager;
};

#endif /* JAM321_HPP_ */
