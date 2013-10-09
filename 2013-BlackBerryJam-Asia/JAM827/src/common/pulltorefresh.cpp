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
#include "PullToRefresh.h"
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

using namespace bb::cascades;

PullToRefresh::PullToRefresh() :
        mThreshold(0), mControl(0), mRefreshActive(false), mToBeRefreshed(false)
{
    mActivityIndicator = ActivityIndicator::create().preferredSize(100, 100);

    mLabelLoading = Label::create("Loading...")
        .vertical(VerticalAlignment::Center).textStyle(SystemDefaults::TextStyles::bigText())
        .leftMargin(40);

    mActivityContainer = Container::create()
        .layout(StackLayout::create().orientation(LayoutOrientation::LeftToRight)).vertical(
            VerticalAlignment::Center).horizontal(HorizontalAlignment::Left).visible(false)
            .add(mActivityIndicator).add(mLabelLoading);

    mImage = ImageView::create("asset:///images/arrow_down.png").vertical(VerticalAlignment::Center);

    mLabel = Label::create(tr("Pull down to refresh")).vertical(VerticalAlignment::Center)
            .textStyle(SystemDefaults::TextStyles::bigText()).leftMargin(40);

    mInstContainer = Container::create()
        .layout(StackLayout::create().orientation(LayoutOrientation::LeftToRight))
        .vertical(VerticalAlignment::Center).horizontal(HorizontalAlignment::Left)
        .add(mImage).add(mLabel);

    Container * innerContainer = Container::create()
        .layout(StackLayout::create().orientation(LayoutOrientation::LeftToRight)).vertical(
            VerticalAlignment::Center).horizontal(HorizontalAlignment::Left).left(60).add(
            mInstContainer).add(mActivityContainer);

    mLayoutUpdateHandler = LayoutUpdateHandler::create(this);
    connect(mLayoutUpdateHandler, SIGNAL(layoutFrameChanged(const QRectF &)),
            this, SLOT(onLayoutFrameChanged(const QRectF &)));

    mRootContainer = Container::create().layout(DockLayout::create()).add(innerContainer)
            .vertical(VerticalAlignment::Fill).horizontal(HorizontalAlignment::Fill);

    setPreferredHeight(200);
    setRoot(mRootContainer);
}

PullToRefresh::~PullToRefresh()
{
    // TODO Auto-generated destructor stub
}

void PullToRefresh::onLayoutFrameChanged(const QRectF& layoutFrame)
{
    if (!mActivityIndicator->isRunning()) {
        if (layoutFrame.y() > 0) {
            mImage->setImageSource(QString("asset:///images/arrow_up.png"));
            mLabel->setText(tr("Release to refresh"));
            mToBeRefreshed = true;
        } else if (layoutFrame.y() < 0) {
            mImage->setImageSource(QString("asset:///images/arrow_down.png"));
            mLabel->setText(tr("Pull down to refresh"));
            mToBeRefreshed = false;
        }
        mInstContainer->setVisible(true);
        mActivityContainer->setVisible(false);
        mThreshold = 1 + layoutFrame.y() / 200;
        emit thresholdChanging(mThreshold);
    }
}

float PullToRefresh::threshold() const
{
    return mThreshold;
}

void PullToRefresh::onListViewTouch(bb::cascades::TouchEvent* event)
{
    if (mThreshold > 0.5 && event->isUp() == true && mToBeRefreshed) {
        mActivityIndicator->setRunning(true);
        mLabel->setText("Updating...");
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
    mControl = control;
    if (!connect(mControl, SIGNAL(touch(bb::cascades::TouchEvent*)),
            this, SLOT(onListViewTouch(bb::cascades::TouchEvent*)))) {
        qWarning() << "connectToTouchSignal failed";
    }
    emit controlChanged(mControl);
}

void PullToRefresh::refreshDone()
{
    FadeTransition *anim1 = FadeTransition().create()
            .to(0).duration(500).target(mLabelLoading).easingCurve(StockCurve::CubicOut).parent(this);
    FadeTransition *anim2 = FadeTransition().create()
            .to(0).duration(500).target(mActivityIndicator).easingCurve(StockCurve::CubicOut).parent(this);

    connect(anim2, SIGNAL(ended()), this, SLOT(endOfHideAnimation()));

    anim1->play();
    anim2->play();
}

void PullToRefresh::endOfHideAnimation()
{
    setPreferredHeight(0);
    QTimer::singleShot(100, this, SLOT(resetStates()));
}

void PullToRefresh::resetStates()
{
    mActivityIndicator->setRunning(false);
    mRefreshActive = false;
    mToBeRefreshed = false;
    mLabel->setText(tr("Pull down to refresh"));
    mActivityIndicator->setOpacity(1.0);
    mLabelLoading->setOpacity(1.0);
    setPreferredHeight(200);
}

bool PullToRefresh::refreshActive()
{
    return mRefreshActive;
}
