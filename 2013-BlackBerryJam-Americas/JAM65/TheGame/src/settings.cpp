/*
 * Copyright (c) 2011, 2012 Research In Motion Limited.
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
#include "settings.h"

#include <bb/cascades/Application>
#include <QSettings>

#include <QDebug>

Settings::Settings()
{
    // We set up the application Organization and name, this is used by QSettings
    // when saving values to the persistent store.
    QCoreApplication::setOrganizationName("CFA");
    QCoreApplication::setApplicationName("TheGame");

}

Settings::~Settings()
{
}

QString Settings::getValueFor(const QString &objectName, const QString &defaultValue)
{
    QSettings settings;

    // If no value has been saved return the value.
    if (settings.value(objectName).isNull()) {
        return defaultValue;
    }

    // Otherwise return the value stored in the settings object.
    return settings.value(objectName).toString();
}

void Settings::saveValueFor(const QString &objectName, const QString &inputValue)
{
    // A new value is saved to the application settings object.
    QSettings settings;
    settings.setValue(objectName, QVariant(inputValue));
}

QString Settings::theme()
{
    return getValueFor("game_theme", "Turtle");
}
void Settings::setTheme(const QString& themeName)
{
    saveValueFor("game_theme", themeName);
    emit themeChanged(themeName);
}
