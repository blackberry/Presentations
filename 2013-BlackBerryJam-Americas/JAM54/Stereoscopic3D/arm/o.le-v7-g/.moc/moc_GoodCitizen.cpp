/****************************************************************************
** Meta object code from reading C++ file 'GoodCitizen.hpp'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/GoodCitizen.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GoodCitizen.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GoodCitizen[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       4,   44, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      25,   13,   12,   12, 0x0a,
      63,   54,   12,   12, 0x0a,
      94,   84,   12,   12, 0x0a,
     116,   84,   12,   12, 0x0a,
     145,  134,   12,   12, 0x0a,
     163,  157,   12,   12, 0x0a,

 // properties: name, type, flags
      13,  198, 0x09095103,
      54,  211, 0x0a095103,
      84,  211, 0x0a095103,
     219,  211, 0x0a095103,

       0        // eod
};

static const char qt_meta_stringdata_GoodCitizen[] = {
    "GoodCitizen\0\0objectColor\0"
    "setObjectColor(QVariantList)\0toolAxis\0"
    "setToolAxis(QString)\0touchMode\0"
    "setTouchMode(QString)\0setModel(QString)\0"
    "skipColour\0reset(bool)\0event\0"
    "onTouch(bb::cascades::TouchEvent*)\0"
    "QVariantList\0QString\0model\0"
};

void GoodCitizen::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GoodCitizen *_t = static_cast<GoodCitizen *>(_o);
        switch (_id) {
        case 0: _t->setObjectColor((*reinterpret_cast< QVariantList(*)>(_a[1]))); break;
        case 1: _t->setToolAxis((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->setTouchMode((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->setModel((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->reset((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->onTouch((*reinterpret_cast< bb::cascades::TouchEvent*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GoodCitizen::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GoodCitizen::staticMetaObject = {
    { &OpenGLView::staticMetaObject, qt_meta_stringdata_GoodCitizen,
      qt_meta_data_GoodCitizen, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GoodCitizen::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GoodCitizen::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GoodCitizen::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GoodCitizen))
        return static_cast<void*>(const_cast< GoodCitizen*>(this));
    return OpenGLView::qt_metacast(_clname);
}

int GoodCitizen::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = OpenGLView::qt_metacall(_c, _id, _a);
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
        case 0: *reinterpret_cast< QVariantList*>(_v) = objectColor(); break;
        case 1: *reinterpret_cast< QString*>(_v) = toolAxis(); break;
        case 2: *reinterpret_cast< QString*>(_v) = touchMode(); break;
        case 3: *reinterpret_cast< QString*>(_v) = model(); break;
        }
        _id -= 4;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setObjectColor(*reinterpret_cast< QVariantList*>(_v)); break;
        case 1: setToolAxis(*reinterpret_cast< QString*>(_v)); break;
        case 2: setTouchMode(*reinterpret_cast< QString*>(_v)); break;
        case 3: setModel(*reinterpret_cast< QString*>(_v)); break;
        }
        _id -= 4;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
