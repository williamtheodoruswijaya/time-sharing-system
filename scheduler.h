#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <sys/types.h>

#define MAX_TASK 10                     // maximum total process yang boleh jalan
#define QUANTUM 5                       // lama proses harus jalan sebelum switch/preempt (Round Robin)

typedef enum {                          // processes States (NEW, RUNNING, PAUSED, DONE)
    PROC_NEW,
    PROC_RUNNING,
    PROC_PAUSED,
    PROC_DONE
} ProcState;

typedef struct {                        // PCB (Process Control Block)
    pid_t pid;
    ProcState state;
} Process;

// Global Variables
extern Process proc_table[MAX_TASK];    // process Table (isi-nya PCB-PCB dengan maks isinya MAX_TASK)
extern int total_proc;                  // total process yang ada di Process Table
extern int active_idx;                  // index dari Process (based on Process Table) yang lagi dalam state RUNNING

int pick_next_process(void);            // abstract Function buat milih process selanjut-nya (ini kita pakai algorithm Round Robin disini)
void scheduler_tick(int sig);           // function buat switch process setelah sebuah process berjalan selama X Quantum detik

#endif
