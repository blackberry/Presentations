/* Copyright (c) 2013 BlackBerry Limited.
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
#ifndef LOADMODELDECORATOR_HPP_
#define LOADMODELDECORATOR_HPP_

#include <bb/cascades/datamanager/DataModelDecorator>

class LoadModelDecorator: public bb::cascades::datamanager::DataModelDecorator
{
    Q_OBJECT

    Q_PROPERTY(bool loadingOldItems READ loadingOldItems WRITE setLoadingOldItems NOTIFY loadingOldItemsChanged FINAL)

    Q_PROPERTY(bool showDecorator READ showDecorator WRITE setShowDecorator NOTIFY showDecoratorChanged FINAL)

public:
    LoadModelDecorator();
    virtual ~LoadModelDecorator();

    Q_INVOKABLE virtual QVariant data(const QVariantList &indexPath);
    Q_INVOKABLE virtual int childCount(const QVariantList& indexPath);
    Q_INVOKABLE virtual bool hasChildren(const QVariantList& indexPath);
    Q_INVOKABLE virtual QString itemType(const QVariantList &indexPath);

    bool loadingOldItems() const;
    Q_SLOT void setLoadingOldItems(bool value);

    bool showDecorator() const;
    Q_SLOT void setShowDecorator(bool value);

    Q_INVOKABLE void temporaryHideDecorator();

signals:
    void loadingOldItemsChanged(bool value);
    void showDecoratorChanged(bool value);

private slots:
    void showAfterDelay();
private:
    int mChildCount;
    bool mLoadingOldItems;
    bool mShowDecorator;
};

#endif /* LOADMODELDECORATOR_HPP_ */
