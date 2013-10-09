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
#include "loadmodeldecorator.h"

using namespace std;

LoadModelDecorator::LoadModelDecorator() :
        mChildCount(0), mLoadingOldItems(false), mShowDecorator(true) {

}

LoadModelDecorator::~LoadModelDecorator() {
}

QVariant LoadModelDecorator::data(const QVariantList& indexPath) {

	if (mShowDecorator && indexPath.at(0) == mChildCount - 1){
		QVariantMap map;
		map.insert("loading", QVariant::fromValue<bool>(mLoadingOldItems));
		return map;
	}

	QVariant data = bb::cascades::datamanager::DataModelDecorator::data(indexPath);
	return data;
}

int LoadModelDecorator::childCount(const QVariantList& indexPath) {
    mChildCount = bb::cascades::datamanager::DataModelDecorator::childCount(indexPath);

    // Add one to the child count if the decorator is to be shown.
    if(mShowDecorator) {
        mChildCount += 1;
    }
	return mChildCount;
}

bool LoadModelDecorator::hasChildren(const QVariantList& indexPath) {
	return bb::cascades::datamanager::DataModelDecorator::hasChildren(indexPath);
}

QString LoadModelDecorator::itemType(const QVariantList &indexPath) {
	if (indexPath.at(0) == mChildCount - 1) {
		return "last_item";
	} else {
		return "item";
	}
}

bool LoadModelDecorator::loadingOldItems() const {
	return mLoadingOldItems;
}

void LoadModelDecorator::setLoadingOldItems(bool value) {
    mLoadingOldItems = value;

	emit loadingOldItemsChanged(mLoadingOldItems);
	QVariantList indexPath;
	indexPath << mChildCount - 1;
	emit itemUpdated(indexPath);
}

void LoadModelDecorator::setShowDecorator(bool value)
{
    if(value != mShowDecorator) {
        mShowDecorator = value;

        if(mShowDecorator) {
            QVariantList indexPath;
            indexPath << mChildCount;
            emit itemAdded(indexPath);
        } else {
            QVariantList indexPath;
            indexPath << mChildCount - 1;
            emit itemRemoved(indexPath);
        }

        emit showDecoratorChanged(mShowDecorator);
    }
}

bool LoadModelDecorator::showDecorator() const {
    return mShowDecorator;
}

void LoadModelDecorator::showAfterDelay() {
    setShowDecorator(true);
}

void LoadModelDecorator::temporaryHideDecorator() {
    setShowDecorator(false);
    QTimer::singleShot(200, this, SLOT(showAfterDelay()));
}
