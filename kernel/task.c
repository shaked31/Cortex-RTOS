#include "../include/kernel/task.h"
#include "../include/drivers/timer.h"
#include "../include/drivers/gic.h"
#include "../arch/arm/v7-a/cpu.h"
#include "../include/lib/stdio.h"

static tcb_t tasks[MAX_TASKS], idle_tcb;
static int curr_task = -1;
static int task_count = 0;

static void idle_task() {
    while (1)
        __asm__ volatile("wfi");
}

static void init_stack(tcb_t* tcb, void* pfunc) {
    uint32_t* sp = &tcb->stack[TASK_STACK_SIZE]; // stack (sp) starts at the top and goes down

    *--sp = 0x13; // current program set regigster (cpsr) - set supervisor mode
    *--sp = (uint32_t)pfunc; // program counter (pc register)
    *--sp = 0; // link register is set to 0 - kernel task isn't suppose to return a value

    for (int i = 0 ; i <= 12 ; i++) {
        *--sp = 0; // clear all the general purpose registers (r0-r12)
    }

    tcb->sp = sp; // pointer to sp that can be used by the task
}

void task_init() {
    for (int i = 0 ; i < MAX_TASKS ; i++) {
        tasks[i].state = TASK_EMPTY;
        tasks[i].sleep_ticks = 0;
    }
    init_stack(&idle_tcb, &idle_task);
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
    
    init_stack(&tasks[id], task_func);
    tasks[id].id = id;
    tasks[id].state = TASK_READY;
    task_count++;

    mini_printf("Task %d created | Code at %x | SP: %x\n", id, (uint32_t)task_func, tasks[id].sp);

    return id;
}

uint32_t scheduler(uint32_t curr_sp, int is_irq) {
    if (is_irq) {
        uint32_t irq_id = GICC_IAR;
        if (irq_id == TIMER0_INT_VALUE) { // timer interrupt has been triggered
            // Clear the timer interrupt
            sp804_timer_t* timer = (sp804_timer_t*)TIMER0_BASE;
            timer->int_clr = 1;

            for (int i = 0 ; i < MAX_TASKS ; i++) {
                if (tasks[i].state == TASK_SLEEPING) {
                    tasks[i].sleep_ticks--;
                    if (tasks[i].sleep_ticks == 0)
                        tasks[i].state = TASK_READY;
                }
            }
        }
        GICC_EOIR = irq_id; // Acknowledge End Of Interrupt
    }


    if (curr_task != -1) {
        tasks[curr_task].sp = (uint32_t*)curr_sp;

        // if blocked, current tasks state wont change to ready
        if (tasks[curr_task].state == TASK_RUNNING)
            tasks[curr_task].state = TASK_READY;
    }
    else // current task is the idle task
        idle_tcb.sp = (uint32_t*)curr_sp;

    if (task_count > 0) {
        int search_limit = MAX_TASKS;
        int next_candidate = curr_task;

        while (search_limit > 0) {
            if (++next_candidate >= MAX_TASKS) {
                // if got to the last task, should return to the first one
                next_candidate = 0;
            }
            if (tasks[next_candidate].state == TASK_READY) {
                curr_task = next_candidate;
                tasks[curr_task].state = TASK_RUNNING;
                return (uint32_t)tasks[curr_task].sp;
            }
            search_limit--;
        }
    }

    // no tasks are ready, set curr_task to -1 (idle) and return the idle task
    curr_task = -1;
    return (uint32_t)idle_tcb.sp;
}

void task_sleep(uint32_t ticks) {
    if (curr_task != -1 && ticks > 0) {
        disable_interrupts();
        tasks[curr_task].state = TASK_SLEEPING;
        tasks[curr_task].sleep_ticks = ticks;
        enable_interrupts();

        task_yield();
    }

}

tcb_t* task_get_current() {
    if (curr_task != -1)
        return &tasks[curr_task];
    return 0;
}

void task_block_current() {
    if (curr_task != -1)
        tasks[curr_task].state = TASK_BLOCKED;
}

void task_unblock(int id) {
    if (id >= 0 && id < MAX_TASKS)
        if (tasks[id].state == TASK_BLOCKED)
            tasks[id].state = TASK_READY;
}