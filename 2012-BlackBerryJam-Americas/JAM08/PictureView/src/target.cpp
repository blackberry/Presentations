/*
 * target.cpp
 *
 *  Created on: Sep 16, 2012
 *      Author: randrade
 */


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

#include "Target.hpp"
#include "Opt.hpp"

#include <bb/system/InvokeAction>
#include <bb/system/InvokeManager>
#include <bb/system/InvokeQueryTargetsReply>
#include <bb/system/InvokeQueryTargetsRequest>
#include <bb/system/InvokeReply>
#include <bb/system/InvokeRequest>
#include <bb/system/InvokeTarget>
#include <bb/system/InvokeTargetReply>

#include <iostream>

using namespace bb::system;

Target::Target()
    : m_invokeReply(0)
    , m_imanager(0)
    , m_queryReply(0)
{
}

Target::Target(const QString &name, const QString &target, bb::system::InvokeManager *imanager, const QString &defaultAction, const int &targetType)
    : m_name(name)
    , m_target(target)
    , m_ireply("--")
    , m_action(defaultAction)
    , m_description("Launch " + name)
    , m_contentDescription("with default action " + defaultAction.section('.', 2, 2))
    , m_targetType(targetType)
    , m_invokeReply(0)
    , m_imanager(imanager)
    , m_queryReply(0)
{
}

Target::~Target()
{
    delete m_imanager;
}

QString Target::name() const
{
    return m_name;
}

QString Target::action() const
{
    return m_action;
}

QString Target::mime() const
{
    return m_mime;
}

QString Target::data() const
{
    return m_data;
}

QString Target::uri() const
{
    return m_uri;
}

QString Target::target() const
{
    return m_target;
}

QString Target::description() const
{
    return m_description;
}

QString Target::contentDescription() const
{
    return m_contentDescription;
}


QString Target::invokeReply() const
{
    return m_ireply;
}

void Target::setTarget(const QString &target)
{
    if (m_target != target) {
        m_target = target;
        emit targetChanged();
    }
}

void Target::setMime(const QString &mime)
{
    if (m_mime != mime) {
        m_mime = mime;
        emit mimeChanged();
    }
}

void Target::setAction(const QString &action)
{
    if (m_action != action) {
        m_action = action;
        emit actionChanged();
    }
}

void Target::setData(const QString &data)
{
    if (m_data != data) {
        m_data = data;
        emit dataChanged();
    }
}

void Target::setUri(const QString &uri)
{
    if (m_uri != uri) {
        m_uri = uri;
        emit uriChanged();
    }
}

void Target::setDescription(const QString &description)
{
    if (m_description != description) {
        m_description = description;
        emit descriptionChanged();
    }
}

void Target::setContentDescription(const QString &contentDescription)
{
    if (m_contentDescription != contentDescription) {
        m_contentDescription = contentDescription;
        emit contentDescriptionChanged();
    }
}



void Target::processInvokeReply()
{
    if (m_invokeReply && m_invokeReply->isFinished()) {
        m_ireply = "Launch-status::";
        m_ireply += QString::number(m_invokeReply->error());
        std::cout << "reply<>" << m_target.toStdString() << "<>" << m_ireply.toStdString() << std::endl;
        emit replyReceived(m_ireply);
    }
}

void Target::invoke()
{
    m_ireply = "--";
    m_invokeReply = 0;

    bb::system::InvokeRequest request;
    request.setTarget(target());
    request.setAction(action());
    request.setMimeType(mime());
    request.setUri(uri());

    if (!m_data.isEmpty()) {
        request.setData(m_data.toUtf8());
    }

    std::cout << "invoke<>" << m_target.toStdString() << "::" << action().toStdString() << "::" << mime().toStdString() << "::" << uri().toStdString() << "::" << (!data().isEmpty() ? data().toStdString() : "") << std::endl;

    m_invokeReply = m_imanager->invoke(request);

    if (!m_invokeReply) {
        std::cerr << "ERROR: Invoke for " << name().toStdString() << " failed" << std::endl;
        return;
    }

    const bool result = connect(m_invokeReply, SIGNAL(finished()), this, SLOT(processInvokeReply()));
    if (!result) {
        std::cerr << "ERROR: Connect failed" << std::endl;
    }
}

bool Target::operator==(const Target &other) const
{
    return (name() == other.name());
}


