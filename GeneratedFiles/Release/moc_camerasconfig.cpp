/****************************************************************************
** Meta object code from reading C++ file 'camerasconfig.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../camerasconfig.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'camerasconfig.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CamerasConfig_t {
    QByteArrayData data[12];
    char stringdata0[264];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CamerasConfig_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CamerasConfig_t qt_meta_stringdata_CamerasConfig = {
    {
QT_MOC_LITERAL(0, 0, 13), // "CamerasConfig"
QT_MOC_LITERAL(1, 14, 16), // "okButtonAccepted"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 16), // "okButtonRejected"
QT_MOC_LITERAL(4, 49, 22), // "buttonAddCameraPressed"
QT_MOC_LITERAL(5, 72, 30), // "setCameraParametersToSpinBoxes"
QT_MOC_LITERAL(6, 103, 26), // "saveCameraParametersFromSB"
QT_MOC_LITERAL(7, 130, 13), // "setCameraType"
QT_MOC_LITERAL(8, 144, 24), // "hidePerspectiveRadLayout"
QT_MOC_LITERAL(9, 169, 25), // "hidePerspectiveQuatLayout"
QT_MOC_LITERAL(10, 195, 34), // "hidePerspectiveDisortionQuatL..."
QT_MOC_LITERAL(11, 230, 33) // "hidePerspectiveDisortionRadLa..."

    },
    "CamerasConfig\0okButtonAccepted\0\0"
    "okButtonRejected\0buttonAddCameraPressed\0"
    "setCameraParametersToSpinBoxes\0"
    "saveCameraParametersFromSB\0setCameraType\0"
    "hidePerspectiveRadLayout\0"
    "hidePerspectiveQuatLayout\0"
    "hidePerspectiveDisortionQuatLayout\0"
    "hidePerspectiveDisortionRadLayout"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CamerasConfig[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x08 /* Private */,
       3,    0,   65,    2, 0x08 /* Private */,
       4,    0,   66,    2, 0x08 /* Private */,
       5,    0,   67,    2, 0x08 /* Private */,
       6,    0,   68,    2, 0x08 /* Private */,
       7,    0,   69,    2, 0x08 /* Private */,
       8,    0,   70,    2, 0x08 /* Private */,
       9,    0,   71,    2, 0x08 /* Private */,
      10,    0,   72,    2, 0x08 /* Private */,
      11,    0,   73,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CamerasConfig::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CamerasConfig *_t = static_cast<CamerasConfig *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->okButtonAccepted(); break;
        case 1: _t->okButtonRejected(); break;
        case 2: _t->buttonAddCameraPressed(); break;
        case 3: _t->setCameraParametersToSpinBoxes(); break;
        case 4: _t->saveCameraParametersFromSB(); break;
        case 5: _t->setCameraType(); break;
        case 6: _t->hidePerspectiveRadLayout(); break;
        case 7: _t->hidePerspectiveQuatLayout(); break;
        case 8: _t->hidePerspectiveDisortionQuatLayout(); break;
        case 9: _t->hidePerspectiveDisortionRadLayout(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject CamerasConfig::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CamerasConfig.data,
      qt_meta_data_CamerasConfig,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CamerasConfig::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CamerasConfig::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CamerasConfig.stringdata0))
        return static_cast<void*>(const_cast< CamerasConfig*>(this));
    return QDialog::qt_metacast(_clname);
}

int CamerasConfig::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
