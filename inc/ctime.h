#ifndef CTIME_H_GUARD
#define CTIME_H_GUARD

#include <time.h>

struct timestamps {
    struct timespec created;
    struct timespec wallet_signed;
    struct timespec server_signed;
};

/* Preenche o campo de tempo apropriado na struct timestamps */
void set_timestamp(struct timespec *ts);

#endif

//NOTA: supostamente falta adicionar à struct timestamps o change_time, mas não podemos alterar o memory.h
