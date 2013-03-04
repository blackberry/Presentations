/****************************************************************************
** Meta object code from reading C++ file 'bucketbbmmanager.h'
**
** Created: Tue Jan 8 11:20:09 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/bucketbbm/bucketbbmmanager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bucketbbmmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_bucketbbm__BucketBBMManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       2,   44, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      43,   29,   28,   28, 0x05,
      89,   70,   28,   28, 0x05,

 // slots: signature, parameters, type, tag, flags
     135,  124,   28,   28, 0x08,
     172,  161,   28,   28, 0x08,

 // methods: signature, parameters, type, tag, flags
     201,   28,   28,   28, 0x02,
     228,  215,   28,   28, 0x02,

 // properties: name, type, flags
      70,  258, 0x0a495001,
      29,  266, 0x01495001,

 // properties: notify_signal_id
       1,
       0,

       0        // eod
};

static const char qt_meta_stringdata_bucketbbm__BucketBBMManager[] = {
    "bucketbbm::BucketBBMManager\0\0appRegistered\0"
    "appRegisteredChanged(bool)\0"
    "registerStatusText\0"
    "registerStatusTextChanged(QString)\0"
    "registered\0onRegisteredChanged(bool)\0"
    "statusText\0onStatusTextChanged(QString)\0"
    "registerApp()\0message,icon\0"
    "updateStatus(QString,QString)\0QString\0"
    "bool\0"
};

void bucketbbm::BucketBBMManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BucketBBMManager *_t = static_cast<BucketBBMManager *>(_o);
        switch (_id) {
        case 0: _t->appRegisteredChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->registerStatusTextChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->onRegisteredChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->onStatusTextChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->registerApp(); break;
        case 5: _t->updateStatus((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData bucketbbm::BucketBBMManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject bucketbbm::BucketBBMManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_bucketbbm__BucketBBMManager,
      qt_meta_data_bucketbbm__BucketBBMManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &bucketbbm::BucketBBMManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *bucketbbm::BucketBBMManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *bucketbbm::BucketBBMManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_bucketbbm__BucketBBMManager))
        return static_cast<void*>(const_cast< BucketBBMManager*>(this));
    return QObject::qt_metacast(_clname);
}

int bucketbbm::BucketBBMManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = registerStatusText(); break;
        case 1: *reinterpret_cast< bool*>(_v) = appRegistered(); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::WriteProperty) {
        _id -= 2;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void bucketbbm::BucketBBMManager::appRegisteredChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void bucketbbm::BucketBBMManager::registerStatusTextChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
