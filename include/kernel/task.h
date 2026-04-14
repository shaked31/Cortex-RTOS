#ifndef TASK_H
#define TASK_H

#include <stdint.h>

#define MAX_TASKS 4
#define TASK_STACK_SIZE 1024 // 4KB stack per task

#define TASK_EMPTY 0
#define TASK_READY 1
#define TASK_RUNNING 2


typedef struct {
    uint32_t* sp;       // current stack pointer
    uint32_t id;        // task id
    uint32_t state;     // task state
    uint32_t stack[TASK_STACK_SIZE];
} tcb_t;

void task_init();
int task_create(void (*task_func)());
uint32_t scheduler(uint32_t curr_sp);

#endif