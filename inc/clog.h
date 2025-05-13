#ifndef CLOG_H_GUARD
#define CLOG_H_GUARD

#include <time.h>

/* Escreve uma entrada no ficheiro de log com timestamp e operação */
void write_log(const char *filename, const char *operation);

#endif