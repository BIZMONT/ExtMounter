/****************************************************************************
** Meta object code from reading C++ file 'ExtExplorer.h'
**
** Created: Sun 31. May 22:44:37 2015
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../ExtExplorer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ExtExplorer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Ext2Explore[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,
      40,   12,   12,   12, 0x08,
      76,   12,   12,   12, 0x08,
     109,  103,   12,   12, 0x08,
     147,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Ext2Explore[] = {
    "Ext2Explore\0\0on_action_Save_triggered()\0"
    "on_action_Rescan_System_triggered()\0"
    "on_action_Exit_triggered()\0index\0"
    "on_action_item_dbclicked(QModelIndex)\0"
    "Context_menu(QPoint)\0"
};

void Ext2Explore::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Ext2Explore *_t = static_cast<Ext2Explore *>(_o);
        switch (_id) {
        case 0: _t->on_action_Save_triggered(); break;
        case 1: _t->on_action_Rescan_System_triggered(); break;
        case 2: _t->on_action_Exit_triggered(); break;
        case 3: _t->on_action_item_dbclicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 4: _t->Context_menu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Ext2Explore::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Ext2Explore::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Ext2Explore,
      qt_meta_data_Ext2Explore, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Ext2Explore::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Ext2Explore::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Ext2Explore::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Ext2Explore))
        return static_cast<void*>(const_cast< Ext2Explore*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int Ext2Explore::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
