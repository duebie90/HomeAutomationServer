/****************************************************************************
** Meta object code from reading C++ file 'datareceiver.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../datareceiver.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'datareceiver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DataReceiver_t {
    QByteArrayData data[15];
    char stringdata0[232];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DataReceiver_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DataReceiver_t qt_meta_stringdata_DataReceiver = {
    {
QT_MOC_LITERAL(0, 0, 12), // "DataReceiver"
QT_MOC_LITERAL(1, 13, 27), // "signalReceivedEndpointIdent"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 11), // "QTcpSocket*"
QT_MOC_LITERAL(4, 54, 6), // "socket"
QT_MOC_LITERAL(5, 61, 5), // "alias"
QT_MOC_LITERAL(6, 67, 4), // "type"
QT_MOC_LITERAL(7, 72, 3), // "MAC"
QT_MOC_LITERAL(8, 76, 27), // "signalReceivedEndpointState"
QT_MOC_LITERAL(9, 104, 5), // "state"
QT_MOC_LITERAL(10, 110, 36), // "signalReceivedUiEndpointState..."
QT_MOC_LITERAL(11, 147, 40), // "signalReceivedServerEndpointS..."
QT_MOC_LITERAL(12, 188, 21), // "signalReceivedUiIdent"
QT_MOC_LITERAL(13, 210, 4), // "pass"
QT_MOC_LITERAL(14, 215, 16) // "slotReceivedData"

    },
    "DataReceiver\0signalReceivedEndpointIdent\0"
    "\0QTcpSocket*\0socket\0alias\0type\0MAC\0"
    "signalReceivedEndpointState\0state\0"
    "signalReceivedUiEndpointStateRequest\0"
    "signalReceivedServerEndpointStateRequest\0"
    "signalReceivedUiIdent\0pass\0slotReceivedData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DataReceiver[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   44,    2, 0x06 /* Public */,
       8,    2,   53,    2, 0x06 /* Public */,
      10,    2,   58,    2, 0x06 /* Public */,
      11,    2,   63,    2, 0x06 /* Public */,
      12,    4,   68,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      14,    0,   77,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString, QMetaType::QString, QMetaType::QString,    4,    5,    6,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,    7,    9,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,    7,    9,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,    7,    9,
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString, QMetaType::QString, QMetaType::QString,    4,    5,   13,    7,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void DataReceiver::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DataReceiver *_t = static_cast<DataReceiver *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalReceivedEndpointIdent((*reinterpret_cast< QTcpSocket*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 1: _t->signalReceivedEndpointState((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: _t->signalReceivedUiEndpointStateRequest((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 3: _t->signalReceivedServerEndpointStateRequest((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 4: _t->signalReceivedUiIdent((*reinterpret_cast< QTcpSocket*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 5: _t->slotReceivedData(); break;
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
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTcpSocket* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (DataReceiver::*_t)(QTcpSocket * , QString , QString , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DataReceiver::signalReceivedEndpointIdent)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (DataReceiver::*_t)(QString , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DataReceiver::signalReceivedEndpointState)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (DataReceiver::*_t)(QString , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DataReceiver::signalReceivedUiEndpointStateRequest)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (DataReceiver::*_t)(QString , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DataReceiver::signalReceivedServerEndpointStateRequest)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (DataReceiver::*_t)(QTcpSocket * , QString , QString , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DataReceiver::signalReceivedUiIdent)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject DataReceiver::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DataReceiver.data,
      qt_meta_data_DataReceiver,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *DataReceiver::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DataReceiver::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_DataReceiver.stringdata0))
        return static_cast<void*>(const_cast< DataReceiver*>(this));
    return QObject::qt_metacast(_clname);
}

int DataReceiver::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void DataReceiver::signalReceivedEndpointIdent(QTcpSocket * _t1, QString _t2, QString _t3, QString _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DataReceiver::signalReceivedEndpointState(QString _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DataReceiver::signalReceivedUiEndpointStateRequest(QString _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DataReceiver::signalReceivedServerEndpointStateRequest(QString _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DataReceiver::signalReceivedUiIdent(QTcpSocket * _t1, QString _t2, QString _t3, QString _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
