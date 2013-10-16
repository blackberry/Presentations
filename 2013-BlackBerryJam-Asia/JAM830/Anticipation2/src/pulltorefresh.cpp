/* Copyright (c) 2013 BlackBerry Limited.
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
#include "pulltorefresh.h"
#include <bb/cascades/LayoutUpdateHandler>
#include <bb/cascades/TouchType>

using namespace bb::cascades;

PullToRefresh::PullToRefresh(bb::cascades::Container* parent) :
        CustomControl(parent), mControl(0), mRefreshActive(false), mToBeRefreshed(false), mVisibleFraction(0.0)
{
    // In order to track how much of the Control is visible on screen we
    // create a layout update handler and connect to the signal for layout
    // frame changes.
    mLayoutUpdateHandler = LayoutUpdateHandler::create(this);
    connect(mLayoutUpdateHandler, SIGNAL(layoutFrameChanged(const QRectF &)), this,
            SLOT(onLayoutFrameChanged(const QRectF &)));

}

PullToRefresh::~PullToRefresh()
{
}

void PullToRefresh::onLayoutFrameChanged(const QRectF& layoutFrame)
{
    // As long as the Control is not in its loading state (refreshing),
    // we monitor its position relative the screen.
    if (!mRefreshActive) {
        if (layoutFrame.y() > 0) {

            // A y position larger then 0 indicates that the entire Control
            // is on screen, that is the user has pulled far enough so that
            // if released the refresh signal should be emitted.
            mToBeRefreshed = true;

        } else if (layoutFrame.y() < 0) {

            // While the position of the Control is less then zero part
            // of the Control is off the screen and the user can still decide
            // to not trigger a refresh of the list.
            mToBeRefreshed = false;
        }
        mVisibleFraction = (layoutFrame.height() + layoutFrame.y() )/ layoutFrame.height();
        emit visibleFractionChanged(mVisibleFraction);
    }
}

void PullToRefresh::onListViewTouch(bb::cascades::TouchEvent* event)
{
    if (event->touchType() == TouchType::Up && mToBeRefreshed) {

        // If the scrollable control that this custom control monitors is released,
        // while mToBeRefreshed is true (the entire Control is showing) update the UI
        // to show a loading state and emit a signal that its time to refresh (get more data).
        mRefreshActive = true;
        mToBeRefreshed = false;
        emit refreshActiveChanged(mRefreshActive);
    }
}

bb::cascades::Control* PullToRefresh::control()
{
    return mControl;
}

void PullToRefresh::setControl(bb::cascades::Control *control)
{
    // A control has been set connect to the touch signal in order
    // to see when the user release the control.
    mControl = control;
    if (!connect(mControl, SIGNAL(touch(bb::cascades::TouchEvent*)), this,
            SLOT(onListViewTouch(bb::cascades::TouchEvent*)))) {
        qWarning() << "connectToTouchSignal failed";
    }
    emit controlChanged(mControl);
}

void PullToRefresh::refreshDone()
{
    resetStates();
}

void PullToRefresh::resetStates()
{
    // Reset all the states of the control.
    mRefreshActive = false;
    mToBeRefreshed = false;
    emit refreshActiveChanged(mRefreshActive);
}

bool PullToRefresh::refreshActive()
{
    return mRefreshActive;
}

float PullToRefresh::visibleFraction()
{
    return mVisibleFraction;
}
