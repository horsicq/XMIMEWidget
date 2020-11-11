INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

!contains(XCONFIG, xformats) {
    XCONFIG += xformats
    include(../XMIME/xmime.pri)
}

FORMS += \
    $$PWD/xmimewidget.ui

HEADERS += \
    $$PWD/xmimewidget.h

SOURCES += \
    $$PWD/xmimewidget.cpp
