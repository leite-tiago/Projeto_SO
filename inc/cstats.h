#ifndef CSTATS_H_GUARD
#define CSTATS_H_GUARD

#include "main.h"

/* Escreve as estatísticas finais no ficheiro indicado */
void write_statistics_file(const char *filename, struct info_container *info, int tx_counter);

#endif