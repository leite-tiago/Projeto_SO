/**
 * Grupo 40
 * Rodrigo Frutuoso - 61865
 * Simão Alexandre - 61874
 * Tiago Leite - 61863
*/

#ifndef CTIME_H_GUARD
#define CTIME_H_GUARD

#include <time.h>

struct timestamps {
    struct timespec created;
    struct timespec wallet_signed;
    struct timespec server_signed;
};

void set_timestamp(struct timespec *ts);

#endif
