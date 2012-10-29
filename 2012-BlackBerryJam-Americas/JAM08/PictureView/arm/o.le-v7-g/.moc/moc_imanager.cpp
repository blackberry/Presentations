/****************************************************************************
** Meta object code from reading C++ file 'imanager.h'
**
** Created: Mon Sep 17 11:15:28 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/imanager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'imanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_IManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      39,   25,    9,    9, 0x0a,

 // methods: signature, parameters, type, tag, flags
      72,   63,    9,    9, 0x02,
     105,   25,    9,    9, 0x02,
     151,  132,    9,    9, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_IManager[] = {
    "IManager\0\0replyChanged()\0category,name\0"
    "invoke(QString,QString)\0dropdown\0"
    "populateContentDropDown(QString)\0"
    "setTarget(QString,QString)\0"
    "name,selectedIndex\0setContent(QString,int)\0"
};

void IManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        IManager *_t = static_cast<IManager *>(_o);
        switch (_id) {
        case 0: _t->replyChanged(); break;
        case 1: _t->invoke((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->populateContentDropDown((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->setTarget((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: _t->setContent((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData IManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject IManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_IManager,
      qt_meta_data_IManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &IManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *IManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *IManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_IManager))
        return static_cast<void*>(const_cast< IManager*>(this));
    return QObject::qt_metacast(_clname);
}

int IManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void IManager::replyChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
