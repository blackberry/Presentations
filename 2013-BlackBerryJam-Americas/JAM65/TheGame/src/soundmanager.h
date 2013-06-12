/*
 * Copyright (c) 2011, 2012 Research In Motion Limited.
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
#ifndef _SOUNDMANAGER_H
#define _SOUNDMANAGER_H

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include <QtCore/qstring.h>
#include <qhash.h>

#include "app.hpp"

/**
 * Forward declarations
 */
class Settings;

/**
 * Sound structs
 */

// The number of max number of sound sources.
#define SOUNDMANAGER_MAX_NBR_OF_SOURCES 32

struct Category
{
    QString mName;
    float mGain;
    bool mMute;
    ALuint mSource;
};

struct Sound
{
    QString mName;
    QString mFileName;
    Category* mCategory;
    ALuint mBufferID;
};

class SoundManager: public QObject
{
Q_OBJECT

Q_PROPERTY (float fxGain READ fxGain WRITE setFxGain NOTIFY fxGainChanged)
Q_PROPERTY (bool fxMute READ fxMute WRITE setFxMute NOTIFY fxMuteChanged)
Q_PROPERTY (float musicGain READ musicGain WRITE setMusicGain NOTIFY musicGainChanged)
Q_PROPERTY (bool musicMute READ musicMute WRITE setMusicMute NOTIFY musicMuteChanged)

public:
    SoundManager(Settings* settings);
    virtual ~SoundManager();

    Q_INVOKABLE
    bool play(QString name);

    float fxGain();
    void setFxGain(float gain);

    bool fxMute();
    void setFxMute(bool mute);

    float musicGain();
    void setMusicGain(float gain);

    bool musicMute();
    void setMusicMute(bool mute);

signals:
    void fxGainChanged(float gain);
    void fxMuteChanged(bool mute);
    void musicGainChanged(float gain);
    void musicMuteChanged(bool mute);

private:
    int getSoundIndex(QString name);
    void initFxMute(bool mute);
    void initMusicMute(bool mute);

    Settings* mSettings;
    Category mFxCategory;
    Category mMusicCategory;

    // all sounds stored in the list
    QList<Sound*> mSounds;

    // Sound buffers.
    QHash<QString, ALuint> mSoundBuffers;

    // Sound sources.
    ALuint soundSources[SOUNDMANAGER_MAX_NBR_OF_SOURCES];

    bool qStringToBool(QString val);
    QString boolToQString(bool val);
};

#endif //_SOUNDMANAGER_H
