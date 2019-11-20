#-------------------------------------------------
#
# Project created by QtCreator 2019-08-06T09:43:01
#
#-------------------------------------------------

TARGET = SVSReader
TEMPLATE = lib

DEFINES += SVSREADER_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        svsreader.cpp

HEADERS += \
        svsreader.h \
        svsreader_global.h 

unix
{
    target.path = /usr/lib
    INSTALLS += target

    debug
    {
        DESTDIR = $$PWD/../BUILD/debug
    }
    release
    {
        DESTDIR = $$PWD/../BUILD/release
    }
}

win32
{
    debug
    {
        DESTDIR = $$PWD/../BUILD/debug
    }
    release
    {
        DESTDIR = $$PWD/../BUILD/release
    }
}

macx
{
    debug
    {
        target.path = $$PWD/../BUILD/
        INSTALLS += target

        CONFIG += install_ok

        DESTDIR = $$PWD/../BUILD/debug
    }

    release
    {
        target.path = $$PWD/../BUILD/
        INSTALLS += target

        CONFIG += install_ok

        DESTDIR = $$PWD/../BUILD/release
    }
}


win32: LIBS += -L$$PWD/../TIFF/Libs/ -ltiff
else:unix: LIBS += -L$$PWD/../../../usr/lib/x86_64-linux-gnu/ -ltiff

win32:INCLUDEPATH += $$PWD/../TIFF/Include
win32:DEPENDPATH += $$PWD/../TIFF/Include

unix:INCLUDEPATH += $$PWD/../../../usr/lib/x86_64-linux-gnu
unix:DEPENDPATH += $$PWD/../../../usr/lib/x86_64-linux-gnu

macx: LIBS += -L$$PWD/../../../../../../usr/local/Cellar/libtiff/4.1.0/lib/ -ltiff

INCLUDEPATH += $$PWD/../../../../../../usr/local/Cellar/libtiff/4.1.0/include
DEPENDPATH += $$PWD/../../../../../../usr/local/Cellar/libtiff/4.1.0/include

macx: PRE_TARGETDEPS += $$PWD/../../../../../../usr/local/Cellar/libtiff/4.1.0/lib/libtiff.a
