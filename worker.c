#include <stdio.h>
#include <unistd.h>
#include <signal.h>

// simulasi Process yang dijalankan
void worker_job(int id) {
    // process bakal run secara ganti-gantian di jalankan sama scheduler (diatur oleh state process yang diatur oleh SIGNAL" yang diatur oleh Scheduler)
    while (1) {
        printf("[Worker %d | PID %d] sedang berjalan\n", id, getpid());
        fflush(stdout);

        // busy loop (anggep aja setelah print atas, process-nya ngejalanin sesuatu yang butuh high compute power (busy loop))
        for (volatile long i = 0; i < 150000000; i++);
    }
}
