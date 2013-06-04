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
#ifndef SCORECOUNTER_H_
#define SCORECOUNTER_H_

#include <QtDeclarative>

/**
 * Forward declarations
 */
class HighscoreDataModel;

class ScoreCounter: public QObject
{
    Q_OBJECT
    Q_PROPERTY(int seconds READ seconds WRITE setSeconds NOTIFY secondsChanged)
    Q_PROPERTY(int moves READ moves WRITE setMoves NOTIFY movesChanged)

public:
    ScoreCounter(HighscoreDataModel* dataModel);
    virtual ~ScoreCounter();

    void saveScoreToHighscore(const QString& name, int level);Q_INVOKABLE
    void updateHighscoreName(const QString& name);

    void startTimer();
    void stopTimer();Q_SLOT
    void onTimerTrigger();

    void addMove();
    void clearScore();

    int seconds();
    void setSeconds(int sec);Q_SIGNAL
    void secondsChanged(int);

    int moves();
    void setMoves(int mov);Q_SIGNAL
    void movesChanged(int);

private:
    ScoreCounter();

    HighscoreDataModel* m_pModel;

    QTimer* timer;
    int mSeconds;
    int mMoves;

};

#endif /* SCORECOUNTER_H_ */
