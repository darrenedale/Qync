QT += widgets
CONFIG += c++14

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
    src/aboutdialogue.h

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
    src/aboutdialogue.cpp

RESOURCES += resources/icons.qrc

DISTFILES += dist/linux/qync.desktop \
    dist/linux/qync.menu \
    dist/linux/man/man1/qync.1 \
    dist/linux/doc/qync/copyright \
    dist/linux/doc/qync/changelog.Debian \
    dist/linux/doc/qync/changelog \
    doc/cwrsync-license.txt \
    doc/license.txt \
    todo.txt \
    doc/mainpage.dox \
    doc/license_cwrsync.dox \
    doc/license_rsync.dox \
    doc/license_cygwin.dox \
    doc/license_openssh.dox \
    doc/license_putty.dox \
    .gitignore

FORMS += \
    ui/processdialogue.ui \
    ui/mainwindow.ui \
    ui/preferencesdialogue.ui \
    ui/aboutdialogue.ui
