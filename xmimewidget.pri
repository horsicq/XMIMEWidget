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

!contains(XCONFIG, xmime) {
    XCONFIG += xmime
    include($$PWD/../XMIME/xmime.pri)
}

!contains(XCONFIG, xshortcuts) {
    XCONFIG += xshortcuts
    include($$PWD/../XShortcuts/xshortcuts.pri)
}

DISTFILES += \
    $$PWD/LICENSE \
    $$PWD/README.md \
    $$PWD/xmimewidget.cmake
