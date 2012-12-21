// Default empty project template
#ifndef CameraInvoker_HPP_
#define CameraInvoker_HPP_

#include <QObject>
#include <bb/cascades/controls/label.h>;
#include <bb/system/InvokeManager.hpp>

namespace bb { namespace cascades { class Application; }}

/*!
 * @brief Application pane object
 *
 *Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class CameraInvoker : public QObject
{
    Q_OBJECT
public:
    CameraInvoker(bb::cascades::Application *app);
    virtual ~CameraInvoker() {}

public slots:
	void onChildCardDone(const bb::system::CardDoneMessage
			&message);
	void onInvokeButtonClicked();

private:
	bb::system::InvokeManager *_invokeManager;
	bb::cascades::Label *_resultsLabel;
};


#endif /* CameraInvoker_HPP_ */
