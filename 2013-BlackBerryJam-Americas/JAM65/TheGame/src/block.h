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
#ifndef BLOCK_H_
#define BLOCK_H_

#include <QObject>

#include <bb/cascades/ImplicitAnimationController>

/**
 * Forward declarations
 */
namespace bb
{
    namespace cascades
    {
        class ImageView;
        class TouchEvent;
    }
}
class GameModelView;

class Block: public QObject
{
Q_OBJECT
public:

    Block(GameModelView *pGameModelView, int rowStartIndex, int colStartIndex, int numOfCols,
            int numOfRows, bool specialBlock);
    virtual ~Block();
    bool isHorizontal();

    Q_SLOT
    void onTouch(bb::cascades::TouchEvent *event);

    float m_touchDownX;
    float m_touchDownY;
    float m_prevTranslationX;
    float m_prevTranslationY;
    float m_currentTranslationX;
    float m_currentTranslationY;
    float m_diffX;
    float m_diffY;

    int m_rowStartIndex;
    int m_colStartIndex;
    int m_numOfCols;
    int m_numOfRows;
    bool m_specialBlock;
    int BLOCK_SIZE;
    int GAME_WIDTH;
    float m_lowerBound;
    float m_upperBound;
    bool m_DidTouchDown; // fix for bug: move from empty space onto the item messes up. so check if move is not started without touch down

    bb::cascades::ImageView *m_pBlockVisual;
    bb::cascades::ImplicitAnimationController m_pImplicitAnimationController;
private:
    GameModelView *m_pGameModelView;

};

#endif /* BLOCK_H_ */
