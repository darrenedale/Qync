/****************************************************************************
** Meta object code from reading C++ file 'QSectionWidget.h'
**
** Created: Sun Jun 20 10:47:21 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../QWidgets/QSectionWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QSectionWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QSectionWidget[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       9,   69, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      22,   16,   15,   15, 0x05,
      43,   16,   15,   15, 0x05,
      80,   65,   15,   15, 0x05,
     105,   16,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
     145,  132,   15,   15, 0x0a,
     174,   16,   15,   15, 0x0a,
     193,   16,   15,   15, 0x0a,
     214,   15,   15,   15, 0x0a,
     234,   15,   15,   15, 0x0a,
     265,  256,   15,   15, 0x08,
     290,   15,   15,   15, 0x08,

 // properties: name, type, flags
     330,  318, 0x0009510b,
     355,  342, 0x0009510b,
     384,  368, 0x0009510b,
     406,  400, 0x15095103,
     419,  415, 0x02095103,
     452,  434, 0x0009510b,
     467,  462, 0x01095103,
     485,  462, 0x01095103,
     501,  462, 0x01095103,

       0        // eod
};

static const char qt_meta_stringdata_QSectionWidget[] = {
    "QSectionWidget\0\0index\0sectionExpanded(int)\0"
    "sectionCollapsed(int)\0index,newState\0"
    "sectionToggled(int,bool)\0"
    "sectionCloseRequested(int)\0index,enable\0"
    "setSectionExpanded(int,bool)\0"
    "expandSection(int)\0collapseSection(int)\0"
    "expandAllSections()\0collapseAllSections()\0"
    "newState\0slotSectionToggled(bool)\0"
    "slotSectionCloseRequested()\0Orientation\0"
    "orientation\0HeadingShape\0headingShape\0"
    "TogglePlacement\0togglePlacement\0QSize\0"
    "iconSize\0int\0headingSpacing\0"
    "Qt::TextElideMode\0elideMode\0bool\0"
    "sectionsCloseable\0sectionsMovable\0"
    "sectionsCollapsible\0"
};

const QMetaObject QSectionWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QSectionWidget,
      qt_meta_data_QSectionWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QSectionWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QSectionWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QSectionWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QSectionWidget))
        return static_cast<void*>(const_cast< QSectionWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int QSectionWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sectionExpanded((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: sectionCollapsed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: sectionToggled((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 3: sectionCloseRequested((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: setSectionExpanded((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 5: expandSection((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: collapseSection((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: expandAllSections(); break;
        case 8: collapseAllSections(); break;
        case 9: slotSectionToggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: slotSectionCloseRequested(); break;
        default: ;
        }
        _id -= 11;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< Orientation*>(_v) = orientation(); break;
        case 1: *reinterpret_cast< HeadingShape*>(_v) = headingShape(); break;
        case 2: *reinterpret_cast< TogglePlacement*>(_v) = togglePlacement(); break;
        case 3: *reinterpret_cast< QSize*>(_v) = iconSize(); break;
        case 4: *reinterpret_cast< int*>(_v) = headingSpacing(); break;
        case 5: *reinterpret_cast< Qt::TextElideMode*>(_v) = elideMode(); break;
        case 6: *reinterpret_cast< bool*>(_v) = sectionsCloseable(); break;
        case 7: *reinterpret_cast< bool*>(_v) = sectionsMovable(); break;
        case 8: *reinterpret_cast< bool*>(_v) = sectionsCollapsible(); break;
        }
        _id -= 9;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setOrientation(*reinterpret_cast< Orientation*>(_v)); break;
        case 1: setHeadingShape(*reinterpret_cast< HeadingShape*>(_v)); break;
        case 2: setTogglePlacement(*reinterpret_cast< TogglePlacement*>(_v)); break;
        case 3: setIconSize(*reinterpret_cast< QSize*>(_v)); break;
        case 4: setHeadingSpacing(*reinterpret_cast< int*>(_v)); break;
        case 5: setElideMode(*reinterpret_cast< Qt::TextElideMode*>(_v)); break;
        case 6: setSectionsCloseable(*reinterpret_cast< bool*>(_v)); break;
        case 7: setSectionsMovable(*reinterpret_cast< bool*>(_v)); break;
        case 8: setSectionsCollapsible(*reinterpret_cast< bool*>(_v)); break;
        }
        _id -= 9;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 9;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void QSectionWidget::sectionExpanded(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QSectionWidget::sectionCollapsed(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QSectionWidget::sectionToggled(int _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QSectionWidget::sectionCloseRequested(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
