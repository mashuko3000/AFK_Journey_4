/****************************************************************************
** Meta object code from reading C++ file 'simulator_worker.hpp'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/ui/simulator_worker.hpp"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'simulator_worker.hpp' doesn't include <QObject>."
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
struct qt_meta_stringdata_simulator_worker_t {
    uint offsetsAndSizes[52];
    char stringdata0[17];
    char stringdata1[17];
    char stringdata2[1];
    char stringdata3[8];
    char stringdata4[16];
    char stringdata5[15];
    char stringdata6[5];
    char stringdata7[14];
    char stringdata8[9];
    char stringdata9[6];
    char stringdata10[9];
    char stringdata11[7];
    char stringdata12[7];
    char stringdata13[12];
    char stringdata14[9];
    char stringdata15[15];
    char stringdata16[8];
    char stringdata17[15];
    char stringdata18[18];
    char stringdata19[7];
    char stringdata20[16];
    char stringdata21[15];
    char stringdata22[11];
    char stringdata23[5];
    char stringdata24[12];
    char stringdata25[22];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_simulator_worker_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_simulator_worker_t qt_meta_stringdata_simulator_worker = {
    {
        QT_MOC_LITERAL(0, 16),  // "simulator_worker"
        QT_MOC_LITERAL(17, 16),  // "progress_updated"
        QT_MOC_LITERAL(34, 0),  // ""
        QT_MOC_LITERAL(35, 7),  // "percent"
        QT_MOC_LITERAL(43, 15),  // "iteration_ready"
        QT_MOC_LITERAL(59, 14),  // "iteration_data"
        QT_MOC_LITERAL(74, 4),  // "data"
        QT_MOC_LITERAL(79, 13),  // "stats_updated"
        QT_MOC_LITERAL(93, 8),  // "probLeaf"
        QT_MOC_LITERAL(102, 5),  // "probL"
        QT_MOC_LITERAL(108, 8),  // "probTrap"
        QT_MOC_LITERAL(117, 6),  // "avgLen"
        QT_MOC_LITERAL(124, 6),  // "size_t"
        QT_MOC_LITERAL(131, 11),  // "currentIter"
        QT_MOC_LITERAL(143, 8),  // "finished"
        QT_MOC_LITERAL(152, 14),  // "error_occurred"
        QT_MOC_LITERAL(167, 7),  // "message"
        QT_MOC_LITERAL(175, 14),  // "run_simulation"
        QT_MOC_LITERAL(190, 17),  // "simulation_config"
        QT_MOC_LITERAL(208, 6),  // "config"
        QT_MOC_LITERAL(215, 15),  // "run_single_step"
        QT_MOC_LITERAL(231, 14),  // "run_batch_step"
        QT_MOC_LITERAL(246, 10),  // "batch_size"
        QT_MOC_LITERAL(257, 4),  // "stop"
        QT_MOC_LITERAL(262, 11),  // "reset_stats"
        QT_MOC_LITERAL(274, 21)   // "request_current_stats"
    },
    "simulator_worker",
    "progress_updated",
    "",
    "percent",
    "iteration_ready",
    "iteration_data",
    "data",
    "stats_updated",
    "probLeaf",
    "probL",
    "probTrap",
    "avgLen",
    "size_t",
    "currentIter",
    "finished",
    "error_occurred",
    "message",
    "run_simulation",
    "simulation_config",
    "config",
    "run_single_step",
    "run_batch_step",
    "batch_size",
    "stop",
    "reset_stats",
    "request_current_stats"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_simulator_worker[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   80,    2, 0x06,    1 /* Public */,
       4,    1,   83,    2, 0x06,    3 /* Public */,
       7,    5,   86,    2, 0x06,    5 /* Public */,
      14,    0,   97,    2, 0x06,   11 /* Public */,
      15,    1,   98,    2, 0x06,   12 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      17,    1,  101,    2, 0x0a,   14 /* Public */,
      20,    1,  104,    2, 0x0a,   16 /* Public */,
      21,    2,  107,    2, 0x0a,   18 /* Public */,
      23,    0,  112,    2, 0x0a,   21 /* Public */,
      24,    0,  113,    2, 0x0a,   22 /* Public */,
      25,    0,  114,    2, 0x0a,   23 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, 0x80000000 | 12,    8,    9,   10,   11,   13,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   16,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 18,   19,
    QMetaType::Void, 0x80000000 | 18,   19,
    QMetaType::Void, 0x80000000 | 18, QMetaType::Int,   19,   22,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject simulator_worker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_simulator_worker.offsetsAndSizes,
    qt_meta_data_simulator_worker,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_simulator_worker_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<simulator_worker, std::true_type>,
        // method 'progress_updated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'iteration_ready'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const iteration_data &, std::false_type>,
        // method 'stats_updated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<size_t, std::false_type>,
        // method 'finished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'error_occurred'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'run_simulation'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const simulation_config &, std::false_type>,
        // method 'run_single_step'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const simulation_config &, std::false_type>,
        // method 'run_batch_step'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const simulation_config &, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'stop'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'reset_stats'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'request_current_stats'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void simulator_worker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<simulator_worker *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->progress_updated((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->iteration_ready((*reinterpret_cast< std::add_pointer_t<iteration_data>>(_a[1]))); break;
        case 2: _t->stats_updated((*reinterpret_cast< std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<size_t>>(_a[5]))); break;
        case 3: _t->finished(); break;
        case 4: _t->error_occurred((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->run_simulation((*reinterpret_cast< std::add_pointer_t<simulation_config>>(_a[1]))); break;
        case 6: _t->run_single_step((*reinterpret_cast< std::add_pointer_t<simulation_config>>(_a[1]))); break;
        case 7: _t->run_batch_step((*reinterpret_cast< std::add_pointer_t<simulation_config>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 8: _t->stop(); break;
        case 9: _t->reset_stats(); break;
        case 10: _t->request_current_stats(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (simulator_worker::*)(int );
            if (_t _q_method = &simulator_worker::progress_updated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (simulator_worker::*)(const iteration_data & );
            if (_t _q_method = &simulator_worker::iteration_ready; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (simulator_worker::*)(double , double , double , double , size_t );
            if (_t _q_method = &simulator_worker::stats_updated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (simulator_worker::*)();
            if (_t _q_method = &simulator_worker::finished; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (simulator_worker::*)(const QString & );
            if (_t _q_method = &simulator_worker::error_occurred; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject *simulator_worker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *simulator_worker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_simulator_worker.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int simulator_worker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void simulator_worker::progress_updated(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void simulator_worker::iteration_ready(const iteration_data & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void simulator_worker::stats_updated(double _t1, double _t2, double _t3, double _t4, size_t _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t5))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void simulator_worker::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void simulator_worker::error_occurred(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
