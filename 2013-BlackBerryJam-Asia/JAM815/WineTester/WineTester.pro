APP_NAME = WineTester

CONFIG += qt warn_on cascades10

LIBS += -lbbmultimedia
LIBS += -lbbcascadesmaps
LIBS += -lbbdata
LIBS += -lbb
LIBS += -lbbdevice
LIBS += -lbbsystem

include(config.pri)

device {
    CONFIG(debug, debug|release) {
        # Device-Debug custom configuration
    }

    CONFIG(release, debug|release) {
        # Device-Release custom configuration
    }
}

simulator {
    CONFIG(debug, debug|release) {
        # Simulator-Debug custom configuration
    }
}
