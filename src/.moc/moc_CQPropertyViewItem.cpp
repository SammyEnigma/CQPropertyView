/****************************************************************************
** Meta object code from reading C++ file 'CQPropertyViewItem.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../include/CQPropertyViewItem.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CQPropertyViewItem.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CQPropertyViewItem_t {
    QByteArrayData data[11];
    char stringdata0[89];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CQPropertyViewItem_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CQPropertyViewItem_t qt_meta_stringdata_CQPropertyViewItem = {
    {
QT_MOC_LITERAL(0, 0, 18), // "CQPropertyViewItem"
QT_MOC_LITERAL(1, 19, 12), // "valueChanged"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 3), // "obj"
QT_MOC_LITERAL(4, 37, 4), // "name"
QT_MOC_LITERAL(5, 42, 11), // "updateValue"
QT_MOC_LITERAL(6, 54, 5), // "alias"
QT_MOC_LITERAL(7, 60, 8), // "editable"
QT_MOC_LITERAL(8, 69, 6), // "hidden"
QT_MOC_LITERAL(9, 76, 6), // "inside"
QT_MOC_LITERAL(10, 83, 5) // "dirty"

    },
    "CQPropertyViewItem\0valueChanged\0\0obj\0"
    "name\0updateValue\0alias\0editable\0hidden\0"
    "inside\0dirty"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQPropertyViewItem[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       6,   30, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   29,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QObjectStar, QMetaType::QString,    3,    4,

 // slots: parameters
    QMetaType::Void,

 // properties: name, type, flags
       4, QMetaType::QString, 0x00095103,
       6, QMetaType::QString, 0x00095103,
       7, QMetaType::Bool, 0x00095103,
       8, QMetaType::Bool, 0x00095103,
       9, QMetaType::Bool, 0x00095103,
      10, QMetaType::Bool, 0x00095103,

       0        // eod
};

void CQPropertyViewItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CQPropertyViewItem *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->valueChanged((*reinterpret_cast< QObject*(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->updateValue(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CQPropertyViewItem::*)(QObject * , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CQPropertyViewItem::valueChanged)) {
                *result = 0;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<CQPropertyViewItem *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->name(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->alias(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->isEditable(); break;
        case 3: *reinterpret_cast< bool*>(_v) = _t->isHidden(); break;
        case 4: *reinterpret_cast< bool*>(_v) = _t->isInside(); break;
        case 5: *reinterpret_cast< bool*>(_v) = _t->isDirty(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<CQPropertyViewItem *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setName(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->setAlias(*reinterpret_cast< QString*>(_v)); break;
        case 2: _t->setEditable(*reinterpret_cast< bool*>(_v)); break;
        case 3: _t->setHidden(*reinterpret_cast< bool*>(_v)); break;
        case 4: _t->setInside(*reinterpret_cast< bool*>(_v)); break;
        case 5: _t->setDirty(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject CQPropertyViewItem::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CQPropertyViewItem.data,
    qt_meta_data_CQPropertyViewItem,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CQPropertyViewItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQPropertyViewItem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CQPropertyViewItem.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CQPropertyViewItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 6;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void CQPropertyViewItem::valueChanged(QObject * _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
