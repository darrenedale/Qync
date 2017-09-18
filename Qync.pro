HEADERS += src/QyncMainWindow.h \
    src/QyncManager.h \
    src/QyncPreset.h \
    src/common.h \
    src/QyncProcess.h \
    src/QyncProcessDialogue.h \
    src/QyncPreferences.h \
    src/QyncPreferencesDialogue.h \
    src/QyncGuiPreferences.h

SOURCES += src/QyncMainWindow.cpp \
    src/QyncManager.cpp \
    src/QyncPreset.cpp \
    qync.cpp \
    src/QyncProcess.cpp \
    src/QyncProcessDialogue.cpp \
    src/QyncPreferences.cpp \
    src/QyncPreferencesDialogue.cpp \
    src/common.cpp \
    src/QyncGuiPreferences.cpp

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
