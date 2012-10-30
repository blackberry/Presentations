#ifndef VIEWER_HPP
#define VIEWER_HPP

#include <screen/screen.h>

#include "Target.hpp"

namespace bb {
    namespace cascades {
        class ForeignWindowControl;
    }
    namespace system {
        class InvokeViewerMessage;
        class InvokeViewerMessageReply;
    }
}

class Viewer : public Target
{
    Q_OBJECT

    /**
     * @brief Viewer's window id
     */
    Q_PROPERTY(QString windowId READ windowId NOTIFY windowIdChanged FINAL);

public:
    /**
     * Default constructor.
     */
    Viewer();

    /**
     * Constructor
     * Setup the foreign window and such.
     */
    Viewer(const QString &name, const QString &target, bb::system::InvokeManager *iMangaer, const QString &defaultAction = "bb.action.OPEN");

    /**
     * Destructor.
     * Clean up and whatnot.
     */
    virtual ~Viewer() {}

    bool operator==(const Viewer &other) const;

public Q_SLOTS:
    /**
     * Retrieve the window id for the viewer window.
     */
    QString windowId() const;

    /**
     * Start viewer and make visible.
     *
     * This function sends a invokeTargetViewer request to the navigator and waits for
     * a viewer window to attach to the Cascades main window.
     */
    void invoke();

    /**
     * Signal that we'd like to close the viewer.
     */
    void closeViewer();

    /**
     * Send message to viewer.
     */
    void sendMessageToViewer(const QString& name, const QString& data);

    /**
     * Send message for which a reply is required.
     */
    void sendQueryResponseMessage(const QString &name, const QString &data);

    /**
     * Reply to a query/response message from a viewer.
     */
    void reply(const QString &name, const QString &data);

    /**
     * Signals the end of the invocation.
     */
    void invokeFinished();

    /**
     * Signals a we've successfully queried for some data.
     */
    void queryFinished();

    /**
     * Received a reply for some prior sent query.
     */
    void received(const bb::system::InvokeViewerMessage &msg);

    /**
     * A reply is ready to be received.
     */
    void replyReady();

    /**
     * Need to close viewer window.
     */
    void onViewerCloseRequestReceived(const QString &windowId);

    /**
     * Clean up once the viewer window's been closed..
     */
    void viewerClosed(const QString &windowId);

Q_SIGNALS:
    /**
     * Viewer's been attached.
     */
    void viewerAttached();

    /**
     * Viewer's been closed.
     */
    void viewerClosed();

    /**
     * Error in starting up  viewer.
     */
    void viewerStartupError();

    /**
     * Corresponding signal for windowId property.
     */
    void windowIdChanged();

    /**
     * Signal's a message has been received.
     */
    void messageReceived(const QString &msgText);

    /**
     * Signal's a reply has been received.
     */
    void replyReceived(const QString &replyText);

private Q_SLOTS:
    /**
     * The viewer's been attached to our foreign window.
     */
    void onViewerAttach(screen_window_t windowHandle, const QString &windowGroup,
                        const QString &windowId);
    /**
     * The viewer's been detached.
     */
    void onViewerDetach();

private:
    /**
     * Generate a quasi-random window id.
     */
    QString generateWindowId() const;

    // The viewer window
    bb::cascades::ForeignWindowControl *m_viewerWindow;

    // The window id
    QString m_windowId;

    // Invoke viewer message
    bb::system::InvokeViewerMessage *m_message;

    // Invoke viewer reply
    bb::system::InvokeViewerMessageReply *m_reply;
};

#endif
