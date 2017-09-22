QT += widgets
CONFIG += c++14

QMAKE_CXXFLAGS += -std=c++1z

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

DISTFILES += tools/* \
    dist/linux/qync.desktop \
    dist/linux/qync.menu \
    dist/linux/man/man1/qync.1 \
    dist/linux/doc/qync/copyright \
    dist/linux/doc/qync/changelog.Debian \
    dist/linux/doc/qync/changelog \
	Qync.Doxyfile \
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
    .clang-format

FORMS += \
    ui/processdialogue.ui \
    ui/mainwindow.ui \
    ui/preferencesdialogue.ui \
    ui/aboutdialogue.ui
