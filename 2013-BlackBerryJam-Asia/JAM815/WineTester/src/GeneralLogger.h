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
 * GENERALLogging.h
 *
 *  Created on: Dec 19, 2012
 *      Author: mgoulet
 */

#ifndef GENERALLOGGING_H_
#define GENERALLOGGING_H_

#include <QDebug>
#include <QString>

#define __GENERAL_LOG_BASE(msg) qDebug() << __PRETTY_FUNCTION__ << QString(msg)

#ifdef QT_NO_DEBUG
	#define GENERAL_LOG(msg) __GENERAL_LOG_BASE(msg)
#else
	#include <stdio.h>
	#define GENERAL_LOG(msg) __GENERAL_LOG_BASE(msg); \
						  fprintf(stderr, "%s %s\n", __PRETTY_FUNCTION__, QString(msg).toStdString().c_str())
#endif

#endif /* GENERALLOGGING_H_ */
