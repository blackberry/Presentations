/****************************************************************************
** Meta object code from reading C++ file 'registrationhandler.h'
**
** Created: Tue Jan 8 11:20:12 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/bucketbbm/registrationhandler.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'registrationhandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_bucketbbm__RegistrationHandler[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       2,   29, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      50,   32,   31,   31, 0x05,
      88,   77,   31,   31, 0x05,

 // slots: signature, parameters, type, tag, flags
     118,  112,   31,   31, 0x08,

 // properties: name, type, flags
     189,  181, 0x0a495103,
      77,  200, 0x01495001,

 // properties: notify_signal_id
       0,
       1,

       0        // eod
};

static const char qt_meta_stringdata_bucketbbm__RegistrationHandler[] = {
    "bucketbbm::RegistrationHandler\0\0"
    "statusTextChanged\0statusTextChanged(QString)\0"
    "registered\0registeredChanged(bool)\0"
    "state\0"
    "registrationStatus(bb::platform::bbm::RegistrationState::Type)\0"
    "QString\0statusText\0bool\0"
};

void bucketbbm::RegistrationHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        RegistrationHandler *_t = static_cast<RegistrationHandler *>(_o);
        switch (_id) {
        case 0: _t->statusTextChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->registeredChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->registrationStatus((*reinterpret_cast< bb::platform::bbm::RegistrationState::Type(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData bucketbbm::RegistrationHandler::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject bucketbbm::RegistrationHandler::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_bucketbbm__RegistrationHandler,
      qt_meta_data_bucketbbm__RegistrationHandler, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &bucketbbm::RegistrationHandler::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *bucketbbm::RegistrationHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *bucketbbm::RegistrationHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_bucketbbm__RegistrationHandler))
        return static_cast<void*>(const_cast< RegistrationHandler*>(this));
    return QObject::qt_metacast(_clname);
}

int bucketbbm::RegistrationHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = statusText(); break;
        case 1: *reinterpret_cast< bool*>(_v) = registered(); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setStatusText(*reinterpret_cast< QString*>(_v)); break;
        }
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
void bucketbbm::RegistrationHandler::statusTextChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void bucketbbm::RegistrationHandler::registeredChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
