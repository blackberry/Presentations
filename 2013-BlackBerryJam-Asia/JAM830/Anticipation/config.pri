# Auto-generated by IDE. Any changes made by user will be lost!
BASEDIR =  $$quote($$_PRO_FILE_PWD_)

device {
    CONFIG(debug, debug|release) {
        SOURCES +=  $$quote($$BASEDIR/src/applicationui.cpp) \
                 $$quote($$BASEDIR/src/data/animdatasource.cpp) \
                 $$quote($$BASEDIR/src/data/globalconnection.cpp) \
                 $$quote($$BASEDIR/src/main.cpp) \
                 $$quote($$BASEDIR/src/pulltorefresh.cpp)

        HEADERS +=  $$quote($$BASEDIR/src/applicationui.hpp) \
                 $$quote($$BASEDIR/src/data/animdatasource.h) \
                 $$quote($$BASEDIR/src/data/globalconnection.h) \
                 $$quote($$BASEDIR/src/pulltorefresh.h)
    }

    CONFIG(release, debug|release) {
        SOURCES +=  $$quote($$BASEDIR/src/applicationui.cpp) \
                 $$quote($$BASEDIR/src/data/animdatasource.cpp) \
                 $$quote($$BASEDIR/src/data/globalconnection.cpp) \
                 $$quote($$BASEDIR/src/main.cpp) \
                 $$quote($$BASEDIR/src/pulltorefresh.cpp)

        HEADERS +=  $$quote($$BASEDIR/src/applicationui.hpp) \
                 $$quote($$BASEDIR/src/data/animdatasource.h) \
                 $$quote($$BASEDIR/src/data/globalconnection.h) \
                 $$quote($$BASEDIR/src/pulltorefresh.h)
    }
}

simulator {
    CONFIG(debug, debug|release) {
        SOURCES +=  $$quote($$BASEDIR/src/applicationui.cpp) \
                 $$quote($$BASEDIR/src/data/animdatasource.cpp) \
                 $$quote($$BASEDIR/src/data/globalconnection.cpp) \
                 $$quote($$BASEDIR/src/main.cpp) \
                 $$quote($$BASEDIR/src/pulltorefresh.cpp)

        HEADERS +=  $$quote($$BASEDIR/src/applicationui.hpp) \
                 $$quote($$BASEDIR/src/data/animdatasource.h) \
                 $$quote($$BASEDIR/src/data/globalconnection.h) \
                 $$quote($$BASEDIR/src/pulltorefresh.h)
    }
}

INCLUDEPATH +=  $$quote($$BASEDIR/src/data) \
         $$quote($$BASEDIR/src)

CONFIG += precompile_header

PRECOMPILED_HEADER =  $$quote($$BASEDIR/precompiled.h)

lupdate_inclusion {
    SOURCES +=  $$quote($$BASEDIR/../src/*.c) \
             $$quote($$BASEDIR/../src/*.c++) \
             $$quote($$BASEDIR/../src/*.cc) \
             $$quote($$BASEDIR/../src/*.cpp) \
             $$quote($$BASEDIR/../src/*.cxx) \
             $$quote($$BASEDIR/../src/data/*.c) \
             $$quote($$BASEDIR/../src/data/*.c++) \
             $$quote($$BASEDIR/../src/data/*.cc) \
             $$quote($$BASEDIR/../src/data/*.cpp) \
             $$quote($$BASEDIR/../src/data/*.cxx) \
             $$quote($$BASEDIR/../assets/*.qml) \
             $$quote($$BASEDIR/../assets/*.js) \
             $$quote($$BASEDIR/../assets/*.qs) \
             $$quote($$BASEDIR/../assets/images/*.qml) \
             $$quote($$BASEDIR/../assets/images/*.js) \
             $$quote($$BASEDIR/../assets/images/*.qs) \
             $$quote($$BASEDIR/../assets/models/*.qml) \
             $$quote($$BASEDIR/../assets/models/*.js) \
             $$quote($$BASEDIR/../assets/models/*.qs) \
             $$quote($$BASEDIR/../assets/models/sql/*.qml) \
             $$quote($$BASEDIR/../assets/models/sql/*.js) \
             $$quote($$BASEDIR/../assets/models/sql/*.qs)

    HEADERS +=  $$quote($$BASEDIR/../src/*.h) \
             $$quote($$BASEDIR/../src/*.h++) \
             $$quote($$BASEDIR/../src/*.hh) \
             $$quote($$BASEDIR/../src/*.hpp) \
             $$quote($$BASEDIR/../src/*.hxx)
}

TRANSLATIONS =  $$quote($${TARGET}.ts)