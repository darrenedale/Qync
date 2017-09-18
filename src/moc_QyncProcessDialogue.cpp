/****************************************************************************
** Meta object code from reading C++ file 'QyncProcessDialogue.h'
**
** Created: Mon Jul 26 21:11:40 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "QyncProcessDialogue.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QyncProcessDialogue.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QyncProcessDialogue[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      23,   21,   20,   20, 0x08,
      45,   21,   20,   20, 0x08,
      70,   67,   20,   20, 0x08,
      99,   94,   20,   20, 0x08,
     129,   67,   20,   20, 0x08,
     156,   20,   20,   20, 0x08,
     169,   20,   20,   20, 0x08,
     190,  186,   20,   20, 0x08,
     215,   20,   20,   20, 0x28,
     233,  186,   20,   20, 0x08,
     261,   20,   20,   20, 0x28,
     282,  186,   20,   20, 0x08,
     305,   20,   20,   20, 0x28,
     325,  321,   20,   20, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QyncProcessDialogue[] = {
    "QyncProcessDialogue\0\0s\0updateStdout(QString)\0"
    "updateStderr(QString)\0pc\0"
    "updateItemProgress(int)\0item\0"
    "updateItemInProgress(QString)\0"
    "updateOverallProgress(int)\0saveOutput()\0"
    "processStarted()\0msg\0processFinished(QString)\0"
    "processFinished()\0processInterrupted(QString)\0"
    "processInterrupted()\0processFailed(QString)\0"
    "processFailed()\0err\0showError(QString)\0"
};

const QMetaObject QyncProcessDialogue::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QyncProcessDialogue,
      qt_meta_data_QyncProcessDialogue, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QyncProcessDialogue::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QyncProcessDialogue::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QyncProcessDialogue::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QyncProcessDialogue))
        return static_cast<void*>(const_cast< QyncProcessDialogue*>(this));
    return QDialog::qt_metacast(_clname);
}

int QyncProcessDialogue::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: updateStdout((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: updateStderr((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: updateItemProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: updateItemInProgress((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: updateOverallProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: saveOutput(); break;
        case 6: processStarted(); break;
        case 7: processFinished((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: processFinished(); break;
        case 9: processInterrupted((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: processInterrupted(); break;
        case 11: processFailed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 12: processFailed(); break;
        case 13: showError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
