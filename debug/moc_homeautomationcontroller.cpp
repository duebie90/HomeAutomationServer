/****************************************************************************
** Meta object code from reading C++ file 'homeautomationcontroller.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../homeautomationcontroller.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'homeautomationcontroller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_HomeAutomationController_t {
    QByteArrayData data[15];
    char stringdata0[202];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HomeAutomationController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HomeAutomationController_t qt_meta_stringdata_HomeAutomationController = {
    {
QT_MOC_LITERAL(0, 0, 24), // "HomeAutomationController"
QT_MOC_LITERAL(1, 25, 19), // "slotClientConnected"
QT_MOC_LITERAL(2, 45, 0), // ""
QT_MOC_LITERAL(3, 46, 11), // "QTcpSocket*"
QT_MOC_LITERAL(4, 58, 12), // "clientSocket"
QT_MOC_LITERAL(5, 71, 13), // "slotUpdateUis"
QT_MOC_LITERAL(6, 85, 23), // "slotProcessMessageNewUi"
QT_MOC_LITERAL(7, 109, 6), // "socket"
QT_MOC_LITERAL(8, 116, 5), // "alias"
QT_MOC_LITERAL(9, 122, 4), // "pass"
QT_MOC_LITERAL(10, 127, 3), // "MAC"
QT_MOC_LITERAL(11, 131, 29), // "slotProcessMessageNewEndpoint"
QT_MOC_LITERAL(12, 161, 4), // "type"
QT_MOC_LITERAL(13, 166, 29), // "slotForwardStateChangeRequest"
QT_MOC_LITERAL(14, 196, 5) // "state"

    },
    "HomeAutomationController\0slotClientConnected\0"
    "\0QTcpSocket*\0clientSocket\0slotUpdateUis\0"
    "slotProcessMessageNewUi\0socket\0alias\0"
    "pass\0MAC\0slotProcessMessageNewEndpoint\0"
    "type\0slotForwardStateChangeRequest\0"
    "state"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HomeAutomationController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x08 /* Private */,
       5,    0,   42,    2, 0x08 /* Private */,
       6,    4,   43,    2, 0x08 /* Private */,
      11,    4,   52,    2, 0x08 /* Private */,
      13,    2,   61,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString, QMetaType::QString, QMetaType::QString,    7,    8,    9,   10,
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString, QMetaType::QString, QMetaType::QString,    7,    8,   12,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,   10,   14,

       0        // eod
};

void HomeAutomationController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        HomeAutomationController *_t = static_cast<HomeAutomationController *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotClientConnected((*reinterpret_cast< QTcpSocket*(*)>(_a[1]))); break;
        case 1: _t->slotUpdateUis(); break;
        case 2: _t->slotProcessMessageNewUi((*reinterpret_cast< QTcpSocket*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 3: _t->slotProcessMessageNewEndpoint((*reinterpret_cast< QTcpSocket*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 4: _t->slotForwardStateChangeRequest((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTcpSocket* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTcpSocket* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTcpSocket* >(); break;
            }
            break;
        }
    }
}

const QMetaObject HomeAutomationController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_HomeAutomationController.data,
      qt_meta_data_HomeAutomationController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *HomeAutomationController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HomeAutomationController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_HomeAutomationController.stringdata0))
        return static_cast<void*>(const_cast< HomeAutomationController*>(this));
    return QObject::qt_metacast(_clname);
}

int HomeAutomationController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
