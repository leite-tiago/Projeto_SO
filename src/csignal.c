/**
 * Grupo 40
 * Rodrigo Frutuoso - 61865
 * Simão Alexandre - 61874
 * Tiago Leite - 61863
*/

#include "../inc/csignal.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static struct info_container* global_info = NULL;
static struct buffers* global_buffs = NULL;

void sigint_handler(int signum) {
    if (global_info && global_buffs) {
        printf("\n[Main] Recebido SIGINT (CTRL+C). Encerrando SOchain...\n");
        end_execution(global_info, global_buffs);
        exit(0);
    }
}

void setup_periodic_alarm(int period, struct info_container *info) {
    // Implementação da configuração do alarme periódico
}

void setup_sigint_handler(struct info_container *info) {
    // Implementação da configuração do handler para SIGINT (CTRL+C)
}

void ignore_signals_in_child() {
    signal(SIGINT, SIG_IGN);
}

void setup_signal_handler(struct info_container* info, struct buffers* buffs) {
    global_info = info;
    global_buffs = buffs;
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
}