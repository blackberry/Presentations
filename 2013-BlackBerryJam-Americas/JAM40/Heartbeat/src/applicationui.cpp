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
#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/ForeignWindowControl>

using namespace bb::cascades;

#ifndef QT_NO_DEBUG
#define DEBUG_LOGGING
#endif

#ifdef DEBUG_LOGGING
#define DEBUG_STDERR
#endif

#ifdef DEBUG_STDERR
static void log_to_stderr(QtMsgType msgType, const char *msg)
{
    (void)msgType;  // go away, warning!
    fprintf(stderr, "%s\n", msg);
}
#endif

#define LOG(format, args...) do { doDebugLog(__PRETTY_FUNCTION__, __LINE__, format, ##args); } while(0)
void doDebugLog(const char* func, int line, const char* format, ...) __attribute__((format (printf, 3, 4)));

#define DEBUGLOGLEN 1024

void doDebugLog(const char* func, int line, const char* format, ...)
{
#ifdef DEBUG_LOGGING
    va_list arg;
    char buf[DEBUGLOGLEN];
    int olderrno = errno;
    va_start(arg, format);
    vsnprintf(buf, sizeof(buf), format, arg);
    va_end(arg);
    qDebug("%s(%d): %s", func, line, buf);
    errno = olderrno;
#endif
}


// some defines for my arbitrary heuristic algorithm.
// if you have more time, you should consider using an FFT for this sort of thing.
#define DARK_THRESH 15
#define DARK_DEBOUNCE 10
#define AVG_FRAMECOUNT 20
#define BEAT_THRESH (-3.0f)
#define BEAT_DEBOUNCE 2
#define AVG_BEATCOUNT 5


ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
        QObject(app),
        mStarted(false),
        mHandle(CAMERA_HANDLE_INVALID),
        mAvgRate(0.0f)
{
#ifdef DEBUG_STDERR
    qInstallMsgHandler(log_to_stderr);
#endif

    // Create scene document from main.qml asset, the parent is set
    // to ensure the document gets destroyed properly at shut down.
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    qml->setContextProperty("Heartbeat", this);

    // Create root object for the UI
    AbstractPane* root = qml->createRootObject<AbstractPane>();
    mRootObj = root;
    // Set created root object as the application scene
    app->setScene(root);

    LOG("startup");
}


ApplicationUI::~ApplicationUI()
{
    stopCamera();
}


void
ApplicationUI::onWindowAttached(screen_window_t win,
                                const QString &group,
                                const QString &id)
{
    (void)group;
    (void)id;
    LOG("window attached!");
    // put the viewfinder window behind the cascades window
    int i = -1;
    screen_set_window_property_iv(win, SCREEN_PROPERTY_ZORDER, &i);
    screen_context_t screen_ctx;
    screen_get_window_property_pv(win, SCREEN_PROPERTY_CONTEXT, (void **)&screen_ctx);
    screen_flush_context(screen_ctx, 0);
}


bool
ApplicationUI::startCamera()
{
    ForeignWindowControl* fwc = mRootObj->findChild<ForeignWindowControl*>("vfWindow");
    if (!fwc) {
        LOG("failed to find FWC");
        return false;
    }

    int err = camera_open(CAMERA_UNIT_REAR, CAMERA_MODE_RW, &mHandle);
    if (err) {
        LOG("failed to open camera: %d", err);
        return false;
    }
    // find smallest available resolution, since we only care about the illumination level of the image.
    unsigned int num = 0;
    camera_get_video_vf_resolutions(mHandle, 0, &num, NULL);
    if (!num) {
        LOG("failed to query video vf resolutions");
    } else {
        camera_res_t res[num];
        camera_get_video_vf_resolutions(mHandle, num, &num, res);
        int smallest = 0;
        for (unsigned int i=1; i<num; i++) {
            if (res[i].width * res[i].height < res[smallest].width * res[smallest].height) {
                smallest = i;
            }
        }
        mWidth = res[smallest].width;
        mHeight = res[smallest].height;
        LOG("selecting resolution: %d x %d", mWidth, mHeight);
        QByteArray groupBA = fwc->windowGroup().toLocal8Bit();
        err = camera_set_videovf_property(mHandle,
                                          CAMERA_IMGPROP_WIN_GROUPID, groupBA.data(),
                                          CAMERA_IMGPROP_WIN_ID, "viewfinder",
                                          CAMERA_IMGPROP_WIDTH, mWidth,
                                          CAMERA_IMGPROP_HEIGHT, mHeight);
        if (err) {
            LOG("failed to configure videovf: %d", err);
        } else {
            err = camera_set_video_property(mHandle,
                                            CAMERA_IMGPROP_WIDTH, mWidth,
                                            CAMERA_IMGPROP_HEIGHT, mHeight);
            if (err) {
                LOG("failed to configure video: %d", err);
            } else {
                err = camera_start_video_viewfinder(mHandle,
                                                    vfCallbackEntry,
                                                    NULL,
                                                    (void*)this);
                if (err) {
                    LOG("failed to start viewfinder");
                } else {
                    mLedOn = false;
                    emit avgrateChanged(mAvgRate=0.0f);
                    emit startedChanged(mStarted=true);
                    return true;
                }
            }
        }
    }
    camera_close(mHandle);
    mHandle = CAMERA_HANDLE_INVALID;
    return false;
}


void
ApplicationUI::stopCamera()
{
    if (mStarted) {
        camera_config_videolight(mHandle, CAMERA_VIDEOLIGHT_OFF);
        camera_stop_video_viewfinder(mHandle);
        camera_close(mHandle);
        mHandle = CAMERA_HANDLE_INVALID;
        emit startedChanged(mStarted=false);
        emit avgrateChanged(mAvgRate=0.0f);
    }
}


void
ApplicationUI::vfCallbackEntry(camera_handle_t handle, camera_buffer_t* buf, void* arg)
{
    (void)handle;
    ((ApplicationUI*)arg)->vfCallback(buf);
}


void
ApplicationUI::vfCallback(camera_buffer_t* buf)
{
    if (buf->frametype != CAMERA_FRAMETYPE_NV12) {
        LOG("unexpected frametype: %d", buf->frametype);
        return;
    }
    static int darkcount = DARK_DEBOUNCE;
    static float avg = 0;
    // let's compute brightness
    uint64_t sum = 0ll;
    unsigned int w = buf->framedesc.nv12.width;
    unsigned int h = buf->framedesc.nv12.height;
    unsigned int s1 = buf->framedesc.nv12.stride;
    for (unsigned int y = 0; y<h; y++) {
        uint8_t* ptr = &(buf->framebuf[y * s1]);
        for (unsigned int x = 0; x<w; x++) {
            sum += ptr[x];
        }
    }
    sum = sum / (w*h);
    //LOG("scene brightness: %llu", sum);

    // turn led on if it has been dark long enough (eg. camera is covered)
    if (!mLedOn) {
        if (sum < DARK_THRESH) {
            if (darkcount) {
                if (!--darkcount) {
                    LOG("dark for %d frames.  turning LED on.", DARK_THRESH);
                    camera_config_videolight(mHandle, CAMERA_VIDEOLIGHT_ON);
                    mLedOn = true;
                }
            }
        } else {
            darkcount = DARK_DEBOUNCE;
        }
    } else {
        // if time permits, I will revisit this code and apply an FFT instead of this
        // heuristic algorithm.
        // In case you are wondering what the algorithm currently does, it's approximately:
        // -compute a running brightness average (weighted over 20 frames)
        // -check for difference in brightness between each frame and the average
        // -when difference is large enough, provisionally declare a "beat"
        // -if the provisional beat is held for at least 2 frames (to rule out false positives), then declare it a real "beat"
        // -calculate time since last beat
        // -blend this time into a running heartrate average (5-beat weighted average)
        // -display the rate on-screen

        // led is on.. let's start averaging (20 frames)
        float afc = AVG_FRAMECOUNT;
        float cur = sum;
        avg = avg * (afc-1)/afc;
        avg += cur / afc;
        float delta = cur-avg;
        static float lastdelta = 0;
        static int beatdebounce = BEAT_DEBOUNCE;
        bool beat = delta < BEAT_THRESH;
        //LOG("AVG: %f   DELTA: %f   DELTA': %f   BEAT: %s", avg, delta, delta-lastdelta, beat?"*":" ");
        lastdelta = delta;
        if (beat) {
            if (beatdebounce) {
                if (!--beatdebounce) {
                    LOG("BEAT DETECTED");
                    static uint64_t t_last = 0ll;
                    uint64_t t_cur;
                    ClockTime(CLOCK_MONOTONIC, NULL, &t_cur);
                    if (t_last > 0ll) {
                        float period = (t_cur - t_last) / 1000000ll;
                        float rate = 0;
                        if (period != 0) {
                            rate = 60000 / period;
                        }
                        float abc = AVG_BEATCOUNT;
                        mAvgRate = mAvgRate * (abc-1)/abc;
                        mAvgRate += rate / abc;
                        LOG("BEAT PERIOD: %f ms  RATE: %f bpm AVERAGE: %f bpm", period, rate, mAvgRate);
                        emit avgrateChanged(mAvgRate);
                    }
                    t_last = t_cur;
                }
            }
        } else {
            beatdebounce = BEAT_DEBOUNCE;
        }

    }

}
