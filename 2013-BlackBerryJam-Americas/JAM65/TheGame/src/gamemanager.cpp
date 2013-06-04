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
#include "gamemanager.h"

#include "scorecounter.h"
#include <QStateMachine>

GameManager::GameManager() :
        m_pScoreCounter(0)
{
    m_currentLevel = 1;
    MAX_LEVELS = 4;
    //create state machine and the states
    m_pGameStateMachine = new QStateMachine();
    m_pInitialState = new QState();
    m_pGamePlayingState = new QState();
    m_pGameOverState = new QState();

    //add states to the state machine
    m_pGameStateMachine->addState(m_pInitialState);
    m_pGameStateMachine->addState(m_pGamePlayingState);
    m_pGameStateMachine->addState(m_pGameOverState);

    m_pGameStateMachine->setInitialState(m_pInitialState);

    //define transition between states
    m_pGameOverState->addTransition(this, SIGNAL(started()), m_pGamePlayingState);
    m_pInitialState->addTransition(this, SIGNAL(started()), m_pGamePlayingState);
    m_pInitialState->addTransition(this, SIGNAL(loadNextLevel()), m_pGamePlayingState);
    m_pGameOverState->addTransition(this, SIGNAL(loadNextLevel()), m_pGamePlayingState);
    m_pGamePlayingState->addTransition(this, SIGNAL(gameOver()), m_pGameOverState);
    m_pGamePlayingState->addTransition(this, SIGNAL(gameCancelled()), m_pInitialState);
    m_pGameStateMachine->start();
}

GameManager::~GameManager()
{
    // TODO Auto-generated destructor stub
}

int GameManager::currentLevel()
{
    return m_currentLevel;
}

void GameManager::notifyGameStart()
{
    emit started();
}

void GameManager::notifyGameOver()
{
    emit gameOver();
}

void GameManager::notifyResetLevel()
{
    emit resetLevel();
}

void GameManager::notifyResetGame()
{
    emit resetGame();
}

void GameManager::notifyGameCancelled()
{
    emit gameCancelled();
    m_pScoreCounter->clearScore();
}

void GameManager::notifyLoadNextLevel()
{
    if (m_currentLevel >= MAX_LEVELS) {
        setLevel(1);
    } else {
        setLevel(m_currentLevel + 1);
    }
    emit loadNextLevel();
}

void GameManager::resetLevel()
{
    m_currentLevel = 1;
}

void GameManager::setLevel(int level)
{
    m_currentLevel = level;
    emit currentLevelChanged();
}

void GameManager::setScoreCounter(ScoreCounter* counter)
{
    m_pScoreCounter = counter;
}

ScoreCounter* GameManager::getScoreCounter()
{
    return m_pScoreCounter;
}

bool GameManager::gameInProgress()
{
    return m_pGameStateMachine->configuration().contains(m_pGamePlayingState);
}

