/****************************************************************************
** Meta object code from reading C++ file 'GoodCitizenApp.hpp'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/GoodCitizenApp.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GoodCitizenApp.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GoodCitizenApp[] = {

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
      28,   16,   15,   15, 0x0a,
      57,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GoodCitizenApp[] = {
    "GoodCitizenApp\0\0layoutFrame\0"
    "onLayoutFrameChanged(QRectF)\0shutdown()\0"
};

void GoodCitizenApp::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GoodCitizenApp *_t = static_cast<GoodCitizenApp *>(_o);
        switch (_id) {
        case 0: _t->onLayoutFrameChanged((*reinterpret_cast< const QRectF(*)>(_a[1]))); break;
        case 1: _t->shutdown(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GoodCitizenApp::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GoodCitizenApp::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GoodCitizenApp,
      qt_meta_data_GoodCitizenApp, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GoodCitizenApp::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GoodCitizenApp::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GoodCitizenApp::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GoodCitizenApp))
        return static_cast<void*>(const_cast< GoodCitizenApp*>(this));
    return QObject::qt_metacast(_clname);
}

int GoodCitizenApp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
