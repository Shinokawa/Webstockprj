/****************************************************************************
** Meta object code from reading C++ file 'userInfo.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../userUI/userInfo.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'userInfo.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.3. It"
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
struct qt_meta_stringdata_CLASSuserInfoENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSuserInfoENDCLASS = QtMocHelpers::stringData(
    "userInfo",
    "doListWidget",
    "",
    "const QListWidgetItem*",
    "item",
    "ci_doOKButton",
    "cp_doOKButton",
    "flashInfoUI"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSuserInfoENDCLASS_t {
    uint offsetsAndSizes[16];
    char stringdata0[9];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[23];
    char stringdata4[5];
    char stringdata5[14];
    char stringdata6[14];
    char stringdata7[12];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSuserInfoENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSuserInfoENDCLASS_t qt_meta_stringdata_CLASSuserInfoENDCLASS = {
    {
        QT_MOC_LITERAL(0, 8),  // "userInfo"
        QT_MOC_LITERAL(9, 12),  // "doListWidget"
        QT_MOC_LITERAL(22, 0),  // ""
        QT_MOC_LITERAL(23, 22),  // "const QListWidgetItem*"
        QT_MOC_LITERAL(46, 4),  // "item"
        QT_MOC_LITERAL(51, 13),  // "ci_doOKButton"
        QT_MOC_LITERAL(65, 13),  // "cp_doOKButton"
        QT_MOC_LITERAL(79, 11)   // "flashInfoUI"
    },
    "userInfo",
    "doListWidget",
    "",
    "const QListWidgetItem*",
    "item",
    "ci_doOKButton",
    "cp_doOKButton",
    "flashInfoUI"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSuserInfoENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   38,    2, 0x108,    1 /* Private | MethodIsConst  */,
       5,    0,   41,    2, 0x08,    3 /* Private */,
       6,    0,   42,    2, 0x08,    4 /* Private */,
       7,    0,   43,    2, 0x08,    5 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject userInfo::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSuserInfoENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSuserInfoENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSuserInfoENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<userInfo, std::true_type>,
        // method 'doListWidget'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QListWidgetItem *, std::false_type>,
        // method 'ci_doOKButton'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'cp_doOKButton'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'flashInfoUI'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void userInfo::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<userInfo *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->doListWidget((*reinterpret_cast< std::add_pointer_t<const QListWidgetItem*>>(_a[1]))); break;
        case 1: _t->ci_doOKButton(); break;
        case 2: _t->cp_doOKButton(); break;
        case 3: _t->flashInfoUI(); break;
        default: ;
        }
    }
}

const QMetaObject *userInfo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *userInfo::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSuserInfoENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int userInfo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
