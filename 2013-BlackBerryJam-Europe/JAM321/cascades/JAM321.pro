APP_NAME = JAM321

LIBS += -lscreen -lEGL -lGLESv1_CL -lpng14 -lfreetype -lbtapi

CONFIG += qt warn_on cascades10

include(config.pri)

DEFINES += USING_GL11
