/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSClickableLabelENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSClickableLabelENDCLASS = QtMocHelpers::stringData(
    "ClickableLabel",
    "clicked",
    "",
    "row"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSClickableLabelENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   20,    2, 0x06,    1 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,

       0        // eod
};

Q_CONSTINIT const QMetaObject ClickableLabel::staticMetaObject = { {
    QMetaObject::SuperData::link<QLabel::staticMetaObject>(),
    qt_meta_stringdata_CLASSClickableLabelENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSClickableLabelENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSClickableLabelENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ClickableLabel, std::true_type>,
        // method 'clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>
    >,
    nullptr
} };

void ClickableLabel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ClickableLabel *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->clicked((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ClickableLabel::*)(int );
            if (_t _q_method = &ClickableLabel::clicked; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *ClickableLabel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ClickableLabel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSClickableLabelENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QLabel::qt_metacast(_clname);
}

int ClickableLabel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void ClickableLabel::clicked(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSMainWindowENDCLASS = QtMocHelpers::stringData(
    "MainWindow",
    "on_btnLogin_clicked",
    "",
    "on_btnCreateUser_clicked",
    "on_btnExecuteQuery_clicked",
    "on_opTable_cellChanged",
    "row",
    "column",
    "on_tblList_cellChanged",
    "on_tblList_cellClicked",
    "on_btnAddtbl_pressed",
    "on_btnGenQuery_pressed",
    "on_btnDelRow_pressed",
    "on_edtQuery_textChanged",
    "on_actionUndo_triggered",
    "on_opTable_cellClicked",
    "on_opTable_currentCellChanged",
    "currentRow",
    "currentColumn",
    "previousRow",
    "previousColumn",
    "handleEditIconClick",
    "QModelIndex",
    "index",
    "handleDeleteIconClick",
    "on_dbTable_cellDoubleClicked",
    "on_btnBackDb_clicked",
    "on_btnCreateDB_pressed",
    "on_btnBackDb2_pressed",
    "on_cbCollation_currentTextChanged",
    "arg1",
    "on_btnExportDb_pressed",
    "on_btnImportDb_pressed",
    "on_btnTables_pressed",
    "on_tblDbUsers_cellClicked",
    "on_btnBackDb2_2_pressed",
    "on_btnUsers_pressed",
    "on_btnRevokeAll_pressed",
    "on_btnSetPrivileges_pressed",
    "on_chFullAccess_checkStateChanged",
    "Qt::CheckState",
    "on_btnAccess_pressed",
    "on_tabWidget_currentChanged",
    "on_tblUsersList_cellChanged",
    "on_btnCreateNewUser_pressed"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMainWindowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      33,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  212,    2, 0x08,    1 /* Private */,
       3,    0,  213,    2, 0x08,    2 /* Private */,
       4,    0,  214,    2, 0x08,    3 /* Private */,
       5,    2,  215,    2, 0x08,    4 /* Private */,
       8,    2,  220,    2, 0x08,    7 /* Private */,
       9,    2,  225,    2, 0x08,   10 /* Private */,
      10,    0,  230,    2, 0x08,   13 /* Private */,
      11,    0,  231,    2, 0x08,   14 /* Private */,
      12,    0,  232,    2, 0x08,   15 /* Private */,
      13,    0,  233,    2, 0x08,   16 /* Private */,
      14,    0,  234,    2, 0x08,   17 /* Private */,
      15,    2,  235,    2, 0x08,   18 /* Private */,
      16,    4,  240,    2, 0x08,   21 /* Private */,
      21,    1,  249,    2, 0x08,   26 /* Private */,
      24,    1,  252,    2, 0x08,   28 /* Private */,
      25,    2,  255,    2, 0x08,   30 /* Private */,
      26,    0,  260,    2, 0x08,   33 /* Private */,
      27,    0,  261,    2, 0x08,   34 /* Private */,
      28,    0,  262,    2, 0x08,   35 /* Private */,
      29,    1,  263,    2, 0x08,   36 /* Private */,
      31,    0,  266,    2, 0x08,   38 /* Private */,
      32,    0,  267,    2, 0x08,   39 /* Private */,
      33,    0,  268,    2, 0x08,   40 /* Private */,
      34,    2,  269,    2, 0x08,   41 /* Private */,
      35,    0,  274,    2, 0x08,   44 /* Private */,
      36,    0,  275,    2, 0x08,   45 /* Private */,
      37,    0,  276,    2, 0x08,   46 /* Private */,
      38,    0,  277,    2, 0x08,   47 /* Private */,
      39,    1,  278,    2, 0x08,   48 /* Private */,
      41,    0,  281,    2, 0x08,   50 /* Private */,
      42,    1,  282,    2, 0x08,   51 /* Private */,
      43,    2,  285,    2, 0x08,   53 /* Private */,
      44,    0,  290,    2, 0x08,   56 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    6,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    6,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    6,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    6,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,   17,   18,   19,   20,
    QMetaType::Void, 0x80000000 | 22,   23,
    QMetaType::Void, 0x80000000 | 22,   23,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    6,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   30,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    6,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 40,   30,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   23,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    6,    7,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSMainWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMainWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMainWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'on_btnLogin_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btnCreateUser_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btnExecuteQuery_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_opTable_cellChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_tblList_cellChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_tblList_cellClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_btnAddtbl_pressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btnGenQuery_pressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btnDelRow_pressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_edtQuery_textChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionUndo_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_opTable_cellClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_opTable_currentCellChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'handleEditIconClick'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>,
        // method 'handleDeleteIconClick'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>,
        // method 'on_dbTable_cellDoubleClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_btnBackDb_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btnCreateDB_pressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btnBackDb2_pressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_cbCollation_currentTextChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'on_btnExportDb_pressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btnImportDb_pressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btnTables_pressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_tblDbUsers_cellClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_btnBackDb2_2_pressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btnUsers_pressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btnRevokeAll_pressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btnSetPrivileges_pressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_chFullAccess_checkStateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const Qt::CheckState &, std::false_type>,
        // method 'on_btnAccess_pressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_tabWidget_currentChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_tblUsersList_cellChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_btnCreateNewUser_pressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_btnLogin_clicked(); break;
        case 1: _t->on_btnCreateUser_clicked(); break;
        case 2: _t->on_btnExecuteQuery_clicked(); break;
        case 3: _t->on_opTable_cellChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 4: _t->on_tblList_cellChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 5: _t->on_tblList_cellClicked((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 6: _t->on_btnAddtbl_pressed(); break;
        case 7: _t->on_btnGenQuery_pressed(); break;
        case 8: _t->on_btnDelRow_pressed(); break;
        case 9: _t->on_edtQuery_textChanged(); break;
        case 10: _t->on_actionUndo_triggered(); break;
        case 11: _t->on_opTable_cellClicked((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 12: _t->on_opTable_currentCellChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[4]))); break;
        case 13: _t->handleEditIconClick((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 14: _t->handleDeleteIconClick((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 15: _t->on_dbTable_cellDoubleClicked((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 16: _t->on_btnBackDb_clicked(); break;
        case 17: _t->on_btnCreateDB_pressed(); break;
        case 18: _t->on_btnBackDb2_pressed(); break;
        case 19: _t->on_cbCollation_currentTextChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 20: _t->on_btnExportDb_pressed(); break;
        case 21: _t->on_btnImportDb_pressed(); break;
        case 22: _t->on_btnTables_pressed(); break;
        case 23: _t->on_tblDbUsers_cellClicked((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 24: _t->on_btnBackDb2_2_pressed(); break;
        case 25: _t->on_btnUsers_pressed(); break;
        case 26: _t->on_btnRevokeAll_pressed(); break;
        case 27: _t->on_btnSetPrivileges_pressed(); break;
        case 28: _t->on_chFullAccess_checkStateChanged((*reinterpret_cast< std::add_pointer_t<Qt::CheckState>>(_a[1]))); break;
        case 29: _t->on_btnAccess_pressed(); break;
        case 30: _t->on_tabWidget_currentChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 31: _t->on_tblUsersList_cellChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 32: _t->on_btnCreateNewUser_pressed(); break;
        default: ;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMainWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 33)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 33;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 33)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 33;
    }
    return _id;
}
QT_WARNING_POP
