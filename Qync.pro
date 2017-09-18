QT += widgets
CONFIG += c++14

HEADERS += \
    src/common.h \
    src/MainWindow.h \
    src/GuiPreferences.h \
    src/Manager.h \
    src/Preferences.h \
    src/PreferencesDialogue.h \
    src/Preset.h \
    src/Process.h \
    src/ProcessDialogue.h

SOURCES += \
	src/qync.cpp \
    src/common.cpp \
    src/MainWindow.cpp \
    src/GuiPreferences.cpp \
    src/Manager.cpp \
    src/Preferences.cpp \
    src/PreferencesDialogue.cpp \
    src/Preset.cpp \
    src/Process.cpp \
    src/ProcessDialogue.cpp

RESOURCES += resources/icons.qrc

OTHER_FILES += dist/linux/qync.desktop \
    dist/linux/qync.menu \
    dist/linux/man/man1/qync.1 \
    dist/linux/doc/qync/copyright \
    dist/linux/doc/qync/changelog.Debian \
    dist/linux/doc/qync/changelog \
    doc/cwrsync-license.txt \
    doc/license.txt \
    todo.txt \
    dist/linux/qync.desktop
