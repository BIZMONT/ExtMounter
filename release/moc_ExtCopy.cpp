/****************************************************************************
** Meta object code from reading C++ file 'ExtCopy.h'
**
** Created: Sun 31. May 22:44:35 2015
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../ExtCopy.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ExtCopy.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ExtCopyProcess[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      42,   16,   15,   15, 0x05,
      91,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
     106,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ExtCopyProcess[] = {
    "ExtCopyProcess\0\0file,from,to,copied,total\0"
    "sig_updateui(QString&,QString&,QString&,int,int)\0"
    "sig_copydone()\0slot_cancelprocess()\0"
};

void ExtCopyProcess::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ExtCopyProcess *_t = static_cast<ExtCopyProcess *>(_o);
        switch (_id) {
        case 0: _t->sig_updateui((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 1: _t->sig_copydone(); break;
        case 2: _t->slot_cancelprocess(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ExtCopyProcess::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ExtCopyProcess::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_ExtCopyProcess,
      qt_meta_data_ExtCopyProcess, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ExtCopyProcess::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ExtCopyProcess::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ExtCopyProcess::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ExtCopyProcess))
        return static_cast<void*>(const_cast< ExtCopyProcess*>(this));
    return QThread::qt_metacast(_clname);
}

int ExtCopyProcess::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void ExtCopyProcess::sig_updateui(QString & _t1, QString & _t2, QString & _t3, int _t4, int _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ExtCopyProcess::sig_copydone()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
static const uint qt_meta_data_ExtCopy[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      39,   32,    8,    8, 0x08,
     104,   78,    8,    8, 0x0a,
     154,    8,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ExtCopy[] = {
    "ExtCopy\0\0signal_cancelprocess()\0button\0"
    "on_buttonBox_clicked(QAbstractButton*)\0"
    "file,from,to,copied,total\0"
    "slot_updateui(QString&,QString&,QString&,int,int)\0"
    "slot_copydone()\0"
};

void ExtCopy::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ExtCopy *_t = static_cast<ExtCopy *>(_o);
        switch (_id) {
        case 0: _t->signal_cancelprocess(); break;
        case 1: _t->on_buttonBox_clicked((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 2: _t->slot_updateui((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 3: _t->slot_copydone(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ExtCopy::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ExtCopy::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ExtCopy,
      qt_meta_data_ExtCopy, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ExtCopy::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ExtCopy::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ExtCopy::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ExtCopy))
        return static_cast<void*>(const_cast< ExtCopy*>(this));
    return QDialog::qt_metacast(_clname);
}

int ExtCopy::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void ExtCopy::signal_cancelprocess()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
