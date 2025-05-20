#ifndef CTIME_H_GUARD
#define CTIME_H_GUARD

#define ID_SHM_TX_TIMES					"SHM_TX_TIMES"

#include <time.h>

struct timestamps {
    struct timespec created;
    struct timespec wallet_signed;
    struct timespec server_signed;
};

/* Preenche o campo de tempo apropriado na struct timestamps */
void set_timestamp(struct timespec *ts);

#endif
