#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/wait.h>

#include "scheduler.h"

void worker_job(int id);

int main(int argc, char *argv[]) { // argc -> validasi isi process (harus 2), argv[1] -> ambil isi process
    // step 1: validasi kalau proses-nya bukan 2, 
    if (argc != 2) {
        printf("Usage: %s <jumlah_proses>\n", argv[0]);
        return 1;
    }
    
    // step 2: ambil jumlah proses dari argumen (jadiin int)
    total_proc = atoi(argv[1]);

    // step 3: validasi jumlah proses (ga boleh kurang dari 1 atau lebih dari ukuran process table)
    if (total_proc < 1 || total_proc > MAX_TASK) {
        printf("Jumlah proses harus 1 - %d\n", MAX_TASK);
        return 1;
    }

    // step 4: masukin proses-nya ke process table
    for (int i = 0; i < total_proc; i++) {
        // step 4.1: buat child process
        pid_t pid = fork();

        // step 4.2: kalau fork gagal, stop aja programnya
        if (pid == 0) {
            raise(SIGSTOP);     // send SIGSTOP ke diri sendiri supaya berhenti dulu
            worker_job(i);      // jalankan kerjaan proses
            exit(0);
        }

        // step 4.3: di parent, simpan info proses ke process table
        proc_table[i].pid = pid;

        // step 4.4: set state proses ke NEW
        proc_table[i].state = PROC_NEW;
        printf("Process %d dibuat (PID=%d)\n", i, pid);
    }

    // step 5: inisialisasi scheduler-nya
    struct sigaction sa;                // ini handler buat SIGALRM
    sa.sa_handler = scheduler_tick;     // pas SIGALRM dipanggil, call scheduler_tick()
    sigemptyset(&sa.sa_mask);           
    sa.sa_flags = SA_RESTART;           // biar syscall yang ke-interrupt di-restart
    sigaction(SIGALRM, &sa, NULL);      // pasang handler SIGALRM

    // step 6: buat timer yang bakal call SIGALRM tiap QUANTUM detik
    struct itimerval timer;
    timer.it_value.tv_sec = QUANTUM;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = QUANTUM;
    timer.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);

    printf("\nRound Robin Scheduler Started\n");

    // step 7: start dari proses pertama, set active_idx, terus kirim SIGCONT ke proses pertama
    active_idx = 0;
    kill(proc_table[0].pid, SIGCONT);
    proc_table[0].state = PROC_RUNNING;

    // step 8: lakuin infinite loop
    while (1) {
        pause(); // pause program sampai ada signal masuk (misal SIGALRM dari timer)
    }

    return 0;
}
