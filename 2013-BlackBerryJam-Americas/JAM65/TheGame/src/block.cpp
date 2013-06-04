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
#include "block.h"

#include <bb/cascades/ImageView>
#include <bb/cascades/TouchEvent>

#include "gamemodelview.h"
#include "gamemanager.h"

Block::Block(GameModelView *pGameModelView, int rowStartIndex, int colStartIndex, int numOfCols,
        int numOfRows, bool specialBlock) :
        m_rowStartIndex(rowStartIndex), m_colStartIndex(colStartIndex), m_numOfCols(numOfCols), m_numOfRows(
                numOfRows), m_specialBlock(specialBlock), m_pGameModelView(pGameModelView)
{
    BLOCK_SIZE = 120;
    GAME_WIDTH = 600;

}

Block::~Block()
{

}

void Block::onTouch(bb::cascades::TouchEvent *event)
{

    if (event->isDown()) {
        m_pGameModelView->getMoveBounds(this, m_lowerBound, m_upperBound);
        m_DidTouchDown = true;
        m_touchDownX = event->screenX();
        m_touchDownY = event->screenY();
    } else if (event->isMove()) {
        if (m_DidTouchDown == false) {
            return;
        }
        m_diffX = event->screenX() - m_touchDownX;
        m_diffY = event->screenY() - m_touchDownY;
        m_currentTranslationX = m_prevTranslationX + m_diffX;
        m_currentTranslationY = m_prevTranslationY + m_diffY;

        //restrict to 0 if less than 0
        m_currentTranslationX = m_currentTranslationX < 0 ? 0 : m_currentTranslationX;
        m_currentTranslationY = m_currentTranslationY < 0 ? 0 : m_currentTranslationY;

        //restrict to GAME_WIDTH if greater than GAME_WIDTH
        m_currentTranslationX =
                m_currentTranslationX > GAME_WIDTH ? GAME_WIDTH : m_currentTranslationX;
        m_currentTranslationY =
                m_currentTranslationY > GAME_WIDTH ? GAME_WIDTH : m_currentTranslationY;

        if (isHorizontal()) {
            int mod = (int) m_currentTranslationX % BLOCK_SIZE;
            int div = (int) m_currentTranslationX / BLOCK_SIZE;
            int candidateColStartIndex;
            if (mod > 50) {
                candidateColStartIndex = div + 1;
            } else {
                candidateColStartIndex = div;
            }
            float candidateTranslationX = candidateColStartIndex * BLOCK_SIZE;
            if (candidateTranslationX >= m_lowerBound && candidateTranslationX <= m_upperBound) {
                event->target()->setTranslationX(m_currentTranslationX);
            }
        } else {
            int mod = (int) m_currentTranslationY % BLOCK_SIZE;
            int div = (int) m_currentTranslationY / BLOCK_SIZE;
            int candidateRowStartIndex;
            if (mod > 50) {
                candidateRowStartIndex = div + 1;
            } else {
                candidateRowStartIndex = div;
            }
            float candidateTranslationY = candidateRowStartIndex * BLOCK_SIZE;
            if (candidateTranslationY >= m_lowerBound && candidateTranslationY <= m_upperBound) {
                event->target()->setTranslationY(m_currentTranslationY);
            }
        }
    } else if (event->isUp()) {
        if (m_DidTouchDown == false) {
            return;
        }
        m_DidTouchDown = false;

        if (isHorizontal()) {
            int mod = (int) m_currentTranslationX % BLOCK_SIZE;
            int div = (int) m_currentTranslationX / BLOCK_SIZE;
            int candidateColStartIndex;
            if (mod > 50) {
                candidateColStartIndex = div + 1;
            } else {
                candidateColStartIndex = div;
            }
            float candidateTranslationX = candidateColStartIndex * BLOCK_SIZE;
            if (candidateTranslationX >= m_lowerBound && candidateTranslationX <= m_upperBound) {
                m_colStartIndex = candidateColStartIndex;
                m_currentTranslationX = m_colStartIndex * BLOCK_SIZE;
                m_prevTranslationX = m_currentTranslationX;
                event->target()->setTranslationX(m_currentTranslationX);
            } else if (candidateTranslationX < m_lowerBound) {
                m_colStartIndex = m_lowerBound / BLOCK_SIZE;
                m_currentTranslationX = m_colStartIndex * BLOCK_SIZE;
                m_prevTranslationX = m_currentTranslationX;
                event->target()->setTranslationX(m_currentTranslationX);
            } else {
                m_colStartIndex = m_upperBound / BLOCK_SIZE;
                m_currentTranslationX = m_colStartIndex * BLOCK_SIZE;
                m_prevTranslationX = m_currentTranslationX;
                event->target()->setTranslationX(m_currentTranslationX);
            }
        } else {
            int mod = (int) m_currentTranslationY % BLOCK_SIZE;
            int div = (int) m_currentTranslationY / BLOCK_SIZE;
            int candidateRowStartIndex;
            if (mod > 50) {
                candidateRowStartIndex = div + 1;
            } else {
                candidateRowStartIndex = div;
            }
            float candidateTranslationY = candidateRowStartIndex * BLOCK_SIZE;
            if (candidateTranslationY >= m_lowerBound && candidateTranslationY <= m_upperBound) {
                m_rowStartIndex = candidateRowStartIndex;
                m_currentTranslationY = m_rowStartIndex * BLOCK_SIZE;
                m_prevTranslationY = m_currentTranslationY;
                event->target()->setTranslationY(m_currentTranslationY);
            } else if (candidateTranslationY < m_lowerBound) {
                m_rowStartIndex = m_lowerBound / BLOCK_SIZE;
                m_currentTranslationY = m_rowStartIndex * BLOCK_SIZE;
                m_prevTranslationY = m_currentTranslationY;
                event->target()->setTranslationY(m_currentTranslationY);
            } else {
                m_rowStartIndex = m_upperBound / BLOCK_SIZE;
                m_currentTranslationY = m_rowStartIndex * BLOCK_SIZE;
                m_prevTranslationY = m_currentTranslationY;
                event->target()->setTranslationY(m_currentTranslationY);
            }
        }
        m_pGameModelView->updateMask();
        m_pGameModelView->addMove();
        if (m_pGameModelView->isGameOver()) {
            GameManager::getInstance().notifyGameOver();
        }

    }
}

bool Block::isHorizontal()
{
    if (m_numOfCols > m_numOfRows) {
        return true;
    } else {
        return false;
    }
}
