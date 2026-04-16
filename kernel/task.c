#include "../include/kernel/task.h"
#include "../include/drivers/timer.h"
#include "../include/drivers/gic.h"
#include "../include/lib/stdio.h"

static tcb_t tasks[MAX_TASKS];
static int curr_task = -1;
static int task_count = 0;

void task_init() {
    for (int i = 0 ; i < MAX_TASKS ; i++) {
        tasks[i].state = TASK_EMPTY;
    }

    mini_printf("Task system initialized\n");
}

int task_create(void (*task_func)()) {
    if (task_count >= MAX_TASKS) {
        mini_printf("Error: Max tasks reached\n");
        return -1;
    }

    int id = -1;
    for (int i = 0 ; i < MAX_TASKS ; i++) {
        if (tasks[i].state == TASK_EMPTY) {
            id = i;
            break;
        }
    }

    if (id == -1)
        return -1;
    
    tcb_t* tcb = &tasks[id];
    tcb->id = id;
    tcb->state = TASK_READY;

    uint32_t* sp = &tcb->stack[TASK_STACK_SIZE]; // stack (sp) starts at the top and goes down

    *--sp = 0x13; // current program set regigster (cpsr) - set supervisor mode
    *--sp = (uint32_t)task_func; // program counter (pc register)
    *--sp = 0; // link register is set to 0 - kernel task isn't suppose to return a value

    for (int i = 0 ; i <= 12 ; i++) {
        *--sp = 0; // clear all the general purpose registers (r0-r12)
    }

    tcb->sp = sp; // pointer to sp that can be used by the task
    task_count++;

    mini_printf("Task %d created | Code at %x | SP: %x\n", tcb->id, (uint32_t)task_func, sp);

    return tcb->id;
}

uint32_t scheduler(uint32_t curr_sp) {
    uint32_t irq_id = GICC_IAR;

    if (irq_id == TIMER0_INT_VALUE) { // timer interrupt has been triggered
        // Clear the timer interrupt
        sp804_timer_t* timer = (sp804_timer_t*)TIMER0_BASE;
        timer->int_clr = 1;
    }

    if (curr_task != -1) {
        tasks[curr_task].sp = (uint32_t*)curr_sp;
        tasks[curr_task].state = TASK_READY;
    }

    if (task_count > 0) {
        do {
            curr_task++;
            if (curr_task >= task_count)
                curr_task = 0; // if got to the last task, should return to the first one

        } while (tasks[curr_task].state != TASK_READY);        
        tasks[curr_task].state = TASK_RUNNING;
    }

    GICC_EOIR = irq_id; // Acknowledge End Of Interrupt

    return (uint32_t)tasks[curr_task].sp;
}

int task_get_current() {
    if (curr_task != -1)
        return &tasks[curr_task];
    return 0;
}

void task_block_current() {
    if (curr_task != -1)
        tasks[curr_task].state = TASK_BLOCKED;
}

void task_unblock(int id) {
    if (id >= 0 && id <= MAX_TASKS)
        tasks[curr_task].state = TASK_READY;
}