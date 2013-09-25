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
 * utilities.h
 *
 *  Created on: May 5, 2013
 *      Author: macuser
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <QVariantList>

namespace bb { namespace cascades { class ArrayDataModel; } }

namespace Utilities
{

	/**
	 * Updates the content of a DataModel with the one in a QVariantList
	 */
	void updateDataModel(QVariantList data, bb::cascades::ArrayDataModel* dataModel);

}


#endif /* UTILITIES_H_ */
