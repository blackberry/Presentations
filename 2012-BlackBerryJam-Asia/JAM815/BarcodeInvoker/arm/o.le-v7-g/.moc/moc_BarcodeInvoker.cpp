/****************************************************************************
** Meta object code from reading C++ file 'BarcodeInvoker.hpp'
**
** Created: Thu Nov 15 14:55:16 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/BarcodeInvoker.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BarcodeInvoker.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BarcodeInvoker[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      24,   16,   15,   15, 0x0a,
      69,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_BarcodeInvoker[] = {
    "BarcodeInvoker\0\0message\0"
    "onChildCardDone(bb::system::CardDoneMessage)\0"
    "onInvokeButtonClicked()\0"
};

void BarcodeInvoker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BarcodeInvoker *_t = static_cast<BarcodeInvoker *>(_o);
        switch (_id) {
        case 0: _t->onChildCardDone((*reinterpret_cast< const bb::system::CardDoneMessage(*)>(_a[1]))); break;
        case 1: _t->onInvokeButtonClicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData BarcodeInvoker::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject BarcodeInvoker::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BarcodeInvoker,
      qt_meta_data_BarcodeInvoker, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BarcodeInvoker::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BarcodeInvoker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BarcodeInvoker::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BarcodeInvoker))
        return static_cast<void*>(const_cast< BarcodeInvoker*>(this));
    return QObject::qt_metacast(_clname);
}

int BarcodeInvoker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
