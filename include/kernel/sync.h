#ifndef SYNC_H
#define SYNC_H

#include <stdint.h>
#include "task.h"

#define LOCK_FREE 0
#define LOCK_TAKEN 1

typedef struct {
    volatile uint32_t lock_state;
    int owner_task_id;
    tcb_t* wait_queue_head;
    tcb_t* wait_queue_tail;
} mutex_t;

void mutex_init(mutex_t* m);
void mutex_lock(mutex_t* m);
void mutex_unlock(mutex_t* m);

#endif