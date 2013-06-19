/* Copyright (c) 2013 Research In Motion Limited.
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
#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include <camera/camera_api.h>

namespace bb
{
    namespace cascades
    {
        class Application;
    }
}

/*!
 * @brief Application object
 *
 *
 */

class ApplicationUI: public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool started READ started NOTIFY startedChanged FINAL);
    Q_PROPERTY(float avgrate READ avgrate NOTIFY avgrateChanged FINAL);

Q_SIGNALS:
    void startedChanged(bool value);
    void avgrateChanged(float value);

public slots:
    void onWindowAttached(screen_window_t win,
                          const QString &group,
                          const QString &id);

public:
    ApplicationUI(bb::cascades::Application *app);
    virtual ~ApplicationUI();
    bool started() const { return mStarted; }
    float avgrate() const { return mAvgRate; }

    Q_INVOKABLE
    bool startCamera();

    Q_INVOKABLE
    void stopCamera();

    static void vfCallbackEntry(camera_handle_t handle, camera_buffer_t* buf, void* arg);
    void vfCallback(camera_buffer_t* buf);

private:
    bool mStarted;
    camera_handle_t mHandle;
    int mWidth;
    int mHeight;
    QObject* mRootObj;
    bool mLedOn;
    float mAvgRate;
};

#endif /* ApplicationUI_HPP_ */
