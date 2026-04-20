# Cortex-RTOS

Cortex-RTOS is a custom, lightweight Real-Time Operating System (RTOS) built from scratch for the ARMv7-A architecture (specifically targeting the Cortex-A8 processor on the RealView PB board).

This project serves as an educational and foundational embedded systems project, demonstrating core operating system concepts such as context switching, hardware interrupt handling, synchronization, and power management.

## 🚀 Features Implemented

* **Preemptive Task Scheduler:** A round-robin scheduler managing task states (`READY`, `RUNNING`, `BLOCKED`, `SLEEPING`).
* **Hardware Interrupt Handling:** Integrated Generic Interrupt Controller (GIC) driver.
* **System Timer:** SP804 timer driver providing the system "tick" for the scheduler.
* **Software Interrupts (SVC):** Assembly-level wrappers for `SVC` instructions to handle voluntary context switching (`task_yield`).
* **Synchronization (Mutex):** Blocking mutexes (`mutex_lock` / `mutex_unlock`) to safely protect shared resources without busy-waiting.
* **Power Management (Idle Task & Sleep):** * `task_sleep(ticks)` implementation allowing tasks to yield the CPU for a specific duration.
    * A dedicated **Idle Task** that executes the ARM `wfi` (Wait For Interrupt) instruction, halting CPU clocks when no tasks are ready, significantly reducing power consumption.
* **UART Driver & Standard I/O:** Basic UART driver with a custom `mini_printf` implementation for debugging and output.

## 📁 Project Structure

~~~text
Cortex-RTOS/
├── apps/               # User applications and exception handlers (main.c, exceptions.c)
├── arch/arm/v7-a/      # Architecture-specific assembly (startup, vector tables, cpu.h)
├── drivers/            # Hardware drivers (GIC, UART, SP804 Timer)
├── include/            # Header files for kernel, drivers, and standard lib
├── kernel/             # Core OS logic (Task Scheduler, Mutex/Sync)
├── lib/                # Standard library implementations (stdio/mini_printf)
├── scripts/            # Linker scripts (linker.ld)
├── Makefile            # Build system rules
└── Dockerfile          # Containerized build environment
~~~

## 🛠️ Prerequisites

To build and run this project natively, you need the ARM cross-compilation toolchain and the QEMU emulator.

**Local Installation (Ubuntu/Debian):**
~~~bash
sudo apt-get update
sudo apt-get install gcc-arm-none-eabi binutils-arm-none-eabi qemu-system-arm make
~~~

## ⚙️ Building and Running (Local)

The project includes a `Makefile` to simplify the build process.

1. **Build the kernel:**
   ~~~bash
   make all
   ~~~
   This will compile the source code and generate `build/kernel.elf` and `build/kernel.bin`.

2. **Run in QEMU:**
   ~~~bash
   make run
   ~~~
   This command launches the `qemu-system-arm` emulator simulating the `realview-pb-a8` machine with the compiled kernel.

3. **Clean the build directory:**
   ~~~bash
   make clean
   ~~~

## 🐳 Building and Running (Using Docker)

A `Dockerfile` is provided to encapsulate the build environment and the QEMU emulator. The container automatically copies the source code, installs dependencies, and is configured to run the OS by default.

1. **Build the Docker Image:**
   Run this from the root of the project to create the image:
   ~~~bash
   docker build -t cortex-rtos .
   ~~~

2. **Run the OS in QEMU:**
   Because the container's default command is set to `make run`, you simply need to start it. The `-it` flags are required to keep the terminal interactive for UART output:
   ~~~bash
   docker run -it --rm cortex-rtos
   ~~~
   *(Note: To exit QEMU, press `Ctrl+A` followed by `X`)*


## 💻 Current Demo (`apps/main.c`)

The current entry point demonstrates two tasks (Task A and Task B) safely accessing the UART driver using a Mutex. After printing, each task calls `task_sleep(50)` to yield the CPU, allowing the system to fall back to the Idle Task and utilize the `wfi` power-saving instruction until the next timer tick.