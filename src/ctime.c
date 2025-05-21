/**
 * Grupo 40
 * Rodrigo Frutuoso - 61865
 * Simão Alexandre - 61874
 * Tiago Leite - 61863
*/

#include "../inc/ctime.h"

void set_timestamp(struct timespec *ts) {
    clock_gettime(CLOCK_REALTIME, ts);
}
