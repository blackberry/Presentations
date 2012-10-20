// Default empty project template
#include "CalendarInvoker.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/system/InvokeManager.hpp>
#include <bb/system/InvokeReply.hpp>
#include <bb/PpsObject>
#include <qdebug.h>;
#include <bb/system/CardDoneMessage.hpp>

using namespace bb::cascades;
using namespace bb::system;
using namespace bb::pim::calendar;

CalendarInvoker::CalendarInvoker(bb::cascades::Application *app) :
		QObject(app), _invokeManager(new InvokeManager(this)), _calendarService(
				new CalendarService()) {
	// create scene document from main.qml asset
	// set parent to created document to ensure it exists for the whole application lifetime
	QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

	// create root object for the UI
	AbstractPane *root = qml->createRootObject<AbstractPane>();
	// set created root object as a scene
	app->setScene(root);

	Button *button;

	// Connect our buttons
	button = root->findChild<Button*>("pickerButton");
	QObject::connect(button, SIGNAL(clicked()), this,
			SLOT(onPickerButtonClicked()));

	button = root->findChild<Button*>("composerButton");
	QObject::connect(button, SIGNAL(clicked()), this,
			SLOT(onComposerButtonClicked()));

	button = root->findChild<Button*>("viewerButton");
	QObject::connect(button, SIGNAL(clicked()), this,
			SLOT(onViewerButtonClicked()));

	_result = root->findChild<Label*>("result");

	QObject::connect(_invokeManager,
			SIGNAL(childCardDone(const bb::system::CardDoneMessage &)), this,
			SLOT(onChildCardDone(const bb::system::CardDoneMessage &)));
}

/***
 * Open the event picker card
 *
 * This will invoke the calendar, and when it returns the
 * onChildCardDone slot will trigger, which will invoke the
 * calendar again, displaying the event
 ***/
void CalendarInvoker::onPickerButtonClicked() {
	InvokeRequest invokeRequest;
	invokeRequest.setTarget("sys.pim.calendar.viewer.nav");
	invokeRequest.setAction("bb.calendar.PICK");
	invokeRequest.setMimeType("text/calendar");

	InvokeTargetReply *invokeReply = _invokeManager->invoke(invokeRequest);
}

/***
 * Invokes the event creator card
 *
 * You can prepopulate the event details if you like
 */
void CalendarInvoker::onComposerButtonClicked() {
	InvokeRequest cardRequest;

	QVariantList participants = QVariantList();
	participants.append("bob@g.com");
	participants.append("vigandhi@rim.com");
	QVariantMap data = QVariantMap();
	data.insert(QString("participants"), QVariant(participants));
	data["subject"] = "test test";
	qDebug() << " your other data is " << data;
	cardRequest.setTarget("sys.pim.calendar.viewer.eventcreate");
	cardRequest.setAction("bb.action.CREATE");
	cardRequest.setMimeType("text/calendar");
	bool ok;
	QByteArray encData = PpsObject::encode(data, &ok);
	qDebug() << encData;
	cardRequest.setData(encData);
	InvokeTargetReply* reply = _invokeManager->invoke(cardRequest);
}


/***
 * Invoke the event viewer card
 *
 * You need to to actually know the event you want to view
 * for this to work
 */
void CalendarInvoker::onViewerButtonClicked() {
	qDebug() << "++++++ View start" << endl;

	int accountID;
	int eventID;
	//Get the event details from somewhere, for example
	//data["accountId"] = accountIdText->text().toInt(); //1338819432;
	//data["eventId"] = eventIdText->text().toInt(); //175
	//data["start"] = startTime->text(); //"2012-05-25 11:30:00.000"; this is needed to distinguish between recurring events, so you get the right one

	QVariantMap data;

	data["accountId"] = accountID;
	data["type"] = "event";
	data["eventId"] = eventID;
	data["start"] = "2012-05-25 11:30:00.000"; //this is needed to distinguish between recurring events, so you get the right one

	InvokeRequest cardRequest;
	cardRequest.setTarget("sys.pim.calendar.viewer.ics");
	cardRequest.setAction("bb.action.OPEN");
	cardRequest.setMimeType("text/calendar");

	bool ok;
	QByteArray encData = PpsObject::encode(data, &ok);

	if (ok) {
		cardRequest.setData(encData);
		InvokeTargetReply* reply = _invokeManager->invoke(cardRequest);
	}
	qDebug() << "++++++ View done" << endl;
}

/***
 * Handle the results of an InvokeRequest
 *
 * We are only using this for the viewer.
 * Currently the card doesn't return any standard control data on
 * the other cards, so we need to hack something up to check
 * for nothing.
 */
void CalendarInvoker::onChildCardDone(
		const bb::system::CardDoneMessage &message) {

	qDebug() << "+++++++ INVOKE: " << message.data() << endl;
	//iCalendarData cal = _calendarService->parseICalendarData()
	//qDebug() << "+++++++ Keys: " << data.keys() << endl;
	//qDebug() << "+++++++ Event ID: " << data["eventID"] << endl;
	//qDebug() << "+++++++ Start: " << data["start"] << endl;
	if (message.data().length() > 0 and message.data() != "string encoded data...") {
		QVariantMap data;
		data["accountId"] = 1; // default calendar account id
		data["icsbytes"] = QString(message.data());
		data["type"] = "ics";
		InvokeRequest invokeRequest;
		invokeRequest.setTarget("sys.pim.calendar.viewer.ics");
		invokeRequest.setAction("bb.calendar.OPEN");
		invokeRequest.setMimeType("text/calendar");
		invokeRequest.setData(bb::PpsObject::encode(data));

		_invokeManager->invoke(invokeRequest);
	}

}
