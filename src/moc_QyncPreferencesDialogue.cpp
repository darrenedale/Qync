/****************************************************************************
** Meta object code from reading C++ file 'QyncPreferencesDialogue.h'
**
** Created: Mon Jul 26 21:11:41 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "QyncPreferencesDialogue.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QyncPreferencesDialogue.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QyncPreferencesDialogue[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      25,   24,   24,   24, 0x08,
      43,   24,   24,   24, 0x08,
      69,   24,   24,   24, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QyncPreferencesDialogue[] = {
    "QyncPreferencesDialogue\0\0savePreferences()\0"
    "savePreferencesAndClose()\0chooseRsyncPath()\0"
};

const QMetaObject QyncPreferencesDialogue::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QyncPreferencesDialogue,
      qt_meta_data_QyncPreferencesDialogue, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QyncPreferencesDialogue::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QyncPreferencesDialogue::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QyncPreferencesDialogue::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QyncPreferencesDialogue))
        return static_cast<void*>(const_cast< QyncPreferencesDialogue*>(this));
    return QDialog::qt_metacast(_clname);
}

int QyncPreferencesDialogue::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: savePreferences(); break;
        case 1: savePreferencesAndClose(); break;
        case 2: chooseRsyncPath(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
