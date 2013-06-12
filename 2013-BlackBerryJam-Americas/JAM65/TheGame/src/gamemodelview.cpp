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
#include "gamemodelview.h"
#include <bb/cascades/AbsoluteLayout>
#include <bb/cascades/AbsoluteLayoutProperties>
#include <bb/cascades/ImageView>
#include <bb/cascades/core/touchtype.h>
#include <bb/cascades/core/touchbehavior.h>
#include <bb/cascades/core/touchreaction.h>
#include <bb/data/JsonDataAccess>
#include <bb/cascades/Container>

#include "block.h"
#include "scorecounter.h"
#include "gamemanager.h"

using namespace bb::cascades;
using namespace bb::data;

GameModelView::GameModelView() :
        m_pGameManager(GameManager::getInstance())
{
    // TODO Auto-generated constructor stub
    BLOCK_SIZE = 120;

    m_pRootContainer = Container::create().preferredSize(720.0f, 720.0f).layout(
            AbsoluteLayout::create());
    setRoot(m_pRootContainer);
    m_pScoreCounter = m_pGameManager.getScoreCounter();
    QObject::connect(&m_pGameManager, SIGNAL(started()), this, SLOT(onStartGame()));
    QObject::connect(&m_pGameManager, SIGNAL(gameOver()), this, SLOT(onGameOver()));
    QObject::connect(&m_pGameManager, SIGNAL(loadNextLevel()), this, SLOT(onLoadNextLevel()));
}

GameModelView::~GameModelView()
{

}

void GameModelView::loadNewModelLevel()
{
    m_pRootContainer->removeAll();
    for (int i = 0; i < m_listOfBlocks.size(); ++i) {
        Block *block = m_listOfBlocks.at(i);
        delete block;
    }
    m_listOfBlocks.clear();
    QString levelFile("app/native/assets/levels/level");
    levelFile.append(QString::number(GameManager::getInstance().currentLevel())).append(".json");
    JsonDataAccess jda;
    QVariant jsonData = jda.load(levelFile);
    QVariantList outerList = jsonData.value<QVariantList>();
    for (int i = 0; i < outerList.size(); i++) {
        QVariantList innerList = outerList.at(i).value<QVariantList>();
        int rowStartIndex = innerList.at(0).toInt(); //rowStartIndex;
        int colStartIndex = innerList.at(1).toInt(); //colStartIndex
        int numOfCols = innerList.at(2).toInt(); //numOfCols
        int numOfRows = innerList.at(3).toInt(); //numOfRows
        bool specialBlock = innerList.at(4).toBool(); // is special block
        Block *block = new Block(this, //pointer to gamemodelview
                rowStartIndex, colStartIndex, numOfCols, numOfRows, specialBlock);

        createVisualForBlock(block);
        m_listOfBlocks.append(block);
        m_pRootContainer->add(block->m_pBlockVisual);
    }
}

void GameModelView::onStartGame()
{
    m_pScoreCounter->clearScore();
    m_pScoreCounter->startTimer();
    m_pGameManager.notifyResetLevel();
    loadNewModelLevel();
    updateMask();
}

void GameModelView::onGameOver()
{
    m_pScoreCounter->stopTimer();
    m_pScoreCounter->saveScoreToHighscore("playerName", m_pGameManager.currentLevel());
    //animate tortoise out of screen
}

void GameModelView::onLoadNextLevel()
{
    m_pScoreCounter->clearScore();
    m_pScoreCounter->startTimer();
    loadNewModelLevel();
    updateMask();
}

bool GameModelView::isGameOver()
{
    for (int i = 0; i < m_listOfBlocks.size(); ++i) {
        Block *block = m_listOfBlocks.at(i);
        if (block->m_specialBlock == true) {
            if (block->m_colStartIndex == 4) {
                return true;
            }
        }
    }
    return false;
}

void GameModelView::createVisualForBlock(Block *block)
{
    QString imagePath("asset:///images/block_");
    if (block->m_specialBlock == false) {
        imagePath.append(QString::number(block->m_numOfCols)).append("x");
        imagePath.append(QString::number(block->m_numOfRows));
    } else {
        imagePath.append("main");
    }
    imagePath.append(".png");

    block->m_prevTranslationX = block->m_colStartIndex * BLOCK_SIZE;
    block->m_prevTranslationY = block->m_rowStartIndex * BLOCK_SIZE;
    block->m_currentTranslationX = block->m_prevTranslationX;
    block->m_currentTranslationY = block->m_prevTranslationY;

    //TODO: modify later to load the qml item
    block->m_pBlockVisual = ImageView::create(imagePath).translate(block->m_prevTranslationX,
            block->m_prevTranslationY);
    block->m_pBlockVisual->addTouchBehavior(
            TouchBehavior::create().addTouchReaction(TouchType::Down, PropagationPhase::AtTarget,
                    TouchResponse::StartTracking));
    block->m_pImplicitAnimationController = ImplicitAnimationController::create(
            block->m_pBlockVisual).enabled(false);
    QObject::connect(block->m_pBlockVisual, SIGNAL(touch(bb::cascades::TouchEvent*)), block,
            SLOT(onTouch(bb::cascades::TouchEvent*)));

}
void GameModelView::addMove()
{
    m_pScoreCounter->addMove();
}

void GameModelView::updateMask()
{
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            mask[i][j] = 0;
        }
    }
    for (int i = 0; i < m_listOfBlocks.size(); ++i) {
        Block *block = m_listOfBlocks.at(i);
        if (block->isHorizontal()) {
            for (int j = block->m_colStartIndex; j < (block->m_colStartIndex + block->m_numOfCols);
                    ++j) {
                mask[block->m_rowStartIndex][j] = 1;
            }
        } else {
            for (int j = block->m_rowStartIndex; j < (block->m_rowStartIndex + block->m_numOfRows);
                    ++j) {
                mask[j][block->m_colStartIndex] = 1;
            }
        }
    }
}

void GameModelView::getMoveBounds(Block *touchedBlock, float& lowerBound, float& upperBound)
{

    if (touchedBlock->isHorizontal()) {
        int lowerCount = 0;
        lowerBound = touchedBlock->m_colStartIndex * BLOCK_SIZE;
        upperBound = touchedBlock->m_colStartIndex * BLOCK_SIZE;
        for (int i = touchedBlock->m_colStartIndex - 1; i >= 0; i--) {
            if (mask[touchedBlock->m_rowStartIndex][i] == 0) {
                lowerCount++;
            } else {
                break;
            }
        }
        if (lowerCount != 0) {
            lowerBound = (touchedBlock->m_colStartIndex - lowerCount) * BLOCK_SIZE;
        }

        int upperCount = 0;
        for (int i = touchedBlock->m_colStartIndex + touchedBlock->m_numOfCols; i < 6; i++) {
            if (mask[touchedBlock->m_rowStartIndex][i] == 0) {
                upperCount++;
            } else {
                break;
            }
        }
        if (upperCount != 0) {
            upperBound = (touchedBlock->m_colStartIndex + upperCount) * BLOCK_SIZE;
        }
    } else {
        int lowerCount = 0;
        lowerBound = touchedBlock->m_rowStartIndex * BLOCK_SIZE;
        upperBound = touchedBlock->m_rowStartIndex * BLOCK_SIZE;
        for (int i = touchedBlock->m_rowStartIndex - 1; i >= 0; i--) {
            if (mask[i][touchedBlock->m_colStartIndex] == 0) {
                lowerCount++;
            } else {
                break;
            }
        }
        if (lowerCount != 0) {
            lowerBound = (touchedBlock->m_rowStartIndex - lowerCount) * BLOCK_SIZE;
        }

        int upperCount = 0;
        for (int i = touchedBlock->m_rowStartIndex + touchedBlock->m_numOfRows; i < 6; i++) {
            if (mask[i][touchedBlock->m_colStartIndex] == 0) {
                upperCount++;
            } else {
                break;
            }
        }
        if (upperCount != 0) {
            upperBound = (touchedBlock->m_rowStartIndex + upperCount) * BLOCK_SIZE;
        }

    }
}

