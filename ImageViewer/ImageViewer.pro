QT += quick core widgets gui
QT += quickcontrols2
QT += qml
CONFIG += c++11
unix:android: QMAKE_LINK += -nostdlib++

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        imageprovider.cpp \
        main.cpp \
        mainview.cpp \
        slide.cpp \
        viewspace.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32{
debug{
    DESTDIR = $$PWD/../BUILD/debug
}
release
{
    DESTDIR = $$PWD/../BUILD/release
}
}

win32: LIBS += -L$$PWD/../TIFF/Libs/ -ltiff

INCLUDEPATH += $$PWD/../TIFF/Include
DEPENDPATH += $$PWD/../TIFF/Include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../BUILD/release/ -lSVSReader
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../BUILD/debug/ -lSVSReader

INCLUDEPATH += $$PWD/../SVSReader
DEPENDPATH += $$PWD/../SVSReader



HEADERS += \
    imageprovider.h \
    mainview.h \
    slide.h \
    viewspace.h

FORMS +=
