/****************************************************************************
** Meta object code from reading C++ file 'QyncMainWindow.h'
**
** Created: Tue Jul 27 18:21:13 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "QyncMainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QyncMainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QyncMainWindow[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x08,
      43,   37,   15,   15, 0x08,
      59,   15,   15,   15, 0x08,
      77,   15,   15,   15, 0x0a,
      94,   15,   15,   15, 0x0a,
     118,   15,   15,   15, 0x0a,
     147,   15,   15,   15, 0x0a,
     166,   15,   15,   15, 0x0a,
     190,   15,   15,   15, 0x0a,
     206,   15,   15,   15, 0x0a,
     235,   15,   15,   15, 0x0a,
     265,   15,   15,   15, 0x0a,
     287,   15,   15,   15, 0x0a,
     311,   15,   15,   15, 0x0a,
     326,   15,   15,   15, 0x0a,
     341,   15,   15,   15, 0x0a,
     359,   15,   15,   15, 0x0a,
     370,   15,   15,   15, 0x0a,
     380,   15,   15,   15, 0x0a,
     388,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QyncMainWindow[] = {
    "QyncMainWindow\0\0showPresetFromMenu()\0"
    "index\0showPreset(int)\0readPreferences()\0"
    "refreshPresets()\0chooseSourceDirectory()\0"
    "chooseDestinationDirectory()\0"
    "chooseSourceFile()\0chooseDestinationFile()\0"
    "chooseLogFile()\0switchSourceAndDestination()\0"
    "saveSettingsToCurrentPreset()\0"
    "removeCurrentPreset()\0newPresetFromSettings()\0"
    "importPreset()\0exportPreset()\0"
    "showPreferences()\0simulate()\0execute()\0"
    "about()\0aboutRsync()\0"
};

const QMetaObject QyncMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_QyncMainWindow,
      qt_meta_data_QyncMainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QyncMainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QyncMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QyncMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QyncMainWindow))
        return static_cast<void*>(const_cast< QyncMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int QyncMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: showPresetFromMenu(); break;
        case 1: showPreset((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: readPreferences(); break;
        case 3: refreshPresets(); break;
        case 4: chooseSourceDirectory(); break;
        case 5: chooseDestinationDirectory(); break;
        case 6: chooseSourceFile(); break;
        case 7: chooseDestinationFile(); break;
        case 8: chooseLogFile(); break;
        case 9: switchSourceAndDestination(); break;
        case 10: saveSettingsToCurrentPreset(); break;
        case 11: removeCurrentPreset(); break;
        case 12: newPresetFromSettings(); break;
        case 13: importPreset(); break;
        case 14: exportPreset(); break;
        case 15: showPreferences(); break;
        case 16: simulate(); break;
        case 17: execute(); break;
        case 18: about(); break;
        case 19: aboutRsync(); break;
        default: ;
        }
        _id -= 20;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
