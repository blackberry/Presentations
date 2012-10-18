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
#include "soundpartyapp.h"

#include <bb/cascades/TabbedPane>
#include <bb/cascades/QmlDocument>

using namespace bb::cascades;

SoundPartyApp::SoundPartyApp()
{
// Create a QMLDocument and load it, using build patterns.
  mQmlDocument = QmlDocument::create("asset:///main.qml");
  mQmlDocument->setParent(this);

  mQmlDocument->setContextProperty("soundpartyApp", this);

  if (!mQmlDocument->hasErrors())
  {

    // The application navigationPane is created from QML.
    TabbedPane *tabs = mQmlDocument->createRootObject<TabbedPane>();

    if (tabs)
    {

      // Initialize the sound manager with a directory that resides in the
      // assets directory which only contains playable files.
      mSoundManager = new SoundManager("sounds/");

      // Set the main application scene to NavigationPane.
      Application::instance()->setScene(tabs);

    }
  }
}

SoundPartyApp::~SoundPartyApp()
{
  // Destroy the sound manager.
  delete mSoundManager;
}

void SoundPartyApp::playSound(const QString &msg)
{
  // Play sound determined by QString parameter
  mSoundManager->play(msg);
}

