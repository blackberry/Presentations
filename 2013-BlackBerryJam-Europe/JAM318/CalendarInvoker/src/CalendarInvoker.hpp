// Default empty project template
#ifndef CalendarInvoker_HPP_
#define CalendarInvoker_HPP_

#include <QObject>
#include <bb/cascades/controls/button.h>
#include <bb/cascades/controls/label.h>
//#include <bb/system/CardDoneMessage.hpp>
#include <bb/system/InvokeManager.hpp>
#include <bb/pim/calendar/CalendarService.hpp>

namespace bb {
namespace cascades {
class Application;

/*!
 * @brief Application pane object
 *
 *Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class CalendarInvoker: public QObject {
Q_OBJECT
public:
	CalendarInvoker(bb::cascades::Application *app);
	virtual ~CalendarInvoker() {
	}

public slots:

	void onChildCardDone(const bb::system::CardDoneMessage &message);
	void onPickerButtonClicked();
	void onComposerButtonClicked();
	void onViewerButtonClicked();

private:

	Label* _result;
	bb::system::InvokeManager* _invokeManager;
	bb::pim::calendar::CalendarService* _calendarService;

};
}
}

#endif /* CalendarInvoker_HPP_ */
