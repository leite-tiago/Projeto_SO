#ifndef CSIGNAL_H_GUARD
#define CSIGNAL_H_GUARD

#include "main.h"

/* Inicializa o handler do alarme periódico */
void setup_periodic_alarm(int period, struct info_container *info);

/* Inicializa o handler para SIGINT (CTRL+C) */
void setup_sigint_handler(struct info_container *info);

#endif