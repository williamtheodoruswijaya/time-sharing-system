#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "scheduler.h"

// define global variables yang udah di set
Process proc_table[MAX_TASK];
int total_proc = 0;
int active_idx = -1;

// algorithm buat process switching-nya (ini Round Robin)
int pick_next_process() {
    // step 1: validation -> kalau tidak ada proses aktif, return -1
    if (total_proc == 0) return -1;

    // step 2: cari process yang lagi running (state-nya adalah RUNNING dan tercatat di active_idx buat easy lookup di process table)
    int start = active_idx;

    // step 3: cari process selanjutnya yang belum DONE
    for (int i = 1; i <= total_proc; i++) {
        // step 3.1: hitung index process selanjutnya secara circular (kalau udah di ujung, balik ke awal (awal dianggep next))
        int idx = (start + i) % total_proc;

        // step 3.2: cek kalau process-nya udah DONE atau belum (kalau DONE, return index-nya/posisi-nya di process table)
        if (proc_table[idx].state != PROC_DONE) {
            return idx;
        }
    }

    // step 4: kalau semua process udah DONE, return -1
    return -1;
}

void scheduler_tick(int sig) {
    // step 1: pick index process yang lagi jalan (state = RUNNING)
    int previous = active_idx;

    // step 2: pilih process selanjutnya yang mau dijalankan (cari index-nya aja)
    int next = pick_next_process();

    // step 3: validation -> kalau semua process udah DONE/gaada process sama sekali, exit program
    if (next == -1) {
        printf("Tidak ada proses aktif\n");
        exit(0);
    }

    // step 4: switch process (preempt current process dan jalankan process baru)
    if (previous != -1 && proc_table[previous].state == PROC_RUNNING) {
        // step 4.1: stop process yang lama
        kill(proc_table[previous].pid, SIGSTOP);

        // step 4.2: update state process yang lama jadi PAUSED
        proc_table[previous].state = PROC_PAUSED;
        printf("⏸ PID %d dihentikan\n", proc_table[previous].pid);
    }

    // step 5: update index process yang lagi jalan sekarang
    active_idx = next;

    // step 6: jalankan process yang baru (sent signal SIGCONT)
    kill(proc_table[active_idx].pid, SIGCONT);

    // step 7: update state process yang baru jadi RUNNING
    proc_table[active_idx].state = PROC_RUNNING;
    printf("▶ PID %d dijalankan\n", proc_table[active_idx].pid);
}
