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

#include "Viewer.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/ForeignWindowControl>
#include <bb/cascades/Window>
#include <bb/system/InvokeManager>
#include <bb/system/InvokeReply>
#include <bb/system/InvokeRequest>
#include <bb/system/InvokeTargetReply>
#include <bb/system/InvokeViewerMessage>
#include <bb/system/InvokeViewerMessageReply>
#include <bb/system/InvokeViewerRequest>

#include <iostream>
#include <time.h>

Viewer::Viewer()
    : m_message(0)
    , m_reply(0)
{
    m_imanager = new bb::system::InvokeManager();

    // Let's create our foreign window where the viewer will render
    m_viewerWindow = bb::cascades::ForeignWindowControl::create();

    // and hook up the viewer window events
    connect(m_viewerWindow, SIGNAL(windowAttached(screen_window_t, const QString &, const QString &)),
            SLOT(onViewerAttach(screen_window_t, const QString &, const QString &)));
    connect(m_viewerWindow, SIGNAL(windowDetached()), SLOT(onViewerDetach()));

    // as well as the invoke manager's events
    connect(m_imanager, SIGNAL(received(const bb::system::InvokeViewerMessage &)),
            SLOT(received(const bb::system::InvokeViewerMessage &)));

    // Let's giver ownership to the custom control
    CustomControl::setRoot(m_viewerWindow);
}

Viewer::Viewer(const QString &name, const QString &target,
               bb::system::InvokeManager *iManager, const QString &defaultAction)
    : Target(name, target, iManager, defaultAction)
    , m_message(0)
    , m_reply(0)
{
    m_description = "Launch " + name;

    // Let's create our foreign window where the viewer will render
    m_viewerWindow = bb::cascades::ForeignWindowControl::create();

    // and hook up the viewer window events
    connect(m_viewerWindow, SIGNAL(windowAttached(screen_window_t, const QString &, const QString &)),
            SLOT(onViewerAttach(screen_window_t, const QString &, const QString &)));
    connect(m_viewerWindow, SIGNAL(windowDetached()), SLOT(onViewerDetach()));

    // as well as the invoke manager's events
    connect(m_imanager, SIGNAL(received(const bb::system::InvokeViewerMessage &)),
            SLOT(received(const bb::system::InvokeViewerMessage &)));

    // Let's giver ownership to the custom control
    CustomControl::setRoot(m_viewerWindow);
}

bool Viewer::operator==(const Viewer &other) const
{
    return this->name() == other.name();
}

QString Viewer::windowId() const
{
    return m_windowId;
}

void Viewer::invoke()
{
    // Close any previous viewers
    if (m_viewerWindow->isBoundToWindow())
        m_imanager->closeViewer(m_windowId);

    // Any unique window name will do
    m_windowId = generateWindowId();
    emit windowIdChanged();

    // Prepare the foreign window control for the incoming viewer
    m_viewerWindow->setWindowId(m_windowId);
    m_viewerWindow->setWindowGroup(
        bb::cascades::Application::instance()->mainWindow()->groupId());

    bb::system::InvokeViewerRequest viewerRequest;
    viewerRequest.setWindowId(m_windowId);
    viewerRequest.setWindowSize(QSize(preferredWidth(), preferredHeight()));

    // Set the invoke target, action, mime & uri
    viewerRequest.setTarget(m_target);
    viewerRequest.setMimeType(m_mime);
    viewerRequest.setAction(m_action);
    viewerRequest.setUri(m_uri);

    // Now, let's send the invoke message
    std::cout << "invoke<>" << m_target.toStdString() << "::" << action().toStdString() << "::" << mime().toStdString() << "::" << uri().toStdString() << "::" << data().toStdString() << std::endl;
    bb::system::InvokeReply *viewerReply = m_imanager->invoke(viewerRequest);
    if (!viewerReply) {
        std::cerr << "Unable to invoke viewer." << std::endl;
        return;
    }

    // Wait for a reply...
    connect(viewerReply, SIGNAL(finished()), SLOT(invokeFinished()));
}

void Viewer::closeViewer()
{
    // Can't close what isn't there
    if (m_windowId == "")
        return;

    bool result = m_imanager->closeViewer(m_windowId);
    if (!result)
        std::cerr << "Unable to close viewer." << std::endl;
}

void Viewer::sendMessageToViewer(const QString &name, const QString &data)
{
    // Prepare the message
    bb::system::InvokeViewerMessage message;
    message.setWindowId(m_windowId);
    message.setName(name);
    message.setData(data);

    bool ok = false;
    m_imanager->send(message, &ok);

    if (!ok)
        std::cerr << "Sending message to viewer failed." << std::endl;
}

void Viewer::sendQueryResponseMessage(const QString &name, const QString &data)
{
    static int idCounter = 0;

    // Prepare the message
    bb::system::InvokeViewerMessage message;
    message.setWindowId(m_windowId);
    message.setId(QString::number(idCounter, 10));
    message.setName(name);
    message.setData(data);

    bool ok = false;
    m_reply = m_imanager->send(message, &ok);

    if (!m_reply) {
        std::cerr << "Sending message to viewer failed." << std::endl;
        return;
    }

    connect(m_reply, SIGNAL(finished()), SLOT(replyReady()));
}

void Viewer::reply(const QString &name, const QString &data)
{
    if (m_message) {
        m_message->setName(name);
        m_message->setData(data);

        m_imanager->reply(*m_message);
    } else
        std::cerr << "No message to reply to." << std::endl;
}

void Viewer::invokeFinished()
{
    // The invocation has happened so let's hook up the signals
    connect(m_imanager, SIGNAL(viewerCloseRequestReceived(const QString &)),
            this, SLOT(onViewerCloseRequestReceived(const QString &)));

    connect(m_imanager, SIGNAL(viewerClosed(const QString &)),
            this, SLOT(viewerClosed(const QString &)));
}

void Viewer::queryFinished()
{
}

void Viewer::received(const bb::system::InvokeViewerMessage &msg)
{
    // We're only interested in our messages
    if (m_windowId.compare(msg.windowId()) == 0) {
        const QString text = "id=" + msg.id() + " Name: " + msg.name() + " Data: " + msg.data();
        emit messageReceived(text);

        m_message = new bb::system::InvokeViewerMessage(msg);

        // Some close messages
        if (msg.name() == "viewerCancelRequest" || msg.name() == "close")
            closeViewer();
    }
}

void Viewer::replyReady()
{
    if (!m_reply)
        return;

    if (m_reply->error() == bb::system::InvokeViewerMessageReplyError::None) {
        const QString text = "id=" + m_reply->id() + " Name: " + m_reply->name() + " Data: " + m_reply->data();
        emit replyReceived(text);
    } else {
        const QString text = "Error! id=" + m_reply->id() + " Name: " + m_reply->name() + " Data: " + m_reply->data();
        emit replyReceived(text);
    }
}

void Viewer::onViewerCloseRequestReceived(const QString &windowId)
{
    // We got a request, let's honour it if it's for us
    if (m_windowId.compare(windowId) == 0)
        closeViewer();
}

void Viewer::viewerClosed(const QString &windowId)
{
    // Let's clear the old window id
    m_windowId = "";
    emit windowIdChanged();
}

void Viewer::onViewerAttach(screen_window_t windowHandle, const QString &windowGroup,
                            const QString &windowId)
{
    // Viewers are initially created invisible, so we must make the window visible ourselves
    int visibility = 1;
    screen_set_window_property_iv(windowHandle, SCREEN_PROPERTY_VISIBLE, &visibility);

    emit viewerAttached();
}

void Viewer::onViewerDetach()
{
    m_viewerWindow->setWindowHandle(0);
    emit viewerClosed();
}

QString Viewer::generateWindowId() const
{
    QString windowId;

    // Let's seed with the current time
    srand(time(0));
    int randomId = rand();

    windowId.sprintf("viewer-%x", randomId);

    return windowId;
}

