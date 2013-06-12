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
#ifndef GAMEMODELVIEW_H_
#define GAMEMODELVIEW_H_

#include <QObject>

#include <bb/cascades/CustomControl>

/**
 * Forward declarations
 */
namespace bb
{
    namespace cascades
    {
        class Container;
    }
}
class Block;
class ScoreCounter;
class GameManager;

class GameModelView: public bb::cascades::CustomControl
{
Q_OBJECT

public:
    GameModelView();
    int BLOCK_SIZE;

    virtual ~GameModelView();
    void getMoveBounds(Block *block, float& lowerBound, float& upperBound);
    void updateMask();
    void loadNewModelLevel();
    bool isGameOver();
    void addMove();

    Q_SLOT
    void onStartGame();Q_SLOT
    void onGameOver();Q_SLOT
    void onLoadNextLevel();

private:
    void createVisualForBlock(Block *block);

    ScoreCounter *m_pScoreCounter;
    bb::cascades::Container *m_pRootContainer;
    GameManager &m_pGameManager;
    QList<Block*> m_listOfBlocks;
    int mask[6][6];

};

#endif /* GAMEMODELVIEW_H_ */
