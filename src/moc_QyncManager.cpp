/****************************************************************************
** Meta object code from reading C++ file 'QyncManager.h'
**
** Created: Mon Jul 26 21:11:39 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "QyncManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QyncManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QyncManager[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      26,   13,   12,   12, 0x05,
      55,   12,   12,   12, 0x05,
      71,   12,   12,   12, 0x05,
      88,   12,   12,   12, 0x05,
     117,  109,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
     151,   12,  146,   12, 0x0a,
     170,  165,  146,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QyncManager[] = {
    "QyncManager\0\0preset,index\0"
    "presetAdded(QyncPreset*,int)\0"
    "presetRemoved()\0presetsChanged()\0"
    "preferencesChanged()\0process\0"
    "processStarted(QyncProcess*)\0bool\0"
    "loadPresets()\0path\0loadPresets(QString)\0"
};

const QMetaObject QyncManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QyncManager,
      qt_meta_data_QyncManager, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QyncManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QyncManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QyncManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QyncManager))
        return static_cast<void*>(const_cast< QyncManager*>(this));
    return QObject::qt_metacast(_clname);
}

int QyncManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: presetAdded((*reinterpret_cast< QyncPreset*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: presetRemoved(); break;
        case 2: presetsChanged(); break;
        case 3: preferencesChanged(); break;
        case 4: processStarted((*reinterpret_cast< QyncProcess*(*)>(_a[1]))); break;
        case 5: { bool _r = loadPresets();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 6: { bool _r = loadPresets((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void QyncManager::presetAdded(QyncPreset * _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QyncManager::presetRemoved()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void QyncManager::presetsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void QyncManager::preferencesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void QyncManager::processStarted(QyncProcess * _t1)const
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(const_cast< QyncManager *>(this), &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
