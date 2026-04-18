#include "../include/kernel/sync.h"
#include "../include/kernel/task.h"
#include "../include/lib/stdio.h"
#include "../arch/arm/v7-a/cpu.h"

void mutex_init(mutex_t* m) {
    m->lock_state = 0;
    m->owner_task_id = -1;
    m->wait_queue_head = 0;
    m->wait_queue_tail = 0;
}

void mutex_lock(mutex_t* m) {
    disable_interrupts();  // prevent timer from interrupting the lock
    tcb_t* curr_task = task_get_current();

    if (curr_task == 0) {
        mini_printf("Error: tried to take mutex outside of a task, halting...");
        enable_interrupts();
        while(1); // halt the system
    }

    if (m->lock_state == LOCK_FREE) {
        m->lock_state = LOCK_TAKEN;
        m->owner_task_id = curr_task->id;
        enable_interrupts();
    }

    else { // lock is taken, join the waiting queue
        curr_task->next = 0; // next task is null (current task will be the last one)

        if (m->wait_queue_tail == 0) { // empty queue
            m->wait_queue_tail = curr_task;
            m->wait_queue_head = curr_task;
        }
        else { // make current task the last
            m->wait_queue_tail->next = curr_task;
            m->wait_queue_tail = curr_task;
        }

        curr_task->state = TASK_BLOCKED;
        enable_interrupts();
        task_yield();
    }
}

void mutex_unlock(mutex_t* m) {
    disable_interrupts();

    if (m->wait_queue_head != 0) { // tasks are waiting
        tcb_t* next_task = m->wait_queue_head;

        m->wait_queue_head = next_task->next;
        if (m->wait_queue_head == 0)
            m->wait_queue_tail = 0;

        next_task->next = 0;
        m->owner_task_id = next_task->id;
        next_task->state = TASK_READY;
        
        enable_interrupts();
        task_yield();
    }

    else { // no tasks waiting
        m->lock_state = LOCK_FREE;
        m->owner_task_id = -1;
        enable_interrupts();
    }
}