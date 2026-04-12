#include "../include/lib/stdio.h"

void undefined_handler() {
    mini_printf("[Exception] Undefined instruction! Halting...\n");
    while (1);
}

void data_abort_handler() {
    mini_printf("[Exception] Data abort (memory error)! Halting...\n");
    while (1);
}

void fatal_reset_handler() {
    mini_printf("[Exception] Unexpected system reset! Halting...\n");
    while (1);
}