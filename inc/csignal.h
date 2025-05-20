#ifndef CSIGNAL_H_GUARD
#define CSIGNAL_H_GUARD

#include "main.h"
#include "ctime.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

/* Inicializa o handler do alarme periódico */
void setup_periodic_alarm(int period, struct info_container *info);

/* Inicializa o handler para SIGINT (CTRL+C) */
void setup_sigint_handler(struct info_container *info);

/* Handler para o alarme */
void alarm_handler(int signo);

#endif
