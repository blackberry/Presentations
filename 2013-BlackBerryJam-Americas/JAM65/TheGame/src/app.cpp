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
#include <bb/cascades/Application>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/OrientationSupport>
#include <bb/cascades/Label>
#include <bb/cascades/DockLayout>
#include <bb/cascades/ActivityIndicator>
#include <bb/cascades/Container>
#include <bb/cascades/ImagePaint>
#include <bb/cascades/Page>
#include "gamemodelview.h"
#include "app.hpp"
#include "highscoredatamodel.h"
#include "gamemanager.h"
#include "soundmanager.h"
#include "settings.h"
#include "scorecounter.h"

using namespace bb::cascades;
using namespace bb::system;

App::App() :
        m_pHighscoreDataModel(0), m_pGameManager(0), m_pScoreCounter(0), m_pSoundManager(0), m_pSettings(
                0), m_fullscreenMode(true)
{

    Page* page = Page::create();
    Container* cont = Container::create();
    cont->setLayout(DockLayout::create());
    cont->setBackground(ImagePaint(QUrl("asset:///images/bg.png")));
    ActivityIndicator* activity = ActivityIndicator::create();
    activity->setPreferredHeight(200);
    activity->setPreferredWidth(200);
    activity->setVerticalAlignment(VerticalAlignment::Center);
    activity->setHorizontalAlignment(HorizontalAlignment::Center);
    cont->add(activity);
    activity->start();
    page->setContent(cont);
    Application::instance()->setScene(page);

    QTimer::singleShot(2, this, SLOT(startApp()));

}

void App::startApp()
{
    qmlRegisterType<GameModelView>("thegame", 1, 0, "GameModelView");

    QmlDocument *qml = QmlDocument::create("asset:///main.qml");
    setContextProperties(qml);
    QObject::connect(Application::instance(), SIGNAL(thumbnail()), this, SLOT(thumbnail()));
    QObject::connect(Application::instance(), SIGNAL(fullscreen()), this, SLOT(fullscreen()));

    AbstractPane *root = qml->createRootObject<AbstractPane>();
    OrientationSupport::instance()->setSupportedDisplayOrientation(
            SupportedDisplayOrientation::All);
    Application::instance()->setScene(root);

    m_pInvokeManager = new InvokeManager(this);

    m_pSceneCover = NULL;
    m_pUIToolkitSupport = new bb::UIToolkitSupport(this);
    connect(m_pUIToolkitSupport, SIGNAL(uiStateChanged (bb::ProcessUIState::Type , const QString&)),
            this, SLOT(onUiStateChanged (bb::ProcessUIState::Type , const QString&)));

}

void App::setContextProperties(QmlDocument* qml)
{
    if (!m_pHighscoreDataModel) {
        m_pHighscoreDataModel = new HighscoreDataModel();
        m_pHighscoreDataModel->setParent(this); // needed so that the list does not take ownership of the datamodel and removes it on page-pop
    }
    if (!m_pScoreCounter) {
        m_pScoreCounter = new ScoreCounter(m_pHighscoreDataModel);
        m_pScoreCounter->setParent(this);
    }
    if (!m_pGameManager) {
        m_pGameManager = &GameManager::getInstance();
        m_pGameManager->setScoreCounter(m_pScoreCounter);
    }
    if (!m_pSettings) {
        m_pSettings = new Settings();
        m_pSettings->setParent(this);
    }
    if (!m_pSoundManager) {
        m_pSoundManager = new SoundManager(m_pSettings);
        m_pSoundManager->setParent(this);
    }

    qml->setContextProperty("_HighscoreDataModel", m_pHighscoreDataModel);
    qml->setContextProperty("gameManager", m_pGameManager);
    qml->setContextProperty("cs", this);
    qml->setContextProperty("_ScoreCounter", m_pScoreCounter);
    qml->setContextProperty("soundManager", m_pSoundManager);
    qml->setContextProperty("settings", m_pSettings);
}

App::~App()
{
}

void App::thumbnail()
{
    m_fullscreenMode = false;
    emit fullscreenChanged(m_fullscreenMode);
    //todo mute music and fx if playing
}

void App::fullscreen()
{
    m_fullscreenMode = true;
    emit fullscreenChanged(m_fullscreenMode);
    //todo unmute music and fx if it should be playing
}

bool App::fullscreenMode()
{
    return m_fullscreenMode;
}

void App::createWinnerScrapCard()
{
    InvokeRequest cardRequest;
    cardRequest.setTarget("scrapcard.target");
    cardRequest.setAction("bb.action.CREATE");
    cardRequest.setMimeType("application/custom");
    qDebug() << "score" << m_pScoreCounter->seconds();
    cardRequest.setData(
            QByteArray().append(
                    QString(m_pSettings->getValueFor("player_name", "playerName")).append(",")).append(
                    QString::number(m_pScoreCounter->seconds())));

    m_pInvokeManager->invoke(cardRequest);
}

void App::onUiStateChanged(bb::ProcessUIState::Type newState, const QString &windowGroupID)
{
    Q_UNUSED(windowGroupID);
    if (newState == bb::ProcessUIState::Thumbnailed) {
        qDebug() << "on thumbnail";

        if (!m_pSceneCover) {
            QmlDocument *qmlDoc = QmlDocument::create("asset:///GameCover.qml");
            setContextProperties(qmlDoc);
            Control *sceneCoverRoot = qmlDoc->createRootObject<Control>();
            m_pCoverTitle = sceneCoverRoot->findChild<Label*>(QString("coverTitle"));
            m_pCoverPlayerName = sceneCoverRoot->findChild<Label*>(QString("coverPlayerName"));
            m_pCoverScore = sceneCoverRoot->findChild<Label*>(QString("coverScore"));
            m_pSceneCover = SceneCover::create().content(sceneCoverRoot);
            Application::instance()->setCover(m_pSceneCover);
        }
        if (m_pGameManager->gameInProgress()) {
            m_pScoreCounter->stopTimer();
            m_pCoverTitle->setText("Ongoing game");
            m_pCoverPlayerName->setText(m_pSettings->getValueFor("player_name", "playerName"));
            m_pCoverScore->setText(
                    "Moves: " + QString::number(m_pScoreCounter->moves()) + "   \n   Time: "
                            + QString::number(m_pScoreCounter->seconds()) + " seconds");

        } else {
            m_pCoverTitle->setText("No game \n in progress");
            m_pScoreCounter->clearScore();
            m_pCoverPlayerName->setText("");
            m_pCoverScore->setText("");
        }
    } else if (newState == bb::ProcessUIState::FullScreen) {
        if (m_pGameManager->gameInProgress()) {
            m_pScoreCounter->startTimer();
        }
    }
}

