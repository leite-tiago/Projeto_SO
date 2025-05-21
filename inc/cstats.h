/**
 * Grupo 40
 * Rodrigo Frutuoso - 61865
 * Simão Alexandre - 61874
 * Tiago Leite - 61863
*/

#ifndef CSTATS_H_GUARD
#define CSTATS_H_GUARD

#include "main.h"
#include <stdio.h>
#include <unistd.h>

void write_statistics_file(const char *filename, struct info_container *info, int tx_counter, int receipts_read);

#endif
