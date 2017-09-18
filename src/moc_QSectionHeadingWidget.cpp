/****************************************************************************
** Meta object code from reading C++ file 'QSectionHeadingWidget.h'
**
** Created: Sun Jun 20 10:47:22 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../QWidgets/QSectionHeadingWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QSectionHeadingWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QSectionHeadingWidget[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   22,   22,   22, 0x05,
      34,   22,   22,   22, 0x05,
      46,   22,   22,   22, 0x05,
      60,   22,   22,   22, 0x05,

 // slots: signature, parameters, type, tag, flags
      85,   77,   22,   22, 0x0a,
     107,  105,   22,   22, 0x0a,
     125,   22,   22,   22, 0x0a,
     134,   22,   22,   22, 0x0a,
     143,   22,   22,   22, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QSectionHeadingWidget[] = {
    "QSectionHeadingWidget\0\0expanded()\0"
    "collapsed()\0toggled(bool)\0closeRequested()\0"
    "heading\0setHeading(QString)\0s\0"
    "setExpanded(bool)\0toggle()\0expand()\0"
    "collapse()\0"
};

const QMetaObject QSectionHeadingWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QSectionHeadingWidget,
      qt_meta_data_QSectionHeadingWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QSectionHeadingWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QSectionHeadingWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QSectionHeadingWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QSectionHeadingWidget))
        return static_cast<void*>(const_cast< QSectionHeadingWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int QSectionHeadingWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: expanded(); break;
        case 1: collapsed(); break;
        case 2: toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: closeRequested(); break;
        case 4: setHeading((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: setExpanded((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: toggle(); break;
        case 7: expand(); break;
        case 8: collapse(); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void QSectionHeadingWidget::expanded()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void QSectionHeadingWidget::collapsed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void QSectionHeadingWidget::toggled(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QSectionHeadingWidget::closeRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
