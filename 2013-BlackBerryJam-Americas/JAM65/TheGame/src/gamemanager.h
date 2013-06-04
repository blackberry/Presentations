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
#ifndef GAMEMANAGER_H_
#define GAMEMANAGER_H_

#include <QObject>

/**
 * Forward declarations
 */
class ScoreCounter;
class QStateMachine;
class QState;

class GameManager: public QObject
{
Q_OBJECT

public:
    static GameManager& getInstance()
    {
        static GameManager instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

    Q_INVOKABLE
    void notifyGameStart();Q_INVOKABLE
    void notifyGameOver();Q_INVOKABLE
    void notifyLoadNextLevel();Q_INVOKABLE
    void notifyResetLevel();Q_INVOKABLE
    void notifyResetGame();Q_INVOKABLE
    void notifyGameCancelled();

    void setScoreCounter(ScoreCounter* counter);
    ScoreCounter* getScoreCounter();

    Q_PROPERTY(int currentLevel READ currentLevel WRITE setLevel RESET resetLevel NOTIFY currentLevelChanged FINAL)
    ;Q_PROPERTY(bool gameInProgress READ gameInProgress NOTIFY gameInProgressChanged FINAL)
    ;

    int currentLevel();
    void setLevel(int level);Q_INVOKABLE
    void resetLevel();
    bool gameInProgress();

    Q_SIGNAL
    void started();Q_SIGNAL
    void gameOver();Q_SIGNAL
    void loadNextLevel();Q_SIGNAL
    void resetGame();Q_SIGNAL
    void currentLevelChanged();Q_SIGNAL
    void gameInProgressChanged();Q_SIGNAL
    void gameCancelled();

private:
    GameManager();
    virtual ~GameManager();
    GameManager(GameManager const&); // Don't Implement
    void operator=(GameManager const&); // Don't implement

    QStateMachine *m_pGameStateMachine;
    QState *m_pGamePlayingState;
    QState *m_pGameOverState;
    QState *m_pInitialState;
    int m_currentLevel;
    int MAX_LEVELS;

    ScoreCounter* m_pScoreCounter;

};

#endif /* GAMEMANAGER_H_ */
