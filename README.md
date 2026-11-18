*This project has been created as part of the 42 curriculum by
tsmalala.*

# Codexion

## Description

Codexion is a concurrency project written in C using POSIX threads
(`pthreads`). It simulates a group of coders arranged in a circular
topology. Each coder needs access to the two shared dongles located next
to them before starting a compilation.

A coder repeatedly performs the following cycle:

1.  Requests access to its two adjacent dongles.
2.  Waits until the scheduler allows it to proceed and the dongles can
    be used.
3.  Compiles for the configured duration.
4.  Releases both dongles.
5.  Debugs and refactors before requesting dongles again.

The program runs one thread per coder and a separate monitor thread that
checks whether a coder has exceeded the allowed time without starting
another compilation.

Two scheduling policies are available:

-   **FIFO (First In, First Out):** scheduling based on dongle-request
    order.
-   **EDF (Earliest Deadline First):** scheduling based on the coders'
    calculated burnout deadlines.

The main goal is to practice thread creation, shared-resource
management, mutex synchronization, scheduling, and the detection of
concurrency problems such as deadlocks and starvation.

## Features

-   One POSIX thread per coder.
-   A monitor thread dedicated to burnout detection.
-   Circular arrangement of coders and shared dongles.
-   FIFO and EDF scheduling modes.
-   Configurable compilation, debugging, refactoring, burnout, and
    dongle-cooldown times.
-   Serialized terminal output.
-   A finite number of compilations per coder.

## Instructions

### Compilation

From the root of the repository, run:

``` bash
make
```

To remove object files:

``` bash
make clean
```

To remove object files and the executable:

``` bash
make fclean
```

To perform a clean rebuild:

``` bash
make re
```

The executable produced by the Makefile is named `codexion`.

### Execution

The program expects exactly eight arguments:

``` bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles dongle_cooldown scheduler
```

All duration arguments are expressed in milliseconds.

  ---------------------------------------------------------------------
  Argument                           Description
  ---------------------------------- ----------------------------------
  `number_of_coders`                 Number of coders (must be greater
                                     than zero).

  `time_to_burnout`                  Maximum allowed time without
                                     starting a new compilation.

  `time_to_compile`                  Duration of one compilation.

  `time_to_debug`                    Duration of the debugging phase.

  `time_to_refactor`                 Duration of the refactoring phase.

  `number_of_compiles`               Number of compilations each coder
                                     must complete.

  `dongle_cooldown`                  Minimum cooldown duration after a
                                     dongle's previous use.

  `scheduler`                        Scheduling policy: `fifo` or
                                     `edf`.
  ---------------------------------------------------------------------

Example using FIFO:

``` bash
./codexion 5 800 200 200 200 5 0 fifo
```

Example using EDF:

``` bash
./codexion 5 800 200 200 200 5 0 edf
```

The program prints timestamped events, including dongle acquisition,
compilation, debugging, refactoring, and burnout detection.

## Blocking cases handled

### Deadlock prevention and Coffman's conditions

Each coder needs two dongles. To avoid a circular wait while acquiring
them, the implementation orders the two mutex acquisitions by dongle ID:
the lower-ID dongle is locked before the higher-ID dongle.

This resource-ordering strategy prevents the circular-wait condition
described in Coffman's deadlock model. The implementation does not claim
to remove every Coffman condition individually; it prevents deadlock by
imposing a consistent global acquisition order.

### Starvation and scheduling

The scheduler provides two policies, FIFO and EDF, to decide whether a
coder should wait before attempting to acquire its dongles. FIFO uses
request timing, while EDF compares calculated burnout deadlines.

These policies are intended to coordinate competing requests and reduce
the risk of a coder being repeatedly bypassed. A scheduling policy alone
is not a formal proof of starvation freedom: that guarantee depends on
the correctness of the priority checks and state transitions.

### Dongle cooldown

Each dongle stores the time of its last use. Before a coder proceeds
with a compilation, the program checks whether the cooldown interval has
elapsed for both dongles. If the cooldown has not elapsed, the coder
releases the dongle locks and retries later.

### Burnout detection

A dedicated monitor thread checks coders that have not yet completed
their required number of compilations. It compares the elapsed time
since each coder's last compilation with the configured burnout limit.

The shared burnout flag is protected by a mutex. The last-compilation
timestamp is also protected when it is read or updated. When a burnout
is detected, the monitor sets the shared flag so coder threads can stop
their work.

The time utility measures elapsed time in milliseconds, and the monitor
polls periodically; detection is therefore performed at millisecond
resolution rather than by a real-time deadline interrupt.

### Log serialization

All terminal messages pass through a shared `printf_mutex`. This
prevents two threads from writing their messages simultaneously and
interleaving their output.

## Thread synchronization mechanisms

### `pthread_mutex_t`

This implementation uses POSIX mutexes (`pthread_mutex_t`) to protect
shared state and shared resources.

  ---------------------------------------------------------------------
  Mutex                              Protected data / purpose
  ---------------------------------- ----------------------------------
  `is_free_mutex` in each dongle     Controls exclusive access to a
                                     dongle and protects its
                                     availability during acquisition
                                     and release.

  `is_waiting_mutex` in each coder   Protects the coder's waiting
                                     state.

  `last_compilation_mutex` in each   Protects the timestamp of the
  coder                              coder's last compilation.

  `compilation_mutex` in each coder  Protects the number of completed
                                     compilations.

  `has_burnout_mutex`                Protects the shared burnout flag.

  `printf_mutex`                     Serializes terminal output.
  ---------------------------------------------------------------------

Examples of race prevention:

-   A coder locks a dongle's mutex before changing its availability, so
    another coder cannot acquire the same dongle at the same time.
-   The monitor locks `last_compilation_mutex` before reading a coder's
    last-compilation timestamp, while the coder locks the same mutex
    when updating it.
-   The monitor and coder threads use `compilation_mutex` when accessing
    the completed-compilation counter.
-   Reads and writes of the shared burnout flag are intended to be
    coordinated with `has_burnout_mutex`.
-   `printf_mutex` ensures that each log operation is printed as one
    serialized operation.

### Condition variables and custom events

This version does **not** use `pthread_cond_t` condition variables or a
separate custom event implementation. Waiting is implemented through
scheduler checks and short sleeps (`usleep`), while mutexes protect the
shared state being checked.

Consequently, thread communication is based on shared state guarded by
mutexes and periodic polling, not on condition-variable notifications or
custom event signals.

## Project structure

``` text
.
├── Makefile
├── README.md
└── Codexion/
    ├── main.c
    ├── main.h
    ├── parsing.c
    ├── initialization.c
    ├── coder_routine.c
    ├── scheduler.c
    ├── burnout.c
    ├── threads.c
    └── time_utils.c
```

-   `main.c`: program entry point, initialization, linking, and cleanup.
-   `parsing.c`: command-line argument parsing and validation.
-   `initialization.c`: creation and initialization of coders and
    dongles.
-   `coder_routine.c`: coder thread routine and compilation lifecycle.
-   `scheduler.c`: FIFO and EDF scheduling decisions.
-   `burnout.c`: monitor thread and burnout checks.
-   `threads.c`: creation and joining of threads.
-   `time_utils.c`: elapsed-time calculations, cooldown checks, and log
    serialization.

## Resources

### Technical documentation

-   [POSIX Threads (pthreads) --- Linux manual
    pages](https://man7.org/linux/man-pages/man7/pthreads.7.html)
-   [pthread_mutex_lock --- Linux manual
    page](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)
-   [pthread_create --- Linux manual
    page](https://man7.org/linux/man-pages/man3/pthread_create.3.html)
-   [pthread_join --- Linux manual
    page](https://man7.org/linux/man-pages/man3/pthread_join.3.html)
-   [The Open Group --- POSIX
    Threads](https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
-   [GNU Make Manual](https://www.gnu.org/software/make/manual/)

### AI usage

AI assistance was used as a learning and review aid during development.
It was used for:

-   Explaining POSIX thread concepts, mutexes, race conditions,
    deadlocks, and scheduling policies.
-   Helping analyze terminal outputs and reason about concurrency bugs.

AI suggestions were treated as guidance, not as a substitute for
understanding or testing the implementation. The code was integrated,
adapted, and tested by the project author. The README describes the
implementation present in this project version; it does not claim that
condition variables or custom events are implemented.
