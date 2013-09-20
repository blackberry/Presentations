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
 *//*
 * QmlTimer.cpp
 *
 *  Created on: Apr 16, 2013
 *      Author: mgoulet
 */

#include <QTimer>
#include "QmlTimer.h"

QmlTimer::QmlTimer(QObject* parent)
     : bb::cascades::CustomControl(),
     _timer(new QTimer(this))
{
    Q_UNUSED(parent);
    connect(_timer, SIGNAL(timeout()), this, SIGNAL(timeout()));
    setVisible(false);
}

bool QmlTimer::isActive()
{
    return _timer->isActive();
}

int QmlTimer::interval()
{
    return _timer->interval();
}

void QmlTimer::setInterval(int m_sec)
{
    // If the timer already has the specified interval, do nothing
    if (_timer->interval() == m_sec)
        return;

    // Otherwise, set the interval of the timer and emit the
    // intervalChanged() signal
    _timer->setInterval(m_sec);
    emit intervalChanged();
}

void QmlTimer::start()
{
    // If the timer has already been started, do nothing
    if (_timer->isActive())
        return;

    // Otherwise, start the timer and emit the activeChanged()
    // signal
    _timer->start();
    emit activeChanged();
}

void QmlTimer::stop()
{
    // If the timer has already been stopped, do nothing
    if (!_timer->isActive())
        return;

    // Otherwise, stop the timer and emit the activeChanged()
    // signal
    _timer->stop();
    emit activeChanged();
}




