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
#ifndef APP_H
#define APP_H

#include <QtCore/QObject>
#include <QtCore/QMetaType>
#include <bb/system/InvokeManager>
#include <bb/UIToolkitSupport>
#include <bb/cascades/SceneCover>
/**
 * Forward declarations
 */
namespace bb
{
    namespace cascades
    {
        class QmlDocument;
        class Label;
    }
}

class SoundManager;
class HighscoreDataModel;
class GameManager;
class ScoreCounter;
class Settings;

class App: public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool fullscreenMode READ fullscreenMode NOTIFY fullscreenChanged)

public:
    App();
    virtual ~App();

    Q_SLOT
    void startApp();

    Q_SLOT
    void thumbnail();Q_SLOT
    void fullscreen();Q_INVOKABLE
    void createWinnerScrapCard();

    bool fullscreenMode();Q_SIGNAL
    void fullscreenChanged(bool);Q_SLOT
    void onUiStateChanged(bb::ProcessUIState::Type newState, const QString &windowGroupID);

private:
    void setContextProperties(bb::cascades::QmlDocument* qml);

    HighscoreDataModel* m_pHighscoreDataModel;
    GameManager* m_pGameManager;
    ScoreCounter* m_pScoreCounter;
    SoundManager* m_pSoundManager;
    Settings* m_pSettings;

    bool m_fullscreenMode;
    bb::system::InvokeManager *m_pInvokeManager;
    bb::cascades::SceneCover *m_pSceneCover;
    bb::UIToolkitSupport *m_pUIToolkitSupport;

    bb::cascades::Label *m_pCoverTitle;
    bb::cascades::Label *m_pCoverPlayerName;
    bb::cascades::Label *m_pCoverScore;
};

#endif // ifndef APP_H
