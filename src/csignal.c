/**
 * Grupo 40
 * Rodrigo Frutuoso - 61865
 * Simão Alexandre - 61874
 * Tiago Leite - 61863
*/

#include "../inc/csignal.h"

static struct info_container* g_info = NULL;
static struct transaction* g_transactions = NULL;
static int g_num_transactions = 0;

void alarm_handler(int signo) {
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);

    if (!g_transactions) return;

    printf("[Alarme] Estado das transações:\n");
    for (int i = 0; i < g_num_transactions; i++) {
        struct transaction *tx = &g_transactions[i];
        if (tx->change_time.created.tv_sec != 0) {
            double elapsed;
            if (tx->change_time.server_signed.tv_sec != 0) {
                elapsed = (tx->change_time.server_signed.tv_sec - tx->change_time.created.tv_sec)
                        + (tx->change_time.server_signed.tv_nsec - tx->change_time.created.tv_nsec) / 1e9;
            } else {
                elapsed = (now.tv_sec - tx->change_time.created.tv_sec)
                        + (now.tv_nsec - tx->change_time.created.tv_nsec) / 1e9;
            }
            printf("%d %.3f\n", tx->id, elapsed);
        }
    }
    fflush(stdout);
}

void setup_periodic_alarm(int period, struct info_container *info, struct transaction* transactions, int num_transactions) {
    if (period <= 0) return;
    g_info = info;
    g_transactions = transactions;
    g_num_transactions = num_transactions;

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

void sigint_handler(int signo) {
    *g_info->terminate = 1;
}

void setup_sigint_handler(struct info_container *info)
{
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    g_info = info;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Error setting up SIGINT handler");
        exit(1);
    }
}
