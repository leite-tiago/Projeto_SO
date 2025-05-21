/**
 * Grupo 40
 * Rodrigo Frutuoso - 61865
 * Simão Alexandre - 61874
 * Tiago Leite - 61863
*/

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

void setup_periodic_alarm(int period, struct info_container *info, struct transaction* transactions, int num_transactions);
void setup_sigint_handler(struct info_container *info);
void alarm_handler(int signo);

#endif
