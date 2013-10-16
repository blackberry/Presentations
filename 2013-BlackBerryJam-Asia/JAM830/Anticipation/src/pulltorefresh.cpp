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
#include <bb/cascades/ActivityIndicator>
#include <bb/cascades/Color>
#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/FadeTransition>
#include <bb/cascades/ImageView>
#include <bb/cascades/Label>
#include <bb/cascades/LayoutUpdateHandler>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StockCurve>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TouchType>

using namespace bb::cascades;

PullToRefresh::PullToRefresh(bb::cascades::Container* parent) :
        CustomControl(parent), mControl(0), mRefreshActive(false), mToBeRefreshed(false)
{
    // Adding the visual components to the custom control, an activity indicator
    // shown when the control is in its refreshActive state, a label with a status text,
    // and an arrow image used to reflect if further pulling is needed in order to trigger the refresh.
    mActivityIndicator = ActivityIndicator::create().preferredSize(100, 100);

    mLabelLoading = Label::create("Loading...").vertical(VerticalAlignment::Center).textStyle(
            SystemDefaults::TextStyles::bigText()).leftMargin(40);

    mActivityContainer = Container::create().layout(
            StackLayout::create().orientation(LayoutOrientation::LeftToRight)).vertical(
            VerticalAlignment::Center).horizontal(HorizontalAlignment::Left).visible(false).add(
            mActivityIndicator).add(mLabelLoading);

    mImage = ImageView::create("asset:///images/arrow_down.png").vertical(
            VerticalAlignment::Center);

    mLabel = Label::create(tr("Pull to refresh")).vertical(VerticalAlignment::Center).textStyle(
            SystemDefaults::TextStyles::bigText()).leftMargin(40);

    mInstContainer = Container::create().layout(
            StackLayout::create().orientation(LayoutOrientation::LeftToRight)).vertical(
            VerticalAlignment::Center).horizontal(HorizontalAlignment::Left).add(mImage).add(
            mLabel);

    Container * innerContainer = Container::create().layout(
            StackLayout::create().orientation(LayoutOrientation::LeftToRight)).vertical(
            VerticalAlignment::Center).horizontal(HorizontalAlignment::Left).left(60).add(
            mInstContainer).add(mActivityContainer);

    mRootContainer = Container::create().layout(DockLayout::create()).add(innerContainer).vertical(
            VerticalAlignment::Fill).horizontal(HorizontalAlignment::Fill);

    setRoot(mRootContainer);

    // In order to track how much of the Control is visible on screen we
    // create a layout update handler and connect to the signal for layout
    // frame changes.
    mLayoutUpdateHandler = LayoutUpdateHandler::create(this);
    connect(mLayoutUpdateHandler, SIGNAL(layoutFrameChanged(const QRectF &)),
            SLOT(onLayoutFrameChanged(const QRectF &)));
}

PullToRefresh::~PullToRefresh()
{
}

void PullToRefresh::onLayoutFrameChanged(const QRectF& layoutFrame)
{
    // If the Activity indicator is not running it means that the user
    // hasn't yet triggered a refresh (not pulled down and released).
    if (!mActivityIndicator->isRunning()) {
        if (layoutFrame.y() > 0) {

            // A y position larger then 0 indicates that the entire Control
            // is on screen, that is the user has pulled far enough so that
            // if released the refresh signal should be emitted.
            mImage->setRotationZ(180);
            mLabel->setText(tr("Release to refresh"));
            mToBeRefreshed = true;

        } else if (layoutFrame.y() < 0) {

            // While the position of the Control is less then zero part
            // of the Control is off the screen and the user can still decide
            // to not trigger a refresh of the list.
            mImage->setRotationZ(0);
            mLabel->setText(tr("Pull to refresh"));
            mToBeRefreshed = false;

        }

        mInstContainer->setVisible(true);
        mActivityContainer->setVisible(false);
    }
}

void PullToRefresh::onListViewTouch(bb::cascades::TouchEvent* event)
{
    if (event->touchType() == TouchType::Up && mToBeRefreshed) {

        // If the scrollable control that this custom control monitors is released,
        // while mToBeRefreshed is true (the entire Control is showing) update the UI
        // to show a loading state and emit a signal that its time to refresh (get more data).
        mActivityIndicator->setRunning(true);
        mLabel->setText(tr("Updating..."));
        mInstContainer->setVisible(false);
        mActivityContainer->setVisible(true);
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
    if (!connect(mControl, SIGNAL(touch(bb::cascades::TouchEvent*)),
            SLOT(onListViewTouch(bb::cascades::TouchEvent*)))) {
        qWarning() << "connectToTouchSignal failed";
    }
    emit controlChanged(mControl);
}

void PullToRefresh::refreshDone()
{
    // Set up the reset animations that will hide the Containers with the label and the Activity indicator.
    FadeTransition *anim1 = FadeTransition().create().to(0).duration(500).target(mLabelLoading).easingCurve(
                    StockCurve::CubicOut).parent(this);
    FadeTransition *anim2 = FadeTransition().create().to(0).duration(500).target(mActivityIndicator).easingCurve(
                    StockCurve::CubicOut).parent(this);

    // Connect to the ended signal of one of the animations in order to
    // reset the the control and then play them.
    connect(anim2, SIGNAL(ended()), this, SLOT(resetStates()));
    anim1->play();
    anim2->play();
}

void PullToRefresh::resetStates()
{
    // Reset all the states of the control.
    mActivityIndicator->setRunning(false);
    mRefreshActive = false;
    mToBeRefreshed = false;
    mLabel->setText(tr("Pull to refresh"));
    mActivityIndicator->setOpacity(1.0);
    mLabelLoading->setOpacity(1.0);
    emit refreshActiveChanged(mRefreshActive);
}

bool PullToRefresh::refreshActive()
{
    return mRefreshActive;
}
