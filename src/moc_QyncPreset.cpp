/****************************************************************************
** Meta object code from reading C++ file 'QyncPreset.h'
**
** Created: Mon Jul 26 21:11:39 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "QyncPreset.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QyncPreset.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QyncPreset[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
      17,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      16,   11, 0x01095103,
      29,   11, 0x01095103,
      49,   11, 0x01095103,
      63,   11, 0x01095103,
      77,   11, 0x01095103,
      98,   11, 0x01095103,
     114,   11, 0x01095103,
     137,   11, 0x01095103,
     153,   11, 0x01095103,
     174,   11, 0x01095103,
     197,   11, 0x01095103,
     209,   11, 0x01095103,
     232,   11, 0x01095103,
     258,   11, 0x01095103,
     280,   11, 0x01095103,
     305,   11, 0x01095103,
     333,  325, 0x0a095103,

       0        // eod
};

static const char qt_meta_stringdata_QyncPreset[] = {
    "QyncPreset\0bool\0preserveTime\0"
    "preservePermissions\0preserveOwner\0"
    "preserveGroup\0windowsCompatability\0"
    "honourDeletions\0alwaysCompareChecksums\0"
    "preserveDevices\0keepPartialTransfers\0"
    "copySymlinksAsSymlinks\0makeBackups\0"
    "useTransferCompression\0onlyUpdateExistingEntries\0"
    "dontMapUsersAndGroups\0copyHardlinksAsHardlinks\0"
    "showItemisedChanges\0QString\0logFile\0"
};

const QMetaObject QyncPreset::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QyncPreset,
      qt_meta_data_QyncPreset, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QyncPreset::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QyncPreset::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QyncPreset::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QyncPreset))
        return static_cast<void*>(const_cast< QyncPreset*>(this));
    return QObject::qt_metacast(_clname);
}

int QyncPreset::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = preserveTime(); break;
        case 1: *reinterpret_cast< bool*>(_v) = preservePermissions(); break;
        case 2: *reinterpret_cast< bool*>(_v) = preserveOwner(); break;
        case 3: *reinterpret_cast< bool*>(_v) = preserveGroup(); break;
        case 4: *reinterpret_cast< bool*>(_v) = windowsCompatability(); break;
        case 5: *reinterpret_cast< bool*>(_v) = honourDeletions(); break;
        case 6: *reinterpret_cast< bool*>(_v) = alwaysCompareChecksums(); break;
        case 7: *reinterpret_cast< bool*>(_v) = preserveDevices(); break;
        case 8: *reinterpret_cast< bool*>(_v) = keepPartialTransfers(); break;
        case 9: *reinterpret_cast< bool*>(_v) = copySymlinksAsSymlinks(); break;
        case 10: *reinterpret_cast< bool*>(_v) = makeBackups(); break;
        case 11: *reinterpret_cast< bool*>(_v) = useTransferCompression(); break;
        case 12: *reinterpret_cast< bool*>(_v) = onlyUpdateExistingEntries(); break;
        case 13: *reinterpret_cast< bool*>(_v) = dontMapUsersAndGroups(); break;
        case 14: *reinterpret_cast< bool*>(_v) = copyHardlinksAsHardlinks(); break;
        case 15: *reinterpret_cast< bool*>(_v) = showItemisedChanges(); break;
        case 16: *reinterpret_cast< QString*>(_v) = logFile(); break;
        }
        _id -= 17;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setPreserveTime(*reinterpret_cast< bool*>(_v)); break;
        case 1: setPreservePermissions(*reinterpret_cast< bool*>(_v)); break;
        case 2: setPreserveOwner(*reinterpret_cast< bool*>(_v)); break;
        case 3: setPreserveGroup(*reinterpret_cast< bool*>(_v)); break;
        case 4: setWindowsCompatability(*reinterpret_cast< bool*>(_v)); break;
        case 5: setHonourDeletions(*reinterpret_cast< bool*>(_v)); break;
        case 6: setAlwaysCompareChecksums(*reinterpret_cast< bool*>(_v)); break;
        case 7: setPreserveDevices(*reinterpret_cast< bool*>(_v)); break;
        case 8: setKeepPartialTransfers(*reinterpret_cast< bool*>(_v)); break;
        case 9: setCopySymlinksAsSymlinks(*reinterpret_cast< bool*>(_v)); break;
        case 10: setMakeBackups(*reinterpret_cast< bool*>(_v)); break;
        case 11: setUseTransferCompression(*reinterpret_cast< bool*>(_v)); break;
        case 12: setOnlyUpdateExistingEntries(*reinterpret_cast< bool*>(_v)); break;
        case 13: setDontMapUsersAndGroups(*reinterpret_cast< bool*>(_v)); break;
        case 14: setCopyHardlinksAsHardlinks(*reinterpret_cast< bool*>(_v)); break;
        case 15: setShowItemisedChanges(*reinterpret_cast< bool*>(_v)); break;
        case 16: setLogFile(*reinterpret_cast< QString*>(_v)); break;
        }
        _id -= 17;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 17;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 17;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 17;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 17;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 17;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 17;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
