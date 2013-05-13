// Default empty project template
#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>

#include "Photo3DView.hpp"
#include "OpenGLThread.hpp"

namespace bb { namespace cascades { class Application; }}

/*!
 * @brief Application pane object
 *
 *Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class ApplicationUI : public QObject
{
    Q_OBJECT
public:
    ApplicationUI(bb::cascades::Application *app);
    virtual ~ApplicationUI() {}

public Q_SLOTS:
	void shutdown();

private:
	//NavigationPane  *m_navPane;
	Photo3DView *m_pphoto3DView_HDMI;
};


#endif /* ApplicationUI_HPP_ */
