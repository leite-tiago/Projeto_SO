/**
 * Grupo 40
 * Rodrigo Frutuoso - 61865
 * Simão Alexandre - 61874
 * Tiago Leite - 61863
*/

#ifndef CSETTINGS_H_GUARD
#define CSETTINGS_H_GUARD

#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_args_file(const char *filename, struct info_container *info);
void read_settings_file(const char *filename);

const char* get_log_filename();
const char* get_statistics_filename();
int get_period();

#endif
