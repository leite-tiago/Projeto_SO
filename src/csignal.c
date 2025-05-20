#include "../inc/csignal.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

static struct info_container* g_info = NULL;

// Variável global para sinalizar término (deve ser usada no main loop)
volatile sig_atomic_t sigint_received = 0;

// Handler do alarme
void alarm_handler(int signo) {
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);

    if (!g_info || !g_info->tx_times) return;

    printf("[Alarme] Estado das transações:\n");
    for (int i = 0; i < g_info->max_txs; i++) {
        if (g_info->tx_times[i].created.tv_sec != 0) {
            double elapsed;
            if (g_info->tx_times[i].server_signed.tv_sec != 0) {
                elapsed = (g_info->tx_times[i].server_signed.tv_sec - g_info->tx_times[i].created.tv_sec)
                        + (g_info->tx_times[i].server_signed.tv_nsec - g_info->tx_times[i].created.tv_nsec) / 1e9;
            } else {
                elapsed = (now.tv_sec - g_info->tx_times[i].created.tv_sec)
                        + (now.tv_nsec - g_info->tx_times[i].created.tv_nsec) / 1e9;
            }
            printf("%d %.3f\n", i, elapsed);
        }
    }
    fflush(stdout);
}

// Setup do alarme periódico
void setup_periodic_alarm(int period, struct info_container *info) {
    if (period <= 0) return; // Não ativa se período for 0
    g_info = info; // Se precisares de info no handler

    struct sigaction sa;
    sa.sa_handler = alarm_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGALRM, &sa, NULL) == -1) {
        perror("Error setting up SIGALRM handler");
        exit(1);
    }

    struct itimerval timer;
    timer.it_value.tv_sec = period;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = period;
    timer.it_interval.tv_usec = 0;
    if (setitimer(ITIMER_REAL, &timer, NULL) == -1) {
        perror("Error setting periodic timer");
        exit(1);
    }
}

/* Handler para SIGINT (CTRL+C) */
void sigint_handler(int signo) {
    sigint_received = 1;
    // Opcional: imprime mensagem
    // write(STDOUT_FILENO, "\n[Main] SIGINT recebido! A terminar...\n", 39);
}

/* Inicializa o handler para SIGINT (CTRL+C) */
void setup_sigint_handler(struct info_container *info)
{
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Error setting up SIGINT handler");
        exit(1);
    }
}
