#ifndef TASK_H
#define TASK_H

#include <stdint.h>

#define MAX_TASKS 4
#define TASK_STACK_SIZE 1024 // 4KB stack per task

#define TASK_EMPTY 0
#define TASK_READY 1
#define TASK_RUNNING 2
#define TASK_BLOCKED 3


typedef struct tcb {
    uint32_t* sp;       // current stack pointer
    uint32_t id;        // task id
    uint32_t state;     // task state
    uint32_t stack[TASK_STACK_SIZE];
    struct tcb* next;
} tcb_t;

void task_init();
int task_create(void (*task_func)());
uint32_t scheduler(uint32_t curr_sp);
int task_get_current();
void task_block_current();
void task_unblock(int id);


static inline void task_yield() {
    __asm__ volatile(
        "push {lr}\n"
        "svc #0\n" // trigger software interrupt that is handled in vector_table.s (svc_handler label)
        "pop {lr}\n"
        : : : "memory"
    );
}

#endif