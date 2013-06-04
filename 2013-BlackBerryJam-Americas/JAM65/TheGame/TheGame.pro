APP_NAME = TheGame

CONFIG += qt warn_on cascades10
LIBS   += -lOpenAL -lalut -lasound -lbbdata -lbbsystem

include(config.pri)

TRANSLATIONS = \
    $${TARGET}_en_GB.ts \
    $${TARGET}_fr.ts \
    $${TARGET}_it.ts \    
    $${TARGET}_de.ts \
    $${TARGET}_es.ts \
    $${TARGET}.ts

