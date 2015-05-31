# -------------------------------------------------
# Project created by QtCreator 2015-05-27T21:59:31
# -------------------------------------------------
TARGET = ExtMounter
TEMPLATE = app
#DEFINES += QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII
SOURCES += main.cpp \
    ExtCopy.cpp \
    ExtExplorer.cpp \
    FileSystem.cpp \
    Read.cpp \
    LVM.cpp \
    Log.cpp \
    DisksOperations.cpp
HEADERS += \
    ExtCopy.h \
    LVM.h \
    FileSystem.h \
    ExtExplorer.h \
    Read.h \
    Partition.h \
    Platform.h
FORMS += ext2explore.ui \
    ext2progress.ui
RESOURCES += ExtMounter.qrc
RC_FILE = app.rc
