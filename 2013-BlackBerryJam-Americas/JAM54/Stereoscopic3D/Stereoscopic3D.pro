APP_NAME = Stereoscopic3D

CONFIG += qt warn_on cascades10
LIBS   += -lbb -lpps -lscreen -lEGL -lGLESv1_CM -lfreetype -lpng


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
