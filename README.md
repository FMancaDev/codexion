*This project has been created as part of the 42 curriculum by fomanca.*

# Codexion

## Description

Codexion is a concurrency simulation developed for the 42 School curriculum. The project explores synchronization between multiple POSIX threads competing for a limited set of shared resources.

The simulation models a group of coders working in a shared environment. To compile their quantum code, each coder must acquire two adjacent USB dongles. After compiling, they debug and refactor before attempting to compile again.

The simulation ends when one of the following conditions occurs:

- A coder burns out because they failed to start compiling before `time_to_burnout`.
- Every coder has completed `number_of_compiles_required` compilations.

The project focuses on solving concurrency problems while ensuring correct synchronization, fairness and safe resource sharing.

---

# Instructions

## Requirements

- POSIX Threads
- C compiler (cc)
- Make

## Compilation

```bash
make
```

Clean object files:

```bash
make clean
```

Remove executable:

```bash
make fclean
```

Recompile:

```bash
make re
```

## Execution

```bash
./codexion \
number_of_coders \
time_to_burnout \
time_to_compile \
time_to_debug \
time_to_refactor \
number_of_compiles_required \
dongle_cooldown \
scheduler
```

### Example

```bash
./codexion 5 800 200 200 200 5 20 fifo
```

Scheduler options:

- `fifo`
- `edf`

---

# Blocking Cases Handled

This implementation addresses the main concurrency problems present in the simulation.

## Deadlock prevention

Deadlocks are prevented by avoiding circular waiting during dongle acquisition. Coders do not all attempt to acquire the same dongle first, eliminating one of Coffman's necessary conditions for deadlock.

## Coffman's Conditions

The solution considers the four Coffman conditions:

- Mutual exclusion (required because dongles cannot be shared)
- Hold and wait
- No preemption
- Circular wait

The implementation specifically breaks the circular wait condition, preventing deadlocks.

## Starvation prevention

The scheduler ensures fair access to USB dongles.

Two arbitration policies are available:

- **FIFO (First In, First Out)** grants the dongle to the oldest waiting request.
- **EDF (Earliest Deadline First)** prioritizes the coder whose burnout deadline is closest.

## Dongle cooldown

Whenever a dongle is released, it enters a cooldown period during which it cannot be acquired by another coder.

This simulates temporary hardware unavailability and introduces additional synchronization challenges.

## Precise burnout detection

A dedicated monitor thread continuously checks every coder.

A coder burns out if they fail to start compiling before:

```
last_compile_start + time_to_burnout
```

When burnout occurs, the simulation immediately terminates.

## Log serialization

All output is protected using a dedicated mutex.

This guarantees that log messages are never interleaved and remain readable.

---

# Thread Synchronization Mechanisms

The implementation relies on POSIX thread synchronization primitives.

## pthread_mutex_t

Mutexes are used to protect shared resources, including:

- USB dongles
- Simulation stop state
- Console output

Only one coder may hold a dongle at any given time.

## pthread_cond_t

Condition variables are used to suspend coders while waiting for a dongle.

Whenever a dongle becomes available after being released and its cooldown expires, waiting coders are notified.

This avoids busy waiting and unnecessary CPU usage.

## Shared State Synchronization

Shared simulation state is protected using mutexes to prevent race conditions between:

- coder threads
- the monitor thread
- logging operations

This ensures that shared variables remain consistent throughout the execution.

## Thread-safe Communication

Communication between threads is achieved through shared protected state and condition variables.

The monitor detects simulation termination and signals waiting threads so they can safely exit without remaining blocked.

---

# Resources

## Documentation

- POSIX Threads Programming
  https://hpc-tutorials.llnl.gov/posix/

- POSIX Threads Manual
  https://man7.org/linux/man-pages/man7/pthreads.7.html

- pthread_mutex documentation
  https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3.html

- pthread_cond documentation
  https://man7.org/linux/man-pages/man3/pthread_cond_wait.3.html

- The Linux Programming Interface
  Michael Kerrisk

- Operating Systems: Three Easy Pieces
  https://pages.cs.wisc.edu/~remzi/OSTEP/

## AI Usage

Artificial Intelligence was used exclusively as a learning and review tool.

AI assisted with:

- understanding POSIX thread synchronization concepts;
- reviewing concurrency algorithms;
- identifying potential race conditions and edge cases;
- improving code organization and documentation;
- reviewing the README.

All design decisions, implementation details, debugging and testing were performed manually. Every AI-generated suggestion was reviewed, understood and adapted before being incorporated into the project.

---

# Project Structure

```
includes/
    codexion.h

src/
    main.c

    parser/
    dongle/
    simulation/
    utils/

Makefile
README.md
```

---

# Author

**fomanca**

Student at **42 Porto**
