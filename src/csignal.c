#include "../inc/csignal.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Variável global para sinalizar término (deve ser usada no main loop)
volatile sig_atomic_t sigint_received = 0;

/* Inicializa o handler do alarme periódico */
void setup_periodic_alarm(int period, struct info_container *info);

/* Handler para SIGINT (CTRL+C) */
void sigint_handler(int signo) {
    sigint_received = 1;
    // Opcional: imprime mensagem
    write(STDOUT_FILENO, "\n[Main] SIGINT recebido! A terminar...\n", 39);
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
