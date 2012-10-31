/****************************************************************************
** Meta object code from reading C++ file 'cardinvoker.h'
**
** Created: Wed Sep 19 15:58:44 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/cardinvoker.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cardinvoker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CardInvoker[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      42,   28,   12,   12, 0x0a,

 // methods: signature, parameters, type, tag, flags
      75,   66,   12,   12, 0x02,
     108,   28,   12,   12, 0x02,
     154,  135,   12,   12, 0x02,
     191,  178,   12,   12, 0x02,
     227,  213,   12,   12, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_CardInvoker[] = {
    "CardInvoker\0\0replyChanged()\0category,name\0"
    "invoke(QString,QString)\0dropdown\0"
    "populateContentDropDown(QString)\0"
    "setTarget(QString,QString)\0"
    "name,selectedIndex\0setContent(QString,int)\0"
    "fullpathname\0updateTarget(QString)\0"
    "selectedIndex\0setaswallpaper(int)\0"
};

void CardInvoker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CardInvoker *_t = static_cast<CardInvoker *>(_o);
        switch (_id) {
        case 0: _t->replyChanged(); break;
        case 1: _t->invoke((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->populateContentDropDown((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->setTarget((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: _t->setContent((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        case 5: _t->updateTarget((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->setaswallpaper((*reinterpret_cast< const int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CardInvoker::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CardInvoker::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CardInvoker,
      qt_meta_data_CardInvoker, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CardInvoker::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CardInvoker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CardInvoker::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CardInvoker))
        return static_cast<void*>(const_cast< CardInvoker*>(this));
    return QObject::qt_metacast(_clname);
}

int CardInvoker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void CardInvoker::replyChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
