INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    $$PWD/dialogmime.ui \
    $$PWD/xmimewidget.ui

HEADERS += \
    $$PWD/dialogmime.h \
    $$PWD/xmimewidget.h

SOURCES += \
    $$PWD/dialogmime.cpp \
    $$PWD/xmimewidget.cpp

!contains(XCONFIG, xformats) {
    XCONFIG += xformats
    include(../XMIME/xmime.pri)
}
