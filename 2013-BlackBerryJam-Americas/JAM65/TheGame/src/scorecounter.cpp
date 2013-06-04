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
#include "scorecounter.h"

#include "highscoredatamodel.h"

ScoreCounter::ScoreCounter(HighscoreDataModel* dataModel) :
        m_pModel(dataModel), mSeconds(0), mMoves(0)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerTrigger()));
}

ScoreCounter::~ScoreCounter()
{
}

void ScoreCounter::startTimer()
{
    timer->start(1000);
}

void ScoreCounter::stopTimer()
{
    timer->stop();
}

void ScoreCounter::onTimerTrigger()
{
    mSeconds++;
    emit secondsChanged(mSeconds);
}

void ScoreCounter::addMove()
{
    mMoves++;
    emit movesChanged(mMoves);
}

int ScoreCounter::seconds()
{
    return mSeconds;
}

void ScoreCounter::setSeconds(int sec)
{
    if (sec != mSeconds) {
        mSeconds = sec;
        emit secondsChanged(sec);
    }
}

int ScoreCounter::moves()
{
    return mMoves;
}

void ScoreCounter::setMoves(int mov)
{
    if (mov != mMoves) {
        mMoves = mov;
        emit movesChanged(mov);
    }
}

void ScoreCounter::clearScore()
{
    stopTimer();
    setMoves(0);
    setSeconds(0);
}

void ScoreCounter::saveScoreToHighscore(const QString& name, int level)
{
    m_pModel->addItem(name, mSeconds * 1000, mMoves, level);
}

void ScoreCounter::updateHighscoreName(const QString& name)
{
    m_pModel->updateLastSavedItem(name);
}
