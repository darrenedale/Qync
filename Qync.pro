QT += widgets
CONFIG += c++1z

#QMAKE_CXXFLAGS += -std=c++1z
DEFINES += QT_NO_KEYWORDS QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x050701

HEADERS += \
    src/functions.h \
    src/application.h \
    src/guipreferences.h \
    src/mainwindow.h \
    src/preferences.h \
    src/preferencesdialogue.h \
    src/preset.h \
    src/process.h \
    src/processdialogue.h \
    src/aboutdialogue.h \
    src/sourcedestinationwidget.h \
    src/units.h \
    src/synchronisewhatcombo.h \
    src/processwidget.h \
    src/presetcombo.h \
    src/presetmenu.h \
    src/notificationwidget.h \
    src/types.h

SOURCES += \
	src/qync.cpp \
    src/functions.cpp \
    src/application.cpp \
    src/guipreferences.cpp \
    src/mainwindow.cpp \
    src/preferences.cpp \
    src/preferencesdialogue.cpp \
    src/preset.cpp \
    src/process.cpp \
    src/processdialogue.cpp \
    src/aboutdialogue.cpp \
    src/sourcedestinationwidget.cpp \
    src/synchronisewhatcombo.cpp \
    src/processwidget.cpp \
    src/presetcombo.cpp \
    src/presetmenu.cpp \
    src/notificationwidget.cpp

RESOURCES += resources/icons.qrc

DISTFILES += tools/* \
    dist/linux/qync.desktop \
    dist/linux/qync.menu \
    dist/linux/man/man1/qync.1 \
    dist/linux/doc/qync/copyright \
    dist/linux/doc/qync/changelog.Debian \
    dist/linux/doc/qync/changelog \
    todo.txt \
    docs/license.txt \
    docs/cwrsync-license.txt \
    docs/mainpage.dox \
    docs/license_cwrsync.dox \
    docs/license_rsync.dox \
    docs/license_cygwin.dox \
    docs/license_openssh.dox \
    docs/license_putty.dox \
    .gitignore \
    .clang-format \
    docs/namespaces.dox \
    qync.Doxyfile

FORMS += \
    ui/processdialogue.ui \
    ui/mainwindow.ui \
    ui/preferencesdialogue.ui \
    ui/aboutdialogue.ui \
    ui/sourcedestinationwidget.ui \
    ui/processwidget.ui \
    ui/notificationwidget.ui
