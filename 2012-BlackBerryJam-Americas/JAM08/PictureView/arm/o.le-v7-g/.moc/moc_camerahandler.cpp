/****************************************************************************
** Meta object code from reading C++ file 'camerahandler.h'
**
** Created: Tue Sep 18 17:11:45 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/camerahandler.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'camerahandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CameraHandler[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x08,
      31,   14,   14,   14, 0x08,
      48,   14,   14,   14, 0x08,
      64,   14,   14,   14, 0x08,
      82,   14,   14,   14, 0x08,

 // methods: signature, parameters, type, tag, flags
     101,   14,   14,   14, 0x02,
     115,   14,   14,   14, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_CameraHandler[] = {
    "CameraHandler\0\0onOpenSuccess()\0"
    "onShutterFired()\0onButtonClick()\0"
    "onStopVfSuccess()\0onStartVfSuccess()\0"
    "camerastart()\0camerastop()\0"
};

void CameraHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CameraHandler *_t = static_cast<CameraHandler *>(_o);
        switch (_id) {
        case 0: _t->onOpenSuccess(); break;
        case 1: _t->onShutterFired(); break;
        case 2: _t->onButtonClick(); break;
        case 3: _t->onStopVfSuccess(); break;
        case 4: _t->onStartVfSuccess(); break;
        case 5: _t->camerastart(); break;
        case 6: _t->camerastop(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData CameraHandler::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CameraHandler::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CameraHandler,
      qt_meta_data_CameraHandler, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CameraHandler::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CameraHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CameraHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CameraHandler))
        return static_cast<void*>(const_cast< CameraHandler*>(this));
    return QObject::qt_metacast(_clname);
}

int CameraHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
