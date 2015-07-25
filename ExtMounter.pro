# -------------------------------------------------
# Project created by QtCreator 2015-05-27T21:59:31
# -------------------------------------------------
TARGET = ExtMounter
TEMPLATE = app
#DEFINES += QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII
SOURCES += main.cpp \
    ExtCopy.cpp \
    ExtExplorer.cpp \
    Read.cpp \
    Log.cpp \
    DisksOperations.cpp \
    Partition.cpp
HEADERS += \
    ExtCopy.h \
    FileSystem.h \
    ExtExplorer.h \
    Read.h \
    MBR.h \
    DisksOperations.h
FORMS += \
    ExtExplorer.ui \
    Progress.ui
RESOURCES += ExtMounter.qrc
RC_FILE = app.rc
