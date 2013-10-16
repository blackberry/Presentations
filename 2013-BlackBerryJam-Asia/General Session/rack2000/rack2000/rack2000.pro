APP_NAME = rack2000

CONFIG += qt warn_on cascades10

LIBS += -lbtapi -lbbsystem
LIBS += -lnfc -lnfc_bps -lbb -lQtNfcSubset

CONFIG += mobility
MOBILITY += sensors

include(config.pri)
