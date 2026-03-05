/****************************************************************************
** Meta object code from reading C++ file 'input_panel.hpp'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/ui/input_panel.hpp"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'input_panel.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_input_panel_t {
    uint offsetsAndSizes[20];
    char stringdata0[12];
    char stringdata1[16];
    char stringdata2[1];
    char stringdata3[18];
    char stringdata4[7];
    char stringdata5[15];
    char stringdata6[15];
    char stringdata7[16];
    char stringdata8[21];
    char stringdata9[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_input_panel_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_input_panel_t qt_meta_stringdata_input_panel = {
    {
        QT_MOC_LITERAL(0, 11),  // "input_panel"
        QT_MOC_LITERAL(12, 15),  // "start_requested"
        QT_MOC_LITERAL(28, 0),  // ""
        QT_MOC_LITERAL(29, 17),  // "simulation_config"
        QT_MOC_LITERAL(47, 6),  // "config"
        QT_MOC_LITERAL(54, 14),  // "step_requested"
        QT_MOC_LITERAL(69, 14),  // "stop_requested"
        QT_MOC_LITERAL(84, 15),  // "reset_requested"
        QT_MOC_LITERAL(100, 20),  // "on_dist_type_changed"
        QT_MOC_LITERAL(121, 5)   // "index"
    },
    "input_panel",
    "start_requested",
    "",
    "simulation_config",
    "config",
    "step_requested",
    "stop_requested",
    "reset_requested",
    "on_dist_type_changed",
    "index"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_input_panel[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   44,    2, 0x06,    1 /* Public */,
       5,    1,   47,    2, 0x06,    3 /* Public */,
       6,    0,   50,    2, 0x06,    5 /* Public */,
       7,    0,   51,    2, 0x06,    6 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       8,    1,   52,    2, 0x08,    7 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    9,

       0        // eod
};

Q_CONSTINIT const QMetaObject input_panel::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_input_panel.offsetsAndSizes,
    qt_meta_data_input_panel,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_input_panel_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<input_panel, std::true_type>,
        // method 'start_requested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const simulation_config &, std::false_type>,
        // method 'step_requested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const simulation_config &, std::false_type>,
        // method 'stop_requested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'reset_requested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_dist_type_changed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>
    >,
    nullptr
} };

void input_panel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<input_panel *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->start_requested((*reinterpret_cast< std::add_pointer_t<simulation_config>>(_a[1]))); break;
        case 1: _t->step_requested((*reinterpret_cast< std::add_pointer_t<simulation_config>>(_a[1]))); break;
        case 2: _t->stop_requested(); break;
        case 3: _t->reset_requested(); break;
        case 4: _t->on_dist_type_changed((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (input_panel::*)(const simulation_config & );
            if (_t _q_method = &input_panel::start_requested; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (input_panel::*)(const simulation_config & );
            if (_t _q_method = &input_panel::step_requested; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (input_panel::*)();
            if (_t _q_method = &input_panel::stop_requested; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (input_panel::*)();
            if (_t _q_method = &input_panel::reset_requested; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject *input_panel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *input_panel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_input_panel.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int input_panel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void input_panel::start_requested(const simulation_config & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void input_panel::step_requested(const simulation_config & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void input_panel::stop_requested()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void input_panel::reset_requested()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
