/****************************************************************************
** Meta object code from reading C++ file 'QyncProcess.h'
**
** Created: Mon Jul 26 21:11:39 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "QyncProcess.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QyncProcess.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QyncProcess[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      12,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,
      28,   23,   12,   12, 0x05,
      61,   52,   12,   12, 0x05,
      79,   52,   12,   12, 0x05,
     102,   52,   12,   12, 0x05,
     128,  123,   12,   12, 0x05,
     164,  160,   12,   12, 0x05,
     182,  160,   12,   12, 0x05,
     203,  160,   12,   12, 0x05,
     223,  219,   12,   12, 0x05,
     243,  238,   12,   12, 0x05,
     274,  238,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
     304,   12,   12,   12, 0x08,
     318,   12,   12,   12, 0x08,
     332,   12,   12,   12, 0x08,
     350,   12,   12,   12, 0x0a,
     358,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QyncProcess[] = {
    "QyncProcess\0\0started()\0item\0"
    "newItemStarted(QString)\0progress\0"
    "itemProgress(int)\0itemProgressBytes(int)\0"
    "overallProgress(int)\0code\0"
    "finished(QyncProcess::ExitCode)\0msg\0"
    "finished(QString)\0interrupted(QString)\0"
    "failed(QString)\0err\0error(QString)\0"
    "data\0standardOutputUpdated(QString)\0"
    "standardErrorUpdated(QString)\0"
    "parseStdout()\0parseStderr()\0"
    "processFinished()\0start()\0stop()\0"
};

const QMetaObject QyncProcess::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QyncProcess,
      qt_meta_data_QyncProcess, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QyncProcess::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QyncProcess::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QyncProcess::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QyncProcess))
        return static_cast<void*>(const_cast< QyncProcess*>(this));
    return QObject::qt_metacast(_clname);
}

int QyncProcess::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: started(); break;
        case 1: newItemStarted((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: itemProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: itemProgressBytes((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: overallProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: finished((*reinterpret_cast< QyncProcess::ExitCode(*)>(_a[1]))); break;
        case 6: finished((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: interrupted((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: failed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: error((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: standardOutputUpdated((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: standardErrorUpdated((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: parseStdout(); break;
        case 13: parseStderr(); break;
        case 14: processFinished(); break;
        case 15: start(); break;
        case 16: stop(); break;
        default: ;
        }
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void QyncProcess::started()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void QyncProcess::newItemStarted(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QyncProcess::itemProgress(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QyncProcess::itemProgressBytes(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QyncProcess::overallProgress(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void QyncProcess::finished(QyncProcess::ExitCode _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void QyncProcess::finished(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void QyncProcess::interrupted(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void QyncProcess::failed(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void QyncProcess::error(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void QyncProcess::standardOutputUpdated(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void QyncProcess::standardErrorUpdated(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}
QT_END_MOC_NAMESPACE
