/****************************************************************************
** Meta object code from reading C++ file 'viewer.hpp'
**
** Created: Sun Sep 16 19:17:43 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/viewer.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'viewer.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Viewer[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       1,  114, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x05,
      25,    7,    7,    7, 0x05,
      40,    7,    7,    7, 0x05,
      61,    7,    7,    7, 0x05,
      87,   79,    7,    7, 0x05,
     122,  112,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
     153,    7,  145,    7, 0x0a,
     164,    7,    7,    7, 0x0a,
     173,    7,    7,    7, 0x0a,
     197,  187,    7,    7, 0x0a,
     234,  187,    7,    7, 0x0a,
     276,  187,    7,    7, 0x0a,
     299,    7,    7,    7, 0x0a,
     316,    7,    7,    7, 0x0a,
     336,  332,    7,    7, 0x0a,
     378,    7,    7,    7, 0x0a,
     400,  391,    7,    7, 0x0a,
     438,  391,    7,    7, 0x0a,
     494,  460,    7,    7, 0x08,
     542,    7,    7,    7, 0x08,

 // properties: name, type, flags
     391,  145, 0x0a495801,

 // properties: notify_signal_id
       3,

       0        // eod
};

static const char qt_meta_stringdata_Viewer[] = {
    "Viewer\0\0viewerAttached()\0viewerClosed()\0"
    "viewerStartupError()\0windowIdChanged()\0"
    "msgText\0messageReceived(QString)\0"
    "replyText\0replyReceived(QString)\0"
    "QString\0windowId()\0invoke()\0closeViewer()\0"
    "name,data\0sendMessageToViewer(QString,QString)\0"
    "sendQueryResponseMessage(QString,QString)\0"
    "reply(QString,QString)\0invokeFinished()\0"
    "queryFinished()\0msg\0"
    "received(bb::system::InvokeViewerMessage)\0"
    "replyReady()\0windowId\0"
    "onViewerCloseRequestReceived(QString)\0"
    "viewerClosed(QString)\0"
    "windowHandle,windowGroup,windowId\0"
    "onViewerAttach(screen_window_t,QString,QString)\0"
    "onViewerDetach()\0"
};

void Viewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Viewer *_t = static_cast<Viewer *>(_o);
        switch (_id) {
        case 0: _t->viewerAttached(); break;
        case 1: _t->viewerClosed(); break;
        case 2: _t->viewerStartupError(); break;
        case 3: _t->windowIdChanged(); break;
        case 4: _t->messageReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->replyReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: { QString _r = _t->windowId();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 7: _t->invoke(); break;
        case 8: _t->closeViewer(); break;
        case 9: _t->sendMessageToViewer((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 10: _t->sendQueryResponseMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 11: _t->reply((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 12: _t->invokeFinished(); break;
        case 13: _t->queryFinished(); break;
        case 14: _t->received((*reinterpret_cast< const bb::system::InvokeViewerMessage(*)>(_a[1]))); break;
        case 15: _t->replyReady(); break;
        case 16: _t->onViewerCloseRequestReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 17: _t->viewerClosed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 18: _t->onViewerAttach((*reinterpret_cast< screen_window_t(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 19: _t->onViewerDetach(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Viewer::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Viewer::staticMetaObject = {
    { &Target::staticMetaObject, qt_meta_stringdata_Viewer,
      qt_meta_data_Viewer, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Viewer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Viewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Viewer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Viewer))
        return static_cast<void*>(const_cast< Viewer*>(this));
    return Target::qt_metacast(_clname);
}

int Viewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Target::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = windowId(); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::WriteProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void Viewer::viewerAttached()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Viewer::viewerClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void Viewer::viewerStartupError()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void Viewer::windowIdChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void Viewer::messageReceived(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Viewer::replyReceived(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
