/****************************************************************************
** Meta object code from reading C++ file 'profilebox.h'
**
** Created: Tue Jan 8 11:20:10 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/bucketbbm/profilebox.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'profilebox.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_bucketbbm__ProfileBox[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      37,   23,   22,   22, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_bucketbbm__ProfileBox[] = {
    "bucketbbm::ProfileBox\0\0text,iconPath\0"
    "createItem(QString,QString)\0"
};

void bucketbbm::ProfileBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ProfileBox *_t = static_cast<ProfileBox *>(_o);
        switch (_id) {
        case 0: _t->createItem((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData bucketbbm::ProfileBox::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject bucketbbm::ProfileBox::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_bucketbbm__ProfileBox,
      qt_meta_data_bucketbbm__ProfileBox, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &bucketbbm::ProfileBox::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *bucketbbm::ProfileBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *bucketbbm::ProfileBox::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_bucketbbm__ProfileBox))
        return static_cast<void*>(const_cast< ProfileBox*>(this));
    return QObject::qt_metacast(_clname);
}

int bucketbbm::ProfileBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
