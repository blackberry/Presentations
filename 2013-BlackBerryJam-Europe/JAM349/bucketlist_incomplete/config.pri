# Auto-generated by IDE. All changes by user will be lost!
# Created at 2/1/13 12:55 PM

BASEDIR = $$_PRO_FILE_PWD_

INCLUDEPATH +=  \
    $$BASEDIR/src/bucketdata \
    $$BASEDIR/src \
    $$BASEDIR/src/bucketbbm

SOURCES +=  \
    $$BASEDIR/src/bucketbbm/bucketbbmmanager.cpp \
    $$BASEDIR/src/bucketbbm/global.cpp \
    $$BASEDIR/src/bucketbbm/profilebox.cpp \
    $$BASEDIR/src/bucketbbm/profilemessage.cpp \
    $$BASEDIR/src/bucketbbm/registrationhandler.cpp \
    $$BASEDIR/src/bucketdata/bucketmodel.cpp \
    $$BASEDIR/src/bucketdata/bucketsettings.cpp \
    $$BASEDIR/src/bucketlistapp.cpp \
    $$BASEDIR/src/main.cpp

HEADERS +=  \
    $$BASEDIR/src/bucketbbm/bucketbbmmanager.h \
    $$BASEDIR/src/bucketbbm/global.h \
    $$BASEDIR/src/bucketbbm/profilebox.h \
    $$BASEDIR/src/bucketbbm/profilemessage.h \
    $$BASEDIR/src/bucketbbm/registrationhandler.h \
    $$BASEDIR/src/bucketdata/bucketmodel.h \
    $$BASEDIR/src/bucketdata/bucketsettings.h \
    $$BASEDIR/src/bucketlistapp.h

CONFIG += precompile_header
PRECOMPILED_HEADER = $$BASEDIR/precompiled.h

lupdate_inclusion {
    SOURCES += \
        $$BASEDIR/../assets/*.qml
}

TRANSLATIONS = \
    $${TARGET}.ts

