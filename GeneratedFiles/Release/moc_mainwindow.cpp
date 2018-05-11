/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[23];
    char stringdata0[328];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 9), // "loadFiles"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 18), // "saveOneFrameToFile"
QT_MOC_LITERAL(4, 41, 18), // "saveSequenceToFile"
QT_MOC_LITERAL(5, 60, 11), // "loadFromAmc"
QT_MOC_LITERAL(6, 72, 6), // "rotate"
QT_MOC_LITERAL(7, 79, 3), // "str"
QT_MOC_LITERAL(8, 83, 23), // "increaseIteratorPressed"
QT_MOC_LITERAL(9, 107, 1), // "i"
QT_MOC_LITERAL(10, 109, 21), // "reduceIteratorPressed"
QT_MOC_LITERAL(11, 131, 18), // "maxIteratorPressed"
QT_MOC_LITERAL(12, 150, 18), // "minIteratorPressed"
QT_MOC_LITERAL(13, 169, 18), // "manageBonesPressed"
QT_MOC_LITERAL(14, 188, 21), // "buttonAddFramePressed"
QT_MOC_LITERAL(15, 210, 24), // "updateGlobalListIterator"
QT_MOC_LITERAL(16, 235, 5), // "value"
QT_MOC_LITERAL(17, 241, 18), // "reloadGLWidgetMenu"
QT_MOC_LITERAL(18, 260, 15), // "refreshSideMenu"
QT_MOC_LITERAL(19, 276, 22), // "disableRotatingButtons"
QT_MOC_LITERAL(20, 299, 18), // "vector<pf::range2>"
QT_MOC_LITERAL(21, 318, 6), // "limits"
QT_MOC_LITERAL(22, 325, 2) // "id"

    },
    "MainWindow\0loadFiles\0\0saveOneFrameToFile\0"
    "saveSequenceToFile\0loadFromAmc\0rotate\0"
    "str\0increaseIteratorPressed\0i\0"
    "reduceIteratorPressed\0maxIteratorPressed\0"
    "minIteratorPressed\0manageBonesPressed\0"
    "buttonAddFramePressed\0updateGlobalListIterator\0"
    "value\0reloadGLWidgetMenu\0refreshSideMenu\0"
    "disableRotatingButtons\0vector<pf::range2>\0"
    "limits\0id"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x08 /* Private */,
       3,    0,   90,    2, 0x08 /* Private */,
       4,    0,   91,    2, 0x08 /* Private */,
       5,    0,   92,    2, 0x08 /* Private */,
       6,    1,   93,    2, 0x08 /* Private */,
       8,    1,   96,    2, 0x08 /* Private */,
      10,    1,   99,    2, 0x08 /* Private */,
      11,    1,  102,    2, 0x08 /* Private */,
      12,    1,  105,    2, 0x08 /* Private */,
      13,    1,  108,    2, 0x08 /* Private */,
      14,    0,  111,    2, 0x08 /* Private */,
      15,    1,  112,    2, 0x08 /* Private */,
      17,    0,  115,    2, 0x08 /* Private */,
      18,    0,  116,    2, 0x08 /* Private */,
      19,    2,  117,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool, 0x80000000 | 20, QMetaType::Int,   21,   22,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->loadFiles(); break;
        case 1: _t->saveOneFrameToFile(); break;
        case 2: _t->saveSequenceToFile(); break;
        case 3: _t->loadFromAmc(); break;
        case 4: _t->rotate((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->increaseIteratorPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->reduceIteratorPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->maxIteratorPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->minIteratorPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->manageBonesPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->buttonAddFramePressed(); break;
        case 11: _t->updateGlobalListIterator((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->reloadGLWidgetMenu(); break;
        case 13: _t->refreshSideMenu(); break;
        case 14: { bool _r = _t->disableRotatingButtons((*reinterpret_cast< vector<pf::range2>(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
