/****************************************************************************
** Meta object code from reading C++ file 'target.hpp'
**
** Created: Mon Sep 17 11:15:25 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/target.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'target.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Target[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       7,   99, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x05,
      29,    7,    7,    7, 0x05,
      57,    7,    7,    7, 0x05,
      70,    7,    7,    7, 0x05,
      84,    7,    7,    7, 0x05,
     106,  100,    7,    7, 0x05,
     129,    7,    7,    7, 0x05,
     145,    7,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
     166,  159,    7,    7, 0x0a,
     197,  185,    7,    7, 0x0a,
     226,  221,    7,    7, 0x0a,
     247,  243,    7,    7, 0x0a,
     268,  263,    7,    7, 0x0a,
     292,  285,    7,    7, 0x0a,
     330,  311,    7,    7, 0x0a,
     361,    7,    7,    7, 0x0a,
     370,    7,    7,    7, 0x0a,

 // properties: name, type, flags
     159,  391, 0x0a495103,
     263,  391, 0x0a495103,
     285,  391, 0x0a495103,
     243,  391, 0x0a495103,
     221,  391, 0x0a495103,
     185,  391, 0x0a495103,
     311,  391, 0x0a495103,

 // properties: notify_signal_id
       6,
       7,
       4,
       2,
       3,
       0,
       1,

       0        // eod
};

static const char qt_meta_stringdata_Target[] = {
    "Target\0\0descriptionChanged()\0"
    "contentDescriptionChanged()\0uriChanged()\0"
    "dataChanged()\0actionChanged()\0reply\0"
    "replyReceived(QString)\0targetChanged()\0"
    "mimeChanged()\0target\0setTarget(QString)\0"
    "description\0setDescription(QString)\0"
    "data\0setData(QString)\0uri\0setUri(QString)\0"
    "mime\0setMime(QString)\0action\0"
    "setAction(QString)\0contentDescription\0"
    "setContentDescription(QString)\0invoke()\0"
    "processInvokeReply()\0QString\0"
};

void Target::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Target *_t = static_cast<Target *>(_o);
        switch (_id) {
        case 0: _t->descriptionChanged(); break;
        case 1: _t->contentDescriptionChanged(); break;
        case 2: _t->uriChanged(); break;
        case 3: _t->dataChanged(); break;
        case 4: _t->actionChanged(); break;
        case 5: _t->replyReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->targetChanged(); break;
        case 7: _t->mimeChanged(); break;
        case 8: _t->setTarget((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->setDescription((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->setData((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->setUri((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 12: _t->setMime((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 13: _t->setAction((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 14: _t->setContentDescription((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 15: _t->invoke(); break;
        case 16: _t->processInvokeReply(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Target::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Target::staticMetaObject = {
    { &bb::cascades::CustomControl::staticMetaObject, qt_meta_stringdata_Target,
      qt_meta_data_Target, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Target::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Target::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Target::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Target))
        return static_cast<void*>(const_cast< Target*>(this));
    typedef bb::cascades::CustomControl QMocSuperClass;
    return QMocSuperClass::qt_metacast(_clname);
}

int Target::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    typedef bb::cascades::CustomControl QMocSuperClass;
    _id = QMocSuperClass::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = target(); break;
        case 1: *reinterpret_cast< QString*>(_v) = mime(); break;
        case 2: *reinterpret_cast< QString*>(_v) = action(); break;
        case 3: *reinterpret_cast< QString*>(_v) = uri(); break;
        case 4: *reinterpret_cast< QString*>(_v) = data(); break;
        case 5: *reinterpret_cast< QString*>(_v) = description(); break;
        case 6: *reinterpret_cast< QString*>(_v) = contentDescription(); break;
        }
        _id -= 7;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setTarget(*reinterpret_cast< QString*>(_v)); break;
        case 1: setMime(*reinterpret_cast< QString*>(_v)); break;
        case 2: setAction(*reinterpret_cast< QString*>(_v)); break;
        case 3: setUri(*reinterpret_cast< QString*>(_v)); break;
        case 4: setData(*reinterpret_cast< QString*>(_v)); break;
        case 5: setDescription(*reinterpret_cast< QString*>(_v)); break;
        case 6: setContentDescription(*reinterpret_cast< QString*>(_v)); break;
        }
        _id -= 7;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 7;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void Target::descriptionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Target::contentDescriptionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void Target::uriChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void Target::dataChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void Target::actionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void Target::replyReceived(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Target::targetChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void Target::mimeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}
QT_END_MOC_NAMESPACE
