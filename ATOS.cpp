//
// Created by 陶源 on 15/4/29.
//

#include "ATOS.h"

const uint8_t MAX_TASKS = 9; //max allowed g_tasks -1 (i.e.: 9 = 10-1)

static HardwareTimer *_timer = NULL;

volatile uint16_t _reset_timeout;
static bool _initialized = false;
TaskEntry g_tasks[MAX_TASKS];
static TaskEntry *g_current_task;
static uint16_t _num_task = 0;


void TaskSemaphore::signal() {
    //
    // Release the waiting task from wait state
    //
    if (_task != NULL) {
        _task->state = TASK_STATE_RUN;
        _task = NULL;
    }
}

int TaskSemaphore::wait() {
    //
    // Set the invoking task in wait state.
    // Only on task can use a sync object at a time
    //
    if (_task == NULL) {
        g_current_task->state = TASK_STATE_WAIT;
        _task = g_current_task;

        //
        // Return success
        //
        return TOS_OK;
    }

    //
    // Return error
    //
    return TOS_ERROR;
}


/////////////////////////////////////////////////////////////////


//ISR (Interrupt Service Routine)
void isr() {
    int i;
    int iRet;

    //
    // Loop through every task
    //
    for (i = 0; i < _num_task; i++) {
        //
        // Set the current task pointer
        //
        g_current_task = &g_tasks[i];

        //
        // Check if the task is active
        //
        if (TASK_STATE_RUN == g_current_task->state) {
            //
            // Check if task is not in sleep
            //
            if (g_current_task->planned < millis()) {
                //
                // Invoke the task function
                //
                iRet = g_current_task->fn(g_current_task->params);

                //
                // check the return value
                //
                if (TASK_RET_DONE == iRet) {
                    //
                    // Mark the task as done base on return value
                    //
                    g_current_task->state = TASK_STATE_DONE;
                }
            }
        }
    }
}

void ATOS::begin(uint16_t resetTimeout) {
    begin(Timer3, resetTimeout);
}

void ATOS::begin(HardwareTimer &timer, uint16_t resetTimeout) {
    _initialized = true;
    _timer = &timer;
    _timer->init(1000);
    _timer->attachInterrupt(isr);
    _reset_timeout = resetTimeout;
}

void ATOS::reset() {
    // TODO
}

int ATOS::findTask(P_TOS_TASK_FN fn) {
    if (fn == NULL) return -1;
    for (int i = 0; i < _num_task; ++i) {
        if (g_tasks[i].fn == *fn) return i;
    }
    return -1;
}

int ATOS::findTask(const char *name) {
    if (name == NULL) return -1;
    for (int i = 0; i < _num_task; ++i) {
        if (g_tasks[i].name == NULL) continue;
        if (!(strcmp(g_tasks[i].name, name))) return i;
    }
    return -1;
}

int ATOS::createTask(P_TOS_TASK_FN fn) {
    return createTask(fn, NULL, NULL);
}

int ATOS::createTask(P_TOS_TASK_FN fn, const char *name) {
    return createTask(fn, name, NULL);
}

int ATOS::createTask(P_TOS_TASK_FN fn, void *params) {
    return createTask(fn, NULL, params);
}

int ATOS::createTask(P_TOS_TASK_FN fn, const char *name, void *params) {
    if (!_initialized || _num_task >= MAX_TASKS) {
        return TOS_ERROR;
    }

    //
    // Initialize the task list if this is the first task created
    //
    if (_num_task == 0) {
        memset((char *) g_tasks, 0, sizeof(g_tasks));
    }

    //
    // Create the task
    //
    if (_num_task < MAX_TASKS) {
        g_tasks[_num_task].fn = fn;
        g_tasks[_num_task].state = TASK_STATE_RUN;
        g_tasks[_num_task].params = params;
        g_tasks[_num_task].planned = 0;
        _num_task++;

        return TOS_OK;
    }

    return TOS_OK;
}

void ATOS::sleep(unsigned long ms) {
    g_current_task->planned = millis() + ms;
}

int ATOS::stop() {
    if (!_initialized) {
        return TOS_ERROR;
    }
    _timer->stop();
    return TOS_OK;
}

int ATOS::start() {
    if (!_initialized) {
        return TOS_ERROR;
    }
    _timer->start();
    return TOS_OK;
}
