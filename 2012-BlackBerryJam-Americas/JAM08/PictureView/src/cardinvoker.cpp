/*
 * cardinvoker.cpp
 *
 *  Created on: Sep 16, 2012
 *      Author: randrade
 */

#include "cardinvoker.h"
#include "Opt.hpp"
#include "Target.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/ActionItem>
#include <bb/cascades/DropDown>
#include <bb/cascades/GroupDataModel>
#include <bb/cascades/Label>
#include <bb/cascades/Option>
#include <bb/data/JsonDataAccess>
#include <bb/system/InvokeManager>
#include <bb/system/InvokeQueryTargetsRequest>
#include <bb/system/InvokeQueryTargetsReply>
#include <bb/system/InvokeRequest>
#include <bb/system/InvokeReply>
#include <bb/system/InvokeAction>
#include <bb/system/InvokeTarget>
#include <bb/platform/Homescreen>

#include <iostream>

using namespace bb::system;
using namespace bb::platform;

CardInvoker::CardInvoker(QObject *parent)
    : QObject(parent)
    , m_imanager(new InvokeManager(this))
{
    initTargets();

}

void CardInvoker::initTargets()
{
	QString filepath;

    m_target = new Target("Pictures", "sys.pictures.card.previewer", m_imanager, "bb.action.VIEW");


    Opt *defalt = new Opt("Default","with default action OPEN","","","","bb.action.OPEN");
    //QList<Opt*> list;
    m_content.clear();
    m_content.append(defalt);

    QDir dir("./shared/camera");


    if (dir.isReadable()){
    	QFileInfoList entries = dir.entryInfoList();
    	for (QList<QFileInfo>::iterator it = entries.begin(); it != entries.end(); it++) {
    		QFileInfo &fileInfo = *it;
    		if (fileInfo.fileName() == "." || fileInfo.fileName() == "..") {
    			continue;
    		}

    		//don't want to search subdirectories with camera
    		if ( !fileInfo.isDir()) {
    			if (fileInfo.isFile() && fileInfo.isReadable()) {
    				QString name(fileInfo.fileName());

    				filepath = "file:///accounts/1000/shared/camera/" + name;
    				std::cout << " File Name: " << name.toStdString() << std::endl;
    				std::cout << " File uri: " << filepath.toStdString() << std::endl;
    				m_content.append(new Opt(name, filepath, "image/jpeg", filepath));
    			}
    		}
    	}

    }



}

void CardInvoker::updateTarget(const QString &fullpathname)
{
	QString substr("IMG_00");
	int pos = fullpathname.indexOf(substr);
	int nsize = fullpathname.size() - pos;
	QString name = fullpathname.mid(pos, nsize);

	std::cout << " File Name: " << name.toStdString() << std::endl;
	std::cout << " File uri: " << fullpathname.toStdString() << std::endl;
	m_content.append(new Opt(name, fullpathname, "image/jpeg", fullpathname));

}

void CardInvoker::setTargetMap(QDeclarativePropertyMap* targetMap)
{
    m_targetMap = targetMap;
}

void CardInvoker::setTarget(const QString &category, const QString &name)
{
    if (m_target) {
        m_targetMap->insert("target", QVariant(m_target->target()));
        m_targetMap->insert("mime", QVariant(m_target->mime()));
        m_targetMap->insert("uri", QVariant(m_target->uri()));
        m_targetMap->insert("data", QVariant(m_target->data()));
        m_targetMap->insert("action", QVariant(m_target->action()));
        m_targetMap->insert("targetDescription", QVariant(m_target->description()));
    } else {
        std::cout << "WARNING: " << category.toStdString() << "::" << name.toStdString() << " not found" << std::endl;
    }
}

void CardInvoker::setContent(const QString &name, const int &selectedIndex)
{
    if (!m_content.isEmpty()) {
        if(selectedIndex >= 0 && selectedIndex < m_content.size()) {
            m_targetMap->insert("mime", QVariant(m_content[selectedIndex]->mime()));
            m_targetMap->insert("uri", QVariant(m_content[selectedIndex]->uri()));
            m_targetMap->insert("data", QVariant(m_content[selectedIndex]->data()));
            m_targetMap->insert("action", QVariant(m_content[selectedIndex]->action()));
            m_targetMap->insert("contentDescription", QVariant(m_content[selectedIndex]->description()));
        } else {
            std::cerr << "Index out of bounds[0-" << m_content.size() << "]: " << selectedIndex << std::endl;
        }
    } else {
        std::cout << "WARNING: found no content for " << name.toStdString() << std::endl;
    }
}

void CardInvoker::invoke(const QString &category, const QString &name)
{
    if (m_target) {
        m_target->invoke();
    } else {
        std::cout << "WARNING: " << category.toStdString() << "::" << name.toStdString() << " not found" << std::endl;
    }
}

void CardInvoker::populateContentDropDown(const QString &dropdname)
{
    using namespace bb::cascades;

    if (!m_content.isEmpty()) {
        DropDown *dropdown = m_abstractPane->findChild<DropDown*>(dropdname);
        if(!dropdown) {
            std::cout << "WARNING: could not find child element " << dropdname.toStdString() << std::endl;
        } else {
            dropdown->removeAll();
            for (int i = 0; i < m_content.size(); i++) {
                if (m_content.at(i)->text() == "Default") {
                    dropdown->add(Option::create().text(m_content.at(i)->text()).selected(true));
                } else {
                    dropdown->add(Option::create().text(m_content.at(i)->text()));
                }
            }
        }
    } else {
      std::cout << "List is empty" << std::endl;
    }
}



void CardInvoker::setAbstractPane(const bb::cascades::AbstractPane *abspane)
{
    m_abstractPane = abspane;
}

void CardInvoker::setaswallpaper(const int &selectedIndex)
{
	HomeScreen homescreen;

	if (!m_content.isEmpty()) {
	        if(selectedIndex >= 0 && selectedIndex < m_content.size()) {
	        	QUrl url(m_content[selectedIndex]->uri());
	        	std::cout << "Setting wallpaper" << std::endl;
	            homescreen.setWallpaper(url);
	        } else {
	            std::cerr << "Index out of bounds[0-" << m_content.size() << "]: " << selectedIndex << std::endl;
	        }
	    } else {
	        std::cout << "WARNING: found no content " << std::endl;
	    }


}







