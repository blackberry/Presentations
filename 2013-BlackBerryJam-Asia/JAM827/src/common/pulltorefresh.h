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
#ifndef PULLTOREFRESH_H_
#define PULLTOREFRESH_H_
#include <bb/cascades/CustomControl>

namespace bb {
    namespace cascades {
        class ActivityIndicator;
        class Container;
        class LayoutUpdateHandler;
        class Label;
        class ImageView;
    }
}

class PullToRefresh: public bb::cascades::CustomControl
{
Q_OBJECT
    /**
     *
     */
    Q_PROPERTY(float threshold READ threshold NOTIFY thresholdChanging)
    Q_PROPERTY(bb::cascades::Control* control READ control WRITE setControl NOTIFY controlChanged)
    Q_PROPERTY(bool refreshActive READ refreshActive NOTIFY refreshActiveChanged)

public:
    PullToRefresh();
    virtual ~PullToRefresh();

    /**
     *
     */
    Q_INVOKABLE void refreshDone();

    /**
     *
     */
    Q_INVOKABLE bool refreshActive();

    float threshold() const;

    bb::cascades::Control* control();
    void setControl(bb::cascades::Control* control);

signals:
    void refreshActiveChanged(bool active);
    void controlChanged(bb::cascades::Control *control);
    void thresholdChanging(float value);

private slots:
    void onLayoutFrameChanged(const QRectF & layoutFrame);
    void onListViewTouch(bb::cascades::TouchEvent* event);
    void resetStates();
    void endOfHideAnimation();

private:

    float mThreshold;
    bb::cascades::Label * mLabelLoading;
    bb::cascades::Label * mLabel;
    bb::cascades::ImageView *mImage;
    bb::cascades::ActivityIndicator *mActivityIndicator;
    bb::cascades::Container *mRootContainer;
    bb::cascades::Container *mInstContainer;
    bb::cascades::Container *mActivityContainer;
    bb::cascades::LayoutUpdateHandler *mLayoutUpdateHandler;
    bb::cascades::Control *mControl;
    bool mRefreshActive;
    bool mToBeRefreshed;
};

#endif /* PULLTOREFRESH_H_ */
