// Default empty project template
#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>

using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application *app)
: QObject(app)
{
	Application::instance()->setAutoExit(false);

	QObject::connect(
			Application::instance(),
			SIGNAL(manualExit()),
			this,
			SLOT(shutdown()));

   // create scene document from main.qml asset
    // set parent to created document to ensure it exists for the whole application lifetime
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

	OpenGLView::setRenderingAPI(GL_ES_1);

	//m_pphoto3DView = new Photo3DView(DISPLAY_DEVICE);
	m_pphoto3DView_HDMI = new Photo3DView(DISPLAY_HDMI);

	qml->setContextProperty("_app", this);
	//qml->setContextProperty("_photo3D", m_pphoto3DView);
	qml->setContextProperty("_photo3D_HDMI", m_pphoto3DView_HDMI);


    // create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();

	m_pphoto3DView_HDMI->setPosition(0, 0);
	m_pphoto3DView_HDMI->setSize(1920, 1080);
	m_pphoto3DView_HDMI->setDisplayMode(VIEW_3D_TYPE_TOP_BOTTOM_LEFT_RIGHT);
	m_pphoto3DView_HDMI->setLeftPhotoFilename("app/native/assets/images/DSC_0080.jpg");
	m_pphoto3DView_HDMI->setRightPhotoFilename("app/native/assets/images/DSC_0081.jpg");
	m_pphoto3DView_HDMI->setEnabled(true);
	m_pphoto3DView_HDMI->add();

    // set created root object as a scene
    app->setScene(root);
}

void ApplicationUI::shutdown() {
	qDebug()  << "ApplicationUI::shutdown";

	OpenGLView::shutdown();

	Application::instance()->quit();
}
