#include "../inc/ctime.h"
#include <time.h>

void set_timestamp(struct timespec *ts) {
    clock_gettime(CLOCK_REALTIME, ts);
}