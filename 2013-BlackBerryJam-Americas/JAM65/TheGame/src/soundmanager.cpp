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
#include "soundmanager.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <QDir>
#include <QMap>
#include <QVariantMap>
#include <QList>
#include <qdebug.h>
#include <AL/al.h>
#include <AL/alut.h>
#include <bb/data/JsonDataAccess>

#include "settings.h"

bool SoundManager::qStringToBool(QString val)
{
    if (val == "true") {
        return true;
    } else {
        return false;
    }
}

QString SoundManager::boolToQString(bool val)
{
    if (val) {
        return "true";
    } else {
        return "false";
    }
}

// Error message function for ALUT.
static void reportALUTError()
{
    qDebug() << "ALUT reported the following error: " << alutGetErrorString(alutGetError());
}

// Error message function for OpenAL.
static void reportOpenALError()
{
    qDebug() << "OpenAL reported the following error: \n" << alutGetErrorString(alGetError());
}

SoundManager::SoundManager(Settings* settings) :
        mSettings(settings)
{

    // setting up music and fx categorys
    mFxCategory.mName = "fx";
    mFxCategory.mGain = -1;
    mFxCategory.mSource = 1;

    mMusicCategory.mName = "music";
    mMusicCategory.mGain = -1;
    mMusicCategory.mSource = 2;

    // TODO: Replace with Json file

    bb::data::JsonDataAccess jda;
    QVariant jsonData = jda.load("app/native/assets/sounds/sounds.json");
    QVariantList songList = jsonData.value<QVariantList>();

    QVariantMap item;

    for (int i = 0; i < songList.size(); i++) {
        item = songList[i].value<QVariantMap>();

        Sound* s = new Sound();
        s->mName = item["name"].value<QString>();
        s->mFileName = item["fileName"].value<QString>();
        s->mBufferID = ALuint(0);

        if (item["category"].value<QString>() == "music") {
            s->mCategory = &mMusicCategory;
        } else {
            s->mCategory = &mFxCategory;
        }
        mSounds.append(s);
    }

    //songsDataSet->setRoot(songList);

    /*
     mSounds.append(new Sound("bonus", "sounds/bonus.wav", ALuint(0), &mFxCategory));
     mSounds.append(new Sound("klick1", "sounds/klick1.wav", ALuint(0), &mFxCategory));
     mSounds.append(new Sound("klick2", "sounds/klick2.wav", ALuint(0), &mFxCategory));
     mSounds.append(new Sound("song1", "sounds/bonus.wav", ALuint(0), &mMusicCategory));
     mSounds.append(new Sound("song2", "sounds/bonus.wav", ALuint(0), &mMusicCategory));
     */

    QString applicationDirectory;
    char cwd[PATH_MAX];
    ALuint bufferID;

    // Initialize ALUT.
    if (alutInit(NULL, NULL) == false) {
        reportALUTError();
    }

    // Get the complete application directory in which we will load sounds from.
    // We convert to QString since it is more convenient when working with directories.
    getcwd(cwd, PATH_MAX);
    applicationDirectory = QString(cwd);
    applicationDirectory.append("/app/native/assets/");

    // create and assing an bufferID to all sounds.
    for (int i = 0; i < mSounds.size(); i++) {
        QString file = applicationDirectory;
        file.append(mSounds[i]->mFileName);

        QFileInfo fileInfo = file;
        bufferID = alutCreateBufferFromFile(fileInfo.absoluteFilePath().toStdString().c_str());

        if (alutGetError() == ALUT_ERROR_NO_ERROR) {
            mSounds[i]->mBufferID = bufferID;
        } else {
            reportALUTError();
        }
    }

    // Generate a number of sources used to attach buffers and play.
    alGenSources(SOUNDMANAGER_MAX_NBR_OF_SOURCES, soundSources);

    if (alGetError() != AL_NO_ERROR) {
        reportOpenALError();
    }

    setFxGain(mSettings->getValueFor("fxGain", "0.75").toFloat());
    initFxMute(qStringToBool(mSettings->getValueFor("fxMute", "false")));

    setMusicGain(mSettings->getValueFor("musicGain", "0.5").toFloat());
    initMusicMute(qStringToBool(mSettings->getValueFor("musicMute", "false")));

}

SoundManager::~SoundManager()
{
    ALuint bufferID = 0;
    int source = 0;

    // Clear all the sources.
    for (source = 0; source < SOUNDMANAGER_MAX_NBR_OF_SOURCES; source++) {
        alDeleteSources(1, &soundSources[source]);

        if (alGetError() != AL_NO_ERROR) {
            reportOpenALError();
        }
    }

    // Clear buffers, iterate through the hash

    QListIterator<Sound*> iterator(mSounds);

    while (iterator.hasNext()) {
        // Get the buffer id and delete it.
        bufferID = iterator.next()->mBufferID;

        if (bufferID != 0) {
            alDeleteBuffers(1, &bufferID);

            if (alGetError() != AL_NO_ERROR) {
                reportOpenALError();
            }
        }
    }

    // Clear the List for sound buffer id's.
    mSounds.clear();

    // Exit ALUT.
    if (alutExit() == false) {
        reportALUTError();
    }
}

int SoundManager::getSoundIndex(QString name)
{

    for (int i = 0; i <= mSounds.size() - 1; i++) {

        if (mSounds[i]->mName == name) {
            return i;
        }
    }
    return -1;
}

bool SoundManager::play(QString name)
{
//    static int source = 0;
//    int avilableSource = source % SOUNDMANAGER_MAX_NBR_OF_SOURCES;

    int index = getSoundIndex(name);
    int avilableSource = mSounds[index]->mCategory->mSource;

    // Get the corresponding buffer id set up in the init function.
    ALuint bufferID = mSounds[getSoundIndex(name)]->mBufferID;

    if (bufferID != 0) {

        // Attach the buffer to an available source.
        alSourcei(soundSources[avilableSource], AL_BUFFER, bufferID);

        if (alGetError() != AL_NO_ERROR) {
            reportOpenALError();
            return false;
        }

        // Play the source.
        alSourcePlay(soundSources[avilableSource]);

        if (alGetError() != AL_NO_ERROR) {
            reportOpenALError();
            return false;
        }

        // Increment which source we are using.
        //source++;

    } else {
        // The buffer was not found.
        return false;
    }

    return true;
}

float SoundManager::fxGain()
{
    return mFxCategory.mGain;
}

void SoundManager::setFxGain(float gain)
{
    if (gain != mFxCategory.mGain) {
        mFxCategory.mGain = gain;
        alSourcef(soundSources[mFxCategory.mSource], AL_GAIN, gain);

        if (alGetError() != AL_NO_ERROR) {
            reportOpenALError();
        } else {
            mSettings->saveValueFor("fxGain", QString::number(gain));
            emit fxGainChanged(gain);
        }
    }
}

bool SoundManager::fxMute()
{
    return mFxCategory.mMute;
}

void SoundManager::setFxMute(bool mute)
{
    if (mute != mFxCategory.mMute) {
        mFxCategory.mMute = mute;
        if (mute) {
            alSourcef(soundSources[mFxCategory.mSource], AL_GAIN, 0);
        } else {
            alSourcef(soundSources[mFxCategory.mSource], AL_GAIN, mFxCategory.mGain);
        }

        if (alGetError() != AL_NO_ERROR) {
            reportOpenALError();
        } else {
            mSettings->saveValueFor("fxMute", boolToQString(mute));
            emit fxMuteChanged(mute);
        }
    }
}

// only used in initalization
void SoundManager::initFxMute(bool mute)
{
    mFxCategory.mMute = mute;
    if (mute) {
        alSourcef(soundSources[mFxCategory.mSource], AL_GAIN, 0);
    } else {
        alSourcef(soundSources[mFxCategory.mSource], AL_GAIN, mFxCategory.mGain);
    }
    if (alGetError() != AL_NO_ERROR) {
        reportOpenALError();
    } else {
        mSettings->saveValueFor("fxMute", boolToQString(mute));
    }
}

float SoundManager::musicGain()
{
    return mMusicCategory.mGain;
}

void SoundManager::setMusicGain(float gain)
{
    if (gain != mMusicCategory.mGain) {
        mMusicCategory.mGain = gain;
        alSourcef(soundSources[mMusicCategory.mSource], AL_GAIN, mMusicCategory.mGain);

        if (alGetError() != AL_NO_ERROR) {
            reportOpenALError();
        } else {
            mSettings->saveValueFor("musicGain", QString::number(gain));
            emit musicGainChanged(gain);
        }
    }
}

bool SoundManager::musicMute()
{
    return mMusicCategory.mMute;
}

void SoundManager::setMusicMute(bool mute)
{
    if (mute != mMusicCategory.mMute) {
        mMusicCategory.mMute = mute;
        if (mute) {
            alSourcef(soundSources[mMusicCategory.mSource], AL_GAIN, 0);
        } else {
            alSourcef(soundSources[mMusicCategory.mSource], AL_GAIN, mMusicCategory.mGain);
        }

        if (alGetError() != AL_NO_ERROR) {
            reportOpenALError();
        } else {
            mSettings->saveValueFor("musicMute", boolToQString(mute));
            emit musicMuteChanged(mute);
        }
    }
}

// only used in initialization
void SoundManager::initMusicMute(bool mute)
{
    mMusicCategory.mMute = mute;
    if (mute) {
        alSourcef(soundSources[mMusicCategory.mSource], AL_GAIN, 0);
    } else {
        alSourcef(soundSources[mMusicCategory.mSource], AL_GAIN, mMusicCategory.mGain);
    }

    if (alGetError() != AL_NO_ERROR) {
        reportOpenALError();
    } else {
        mSettings->saveValueFor("musicMute", boolToQString(mute));
    }
}
