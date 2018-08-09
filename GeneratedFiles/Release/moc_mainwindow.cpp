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
    QByteArrayData data[66];
    char stringdata0[1293];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 13), // "saveDatToFile"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 24), // "saveDatFromQStringToFile"
QT_MOC_LITERAL(4, 51, 8), // "fileName"
QT_MOC_LITERAL(5, 60, 15), // "loadDatFromFile"
QT_MOC_LITERAL(6, 76, 27), // "saveAllFramesSequenceToFile"
QT_MOC_LITERAL(7, 104, 13), // "saveAMCToFile"
QT_MOC_LITERAL(8, 118, 24), // "saveAMCFromQStringToFile"
QT_MOC_LITERAL(9, 143, 11), // "loadFromAmc"
QT_MOC_LITERAL(10, 155, 13), // "saveAsfToFile"
QT_MOC_LITERAL(11, 169, 24), // "saveAsfFromQStringToFile"
QT_MOC_LITERAL(12, 194, 11), // "loadFromASF"
QT_MOC_LITERAL(13, 206, 18), // "manageBonesPressed"
QT_MOC_LITERAL(14, 225, 23), // "openCameraConfigPressed"
QT_MOC_LITERAL(15, 249, 18), // "saveCamerasPressed"
QT_MOC_LITERAL(16, 268, 18), // "loadCamerasPressed"
QT_MOC_LITERAL(17, 287, 18), // "saveProjectPressed"
QT_MOC_LITERAL(18, 306, 18), // "loadProjectPressed"
QT_MOC_LITERAL(19, 325, 13), // "configPressed"
QT_MOC_LITERAL(20, 339, 26), // "saveModelStateToBoolVector"
QT_MOC_LITERAL(21, 366, 6), // "rotate"
QT_MOC_LITERAL(22, 373, 3), // "str"
QT_MOC_LITERAL(23, 377, 9), // "setRotate"
QT_MOC_LITERAL(24, 387, 16), // "updateBoneLength"
QT_MOC_LITERAL(25, 404, 25), // "setBoneLengthFromLineEdit"
QT_MOC_LITERAL(26, 430, 24), // "updateBoneLengthGeometry"
QT_MOC_LITERAL(27, 455, 33), // "setBoneLengthGeometryFromLine..."
QT_MOC_LITERAL(28, 489, 19), // "updateBoneTopRadius"
QT_MOC_LITERAL(29, 509, 28), // "setBoneTopRadiusFromLineEdit"
QT_MOC_LITERAL(30, 538, 22), // "updateBoneBottomRadius"
QT_MOC_LITERAL(31, 561, 31), // "setBoneBottomRadiusFromLineEdit"
QT_MOC_LITERAL(32, 593, 9), // "translate"
QT_MOC_LITERAL(33, 603, 12), // "setTranslate"
QT_MOC_LITERAL(34, 616, 12), // "selectCamera"
QT_MOC_LITERAL(35, 629, 13), // "updateSliders"
QT_MOC_LITERAL(36, 643, 23), // "increaseIteratorPressed"
QT_MOC_LITERAL(37, 667, 1), // "i"
QT_MOC_LITERAL(38, 669, 21), // "reduceIteratorPressed"
QT_MOC_LITERAL(39, 691, 18), // "maxIteratorPressed"
QT_MOC_LITERAL(40, 710, 18), // "minIteratorPressed"
QT_MOC_LITERAL(41, 729, 16), // "playPausePressed"
QT_MOC_LITERAL(42, 746, 24), // "showModelInDialogPressed"
QT_MOC_LITERAL(43, 771, 29), // "selectImagesBackgroundPressed"
QT_MOC_LITERAL(44, 801, 26), // "selectAviBackgroundPressed"
QT_MOC_LITERAL(45, 828, 25), // "resizeDownGLWidgetPressed"
QT_MOC_LITERAL(46, 854, 23), // "resizeUpGLWidgetPressed"
QT_MOC_LITERAL(47, 878, 18), // "deleteFramePressed"
QT_MOC_LITERAL(48, 897, 21), // "buttonAddFramePressed"
QT_MOC_LITERAL(49, 919, 19), // "updateFrameIterator"
QT_MOC_LITERAL(50, 939, 18), // "reloadGLWidgetMenu"
QT_MOC_LITERAL(51, 958, 18), // "reloadGLRotateMenu"
QT_MOC_LITERAL(52, 977, 21), // "refreshRotateSideMenu"
QT_MOC_LITERAL(53, 999, 21), // "refreshLengthSideMenu"
QT_MOC_LITERAL(54, 1021, 23), // "refreshGeometrySideMenu"
QT_MOC_LITERAL(55, 1045, 22), // "updateBoneConfigFromCB"
QT_MOC_LITERAL(56, 1068, 28), // "updateBoneConfigFromLimitsSB"
QT_MOC_LITERAL(57, 1097, 30), // "updateBoneConfigFromVelocitySB"
QT_MOC_LITERAL(58, 1128, 11), // "tabSelected"
QT_MOC_LITERAL(59, 1140, 22), // "disableLimitsSpinBoxes"
QT_MOC_LITERAL(60, 1163, 14), // "playBackground"
QT_MOC_LITERAL(61, 1178, 14), // "setGoldenRatio"
QT_MOC_LITERAL(62, 1193, 25), // "setGoldenRatioFromSpinBox"
QT_MOC_LITERAL(63, 1219, 19), // "setIteratorToFrames"
QT_MOC_LITERAL(64, 1239, 25), // "reduceGlobalSliderPressed"
QT_MOC_LITERAL(65, 1265, 27) // "increaseGlobalSliderPressed"

    },
    "MainWindow\0saveDatToFile\0\0"
    "saveDatFromQStringToFile\0fileName\0"
    "loadDatFromFile\0saveAllFramesSequenceToFile\0"
    "saveAMCToFile\0saveAMCFromQStringToFile\0"
    "loadFromAmc\0saveAsfToFile\0"
    "saveAsfFromQStringToFile\0loadFromASF\0"
    "manageBonesPressed\0openCameraConfigPressed\0"
    "saveCamerasPressed\0loadCamerasPressed\0"
    "saveProjectPressed\0loadProjectPressed\0"
    "configPressed\0saveModelStateToBoolVector\0"
    "rotate\0str\0setRotate\0updateBoneLength\0"
    "setBoneLengthFromLineEdit\0"
    "updateBoneLengthGeometry\0"
    "setBoneLengthGeometryFromLineEdit\0"
    "updateBoneTopRadius\0setBoneTopRadiusFromLineEdit\0"
    "updateBoneBottomRadius\0"
    "setBoneBottomRadiusFromLineEdit\0"
    "translate\0setTranslate\0selectCamera\0"
    "updateSliders\0increaseIteratorPressed\0"
    "i\0reduceIteratorPressed\0maxIteratorPressed\0"
    "minIteratorPressed\0playPausePressed\0"
    "showModelInDialogPressed\0"
    "selectImagesBackgroundPressed\0"
    "selectAviBackgroundPressed\0"
    "resizeDownGLWidgetPressed\0"
    "resizeUpGLWidgetPressed\0deleteFramePressed\0"
    "buttonAddFramePressed\0updateFrameIterator\0"
    "reloadGLWidgetMenu\0reloadGLRotateMenu\0"
    "refreshRotateSideMenu\0refreshLengthSideMenu\0"
    "refreshGeometrySideMenu\0updateBoneConfigFromCB\0"
    "updateBoneConfigFromLimitsSB\0"
    "updateBoneConfigFromVelocitySB\0"
    "tabSelected\0disableLimitsSpinBoxes\0"
    "playBackground\0setGoldenRatio\0"
    "setGoldenRatioFromSpinBox\0setIteratorToFrames\0"
    "reduceGlobalSliderPressed\0"
    "increaseGlobalSliderPressed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      61,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  319,    2, 0x08 /* Private */,
       3,    1,  320,    2, 0x08 /* Private */,
       5,    0,  323,    2, 0x08 /* Private */,
       6,    0,  324,    2, 0x08 /* Private */,
       7,    0,  325,    2, 0x08 /* Private */,
       8,    1,  326,    2, 0x08 /* Private */,
       9,    0,  329,    2, 0x08 /* Private */,
      10,    0,  330,    2, 0x08 /* Private */,
      11,    1,  331,    2, 0x08 /* Private */,
      12,    0,  334,    2, 0x08 /* Private */,
      13,    0,  335,    2, 0x08 /* Private */,
      14,    0,  336,    2, 0x08 /* Private */,
      15,    0,  337,    2, 0x08 /* Private */,
      16,    0,  338,    2, 0x08 /* Private */,
      17,    0,  339,    2, 0x08 /* Private */,
      18,    0,  340,    2, 0x08 /* Private */,
      19,    0,  341,    2, 0x08 /* Private */,
      20,    0,  342,    2, 0x08 /* Private */,
      21,    1,  343,    2, 0x08 /* Private */,
      23,    1,  346,    2, 0x08 /* Private */,
      24,    1,  349,    2, 0x08 /* Private */,
      25,    1,  352,    2, 0x08 /* Private */,
      26,    1,  355,    2, 0x08 /* Private */,
      27,    1,  358,    2, 0x08 /* Private */,
      28,    1,  361,    2, 0x08 /* Private */,
      29,    1,  364,    2, 0x08 /* Private */,
      30,    1,  367,    2, 0x08 /* Private */,
      31,    1,  370,    2, 0x08 /* Private */,
      32,    1,  373,    2, 0x08 /* Private */,
      33,    1,  376,    2, 0x08 /* Private */,
      34,    1,  379,    2, 0x08 /* Private */,
      35,    0,  382,    2, 0x08 /* Private */,
      36,    1,  383,    2, 0x08 /* Private */,
      38,    1,  386,    2, 0x08 /* Private */,
      39,    1,  389,    2, 0x08 /* Private */,
      40,    1,  392,    2, 0x08 /* Private */,
      41,    1,  395,    2, 0x08 /* Private */,
      42,    1,  398,    2, 0x08 /* Private */,
      43,    1,  401,    2, 0x08 /* Private */,
      44,    1,  404,    2, 0x08 /* Private */,
      45,    1,  407,    2, 0x08 /* Private */,
      46,    1,  410,    2, 0x08 /* Private */,
      47,    1,  413,    2, 0x08 /* Private */,
      48,    0,  416,    2, 0x08 /* Private */,
      49,    1,  417,    2, 0x08 /* Private */,
      50,    0,  420,    2, 0x08 /* Private */,
      51,    0,  421,    2, 0x08 /* Private */,
      52,    0,  422,    2, 0x08 /* Private */,
      53,    0,  423,    2, 0x08 /* Private */,
      54,    0,  424,    2, 0x08 /* Private */,
      55,    0,  425,    2, 0x08 /* Private */,
      56,    0,  426,    2, 0x08 /* Private */,
      57,    0,  427,    2, 0x08 /* Private */,
      58,    0,  428,    2, 0x08 /* Private */,
      59,    0,  429,    2, 0x08 /* Private */,
      60,    1,  430,    2, 0x08 /* Private */,
      61,    0,  433,    2, 0x08 /* Private */,
      62,    0,  434,    2, 0x08 /* Private */,
      63,    0,  435,    2, 0x08 /* Private */,
      64,    0,  436,    2, 0x08 /* Private */,
      65,    0,  437,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   37,
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
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->saveDatToFile(); break;
        case 1: _t->saveDatFromQStringToFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->loadDatFromFile(); break;
        case 3: _t->saveAllFramesSequenceToFile(); break;
        case 4: _t->saveAMCToFile(); break;
        case 5: _t->saveAMCFromQStringToFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->loadFromAmc(); break;
        case 7: _t->saveAsfToFile(); break;
        case 8: _t->saveAsfFromQStringToFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->loadFromASF(); break;
        case 10: _t->manageBonesPressed(); break;
        case 11: _t->openCameraConfigPressed(); break;
        case 12: _t->saveCamerasPressed(); break;
        case 13: _t->loadCamerasPressed(); break;
        case 14: _t->saveProjectPressed(); break;
        case 15: _t->loadProjectPressed(); break;
        case 16: _t->configPressed(); break;
        case 17: _t->saveModelStateToBoolVector(); break;
        case 18: _t->rotate((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 19: _t->setRotate((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 20: _t->updateBoneLength((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 21: _t->setBoneLengthFromLineEdit((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 22: _t->updateBoneLengthGeometry((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 23: _t->setBoneLengthGeometryFromLineEdit((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 24: _t->updateBoneTopRadius((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 25: _t->setBoneTopRadiusFromLineEdit((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 26: _t->updateBoneBottomRadius((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 27: _t->setBoneBottomRadiusFromLineEdit((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 28: _t->translate((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 29: _t->setTranslate((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 30: _t->selectCamera((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 31: _t->updateSliders(); break;
        case 32: _t->increaseIteratorPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 33: _t->reduceIteratorPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 34: _t->maxIteratorPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 35: _t->minIteratorPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 36: _t->playPausePressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 37: _t->showModelInDialogPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 38: _t->selectImagesBackgroundPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 39: _t->selectAviBackgroundPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 40: _t->resizeDownGLWidgetPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 41: _t->resizeUpGLWidgetPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 42: _t->deleteFramePressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 43: _t->buttonAddFramePressed(); break;
        case 44: _t->updateFrameIterator((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 45: _t->reloadGLWidgetMenu(); break;
        case 46: _t->reloadGLRotateMenu(); break;
        case 47: _t->refreshRotateSideMenu(); break;
        case 48: _t->refreshLengthSideMenu(); break;
        case 49: _t->refreshGeometrySideMenu(); break;
        case 50: _t->updateBoneConfigFromCB(); break;
        case 51: _t->updateBoneConfigFromLimitsSB(); break;
        case 52: _t->updateBoneConfigFromVelocitySB(); break;
        case 53: _t->tabSelected(); break;
        case 54: _t->disableLimitsSpinBoxes(); break;
        case 55: _t->playBackground((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 56: _t->setGoldenRatio(); break;
        case 57: _t->setGoldenRatioFromSpinBox(); break;
        case 58: _t->setIteratorToFrames(); break;
        case 59: _t->reduceGlobalSliderPressed(); break;
        case 60: _t->increaseGlobalSliderPressed(); break;
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
        if (_id < 61)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 61;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 61)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 61;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
