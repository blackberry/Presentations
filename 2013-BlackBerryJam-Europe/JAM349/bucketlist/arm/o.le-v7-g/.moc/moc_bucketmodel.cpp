/****************************************************************************
** Meta object code from reading C++ file 'bucketmodel.h'
**
** Created: Tue Jan 8 11:20:15 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/bucketdata/bucketmodel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bucketmodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BucketModel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       3,   49, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,   13,   12,   12, 0x05,
      57,   43,   12,   12, 0x05,
     100,   87,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
     150,  126,   12,   12, 0x0a,
     196,  182,   12,   12, 0x0a,
     238,  228,   12,   12, 0x0a,
     279,  261,   12,   12, 0x0a,

 // properties: name, type, flags
      13,  312, 0x0a495103,
      43,  312, 0x0a495103,
      87,  320, 0x01495001,

 // properties: notify_signal_id
       0,
       1,
       2,

       0        // eod
};

static const char qt_meta_stringdata_BucketModel[] = {
    "BucketModel\0\0filter\0filterChanged(QString)\0"
    "jsonAssetPath\0jsonAssetPathChanged(QString)\0"
    "bucketIsFull\0bucketIsFullChanged(bool)\0"
    "selectionList,newStatus\0"
    "setStatus(QVariantList,QString)\0"
    "selectionList\0deleteBucketItems(QVariantList)\0"
    "itemTitle\0addBucketItem(QString)\0"
    "item,newItemTitle\0editBucketItem(QVariant,QString)\0"
    "QString\0bool\0"
};

void BucketModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BucketModel *_t = static_cast<BucketModel *>(_o);
        switch (_id) {
        case 0: _t->filterChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->jsonAssetPathChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->bucketIsFullChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->setStatus((*reinterpret_cast< const QVariantList(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: _t->deleteBucketItems((*reinterpret_cast< const QVariantList(*)>(_a[1]))); break;
        case 5: _t->addBucketItem((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->editBucketItem((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData BucketModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject BucketModel::staticMetaObject = {
    { &BucketListModel::staticMetaObject, qt_meta_stringdata_BucketModel,
      qt_meta_data_BucketModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BucketModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BucketModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BucketModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BucketModel))
        return static_cast<void*>(const_cast< BucketModel*>(this));
    return BucketListModel::qt_metacast(_clname);
}

int BucketModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BucketListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = filter(); break;
        case 1: *reinterpret_cast< QString*>(_v) = jsonAssetPath(); break;
        case 2: *reinterpret_cast< bool*>(_v) = bucketIsFull(); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setFilter(*reinterpret_cast< QString*>(_v)); break;
        case 1: setJsonAssetPath(*reinterpret_cast< QString*>(_v)); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void BucketModel::filterChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BucketModel::jsonAssetPathChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void BucketModel::bucketIsFullChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
