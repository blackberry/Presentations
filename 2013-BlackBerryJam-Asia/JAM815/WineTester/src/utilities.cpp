/*
 * Copyright 2013 BlackBerry Limited.
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

/*
 * utilities.cpp
 *
 *  Created on: May 5, 2013
 *      Author: macuser
 */

#include "utilities.h"

#include <bb/cascades/DataModel>
#include <bb/cascades/ArrayDataModel>

using namespace bb::cascades;

void Utilities::updateDataModel(QVariantList data, bb::cascades::ArrayDataModel* dataModel)
{
    int dataLength = data.length();

    for (int i = 0; i < dataLength; i++) {
        QVariant variant = data[i];

        int indexOf = dataModel->indexOf(variant);

        if (indexOf != -1) {
            dataModel->replace(i, variant);
        }
        else {
            dataModel->append(variant);
        }
    }
}
