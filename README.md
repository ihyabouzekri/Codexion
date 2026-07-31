*This project has been created as part of the 42 curriculum by ibouzekr.*

# Codexion

## Description

Codexion is a multithreaded C simulation of the classic **dining philosophers problem**, wrapped in a "coding team" theme: instead of philosophers sharing forks, *coders* compete for *dongles* to *compile* code. Each coder repeatedly tries to grab the two dongles next to it, compiles for a fixed duration, then debugs and refactors before starting over. A coder that goes too long without compiling "burns out" and the simulation stops.

The goal of the project is to correctly manage several threads sharing a limited set of resources without data races, deadlocks, or starvation, while keeping accurate timing and logging of each coder's state.

## Instructions

**Compilation**

```bash
make
```

This builds the `codexion` executable with `-Wall -Wextra -Werror -pthread`.

Other available targets:

```bash
make clean   # remove object files
make fclean  # remove object files and the executable
make re      # rebuild from scratch
```

**Execution**

```bash
./codexion number_of_coders burnout_time compile_time debug_time refact_time nb_compilation dongle_cooldown scheduler
```

- `number_of_coders` – number of coder threads (and dongles) in the simulation
- `burnout_time` – time (ms) a coder can go without compiling before it burns out
- `compile_time` – time (ms) spent compiling once both dongles are held
- `debug_time` – time (ms) spent debugging after releasing the dongles
- `refact_time` – time (ms) spent refactoring before requesting dongles again
- `nb_compilation` – number of times each coder must compile before the simulation stops successfully
- `dongle_cooldown` – minimum time (ms) a dongle must rest after being released before it can be taken again
- `scheduler` – request ordering policy, either `fifo` or `edf`

Example:

```bash
./codexion 5 800 200 200 100 7 50 edf
```

## Blocking cases handled

- **Deadlock (circular wait):** 
coders pick up their two dongles in a fixed alternating order (even-numbered coders take the right dongle first, odd-numbered coders take the left one first), breaking the circular dependency that causes classic deadlock.
- **Hold-and-wait prevention:** if a coder acquires one dongle but cannot get the second, it releases the first dongle and retries instead of holding it indefinitely, so other coders are never permanently blocked behind it.
- **Starvation:** each dongle keeps a min-heap of pending requests ordered either by arrival order (`fifo`) or by compile deadline (`edf`), so a coder can only take a dongle when it is at the front of that queue, guaranteeing every coder eventually gets served.
- **Single-coder edge case:** with only one coder, no second dongle is ever available, so this case is detected up front and handled separately to avoid an unsolvable deadlock.
- **Death/burnout detection:** a dedicated monitor thread periodically checks each coder's last compile time under its mutex, so it can never read a value that is being updated mid-write, and it stops the simulation as soon as it detects a burnout.

## Thread synchronization mechanisms

- **Per-coder mutex** (`t_coder.mutex`): protects each coder's shared state (`last_compile`, `compiles_done`) from concurrent read/write access by the coder thread and the monitor thread.
- **Per-dongle mutex** (`t_dongle.lock`): protects a dongle's availability flag and release timestamp when it is taken or given back and protects the min-heap of pending requests during push/pop operations.
- **Counter mutex** (`t_data.counter_mutex`): guarantees a unique, race-free arrival number for every dongle request, used for FIFO ordering and deadline tie-breaking.
- **Simulation mutex + condition variable** (`t_data.simulation_mutex`, `t_data.ready_cond`): used to hold every thread at the starting line until the simulation is marked ready, then release them all together with a broadcast. (`t_data.simulation_mutex`) is used in other parts of the code like to protect simualtion running check or stop.
- **Log mutex** (`t_data.log_mutex`): serializes console output and the burnout announcement so log lines from different threads are never interleaved.

## Resources

- 42 project subject
- `man pthread_create`, `man pthread_mutex_lock`, `man pthread_cond_wait`, `man gettimeofday`
- General references on the dining philosophers problem and deadlock-avoidance strategies 
- min heap: https://www.geeksforgeeks.org/dsa/introduction-to-min-heap-data-structure/

**AI usage:** AI assistance was used for debugging thread-synchronization issues (identifying race conditions and reviewing mutex placement) and for reviewing README.