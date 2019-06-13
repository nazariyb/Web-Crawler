#ifndef LAB4_TASK_H
#define LAB4_TASK_H

#include <qt5/QtCore/QThreadPool>
#include <atomic>

#include "thread_safe_queue.h"
#include "function_wrapper.h"
#include "../reader/word_usings.h"

class Task : public QRunnable
    {
    public:
    function_wrapper func;

    explicit Task (function_wrapper& func_) : QRunnable() { func = std::move(func_); }

    void run () override { func(); };

    };


#endif //LAB4_TASK_H
