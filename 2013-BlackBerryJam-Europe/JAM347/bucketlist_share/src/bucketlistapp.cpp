/* Copyright (c) 2012 Research In Motion Limited.
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
#include "bucketlistapp.h"
#include "bucketdata/bucketmodel.h"
#include "bucketdata/bucketsettings.h"
#include "bucketbbm/bucketbbmmanager.h"

#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/system/InvokeManager>
#include <bb/system/InvokeRequest>
#include <bb/cascades/Page>

using namespace bb::cascades;
using namespace bucketbbm;

BucketListApp::BucketListApp()
{

	m_invokeManager = new bb::system::InvokeManager(this);

	QObject::connect(m_invokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)),
			this, SLOT(handleInvoke(const bb::system::InvokeRequest&)));

	m_bucketItemTitle = "something";

    // Set the application organization and name, which is used by QSettings
    // when saving values to the persistent store.
    QCoreApplication::setOrganizationName("Example");
    QCoreApplication::setApplicationName("Bucket List Settings");

    // The model for populating the bucket list is registered, so that it and all its
    // properties can be accessed directly from QML. This is done before creating the
    // QmlDocument below so that it is available when the corresponding QML component
    // is needed (see main.qml).
    qmlRegisterType<BucketModel>("com.bucketlist.bucketdata", 1, 0, "BucketModel");

    // The application settings object used to store the BBM connection state
    qmlRegisterType<BucketSettings>("com.bucketlist.bucketdata", 1, 0, "BucketSettings");

    // The BBM manager that can connect the application to BBM and update the BBM status message
    qmlRegisterType<BucketBBMManager>("com.bucketlist.bucketbbm", 1, 0, "BucketBBMManager");

    // Create a QMLDocument and load it, using build patterns.
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    // Make this object available to the UI as context property
	qml->setContextProperty("_theApp", this);

    if (!qml->hasErrors()) {

        AbstractPane *appPage = qml->createRootObject<AbstractPane>();
/*
        //Connect the signal and slots used when invoked through a .buk file.
        Page *page = appPage->findChild<Page*>("bucketListPage");

        QObject::connect(this,
              SIGNAL(newBucketListItem(QString)),
              page,
              SLOT(onNewBucketListItem(QString)));
*/

        if (appPage) {
            // Set the main scene to the application Page.
            Application::instance()->setScene(appPage);
        }
    }
}

void BucketListApp::handleInvoke(const bb::system::InvokeRequest& invoke)
{
	//Grab the .buk file we were invoked with.
	QUrl uri = invoke.uri();

	QFile file(uri.toLocalFile());

	m_bucketItemTitle = "";

	if(file.open(QIODevice::ReadOnly))
	{
		QTextStream in(&file);
		while(!in.atEnd())
		{
			m_bucketItemTitle += in.readLine();
		}
		file.close();
	}

	emit incomingBucketItem();

}

QString BucketListApp::bucketItemTitle() const
{
	return m_bucketItemTitle;
}

