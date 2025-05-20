#include "../inc/ctime.h"

void set_timestamp(struct timespec *ts) {
    clock_gettime(CLOCK_REALTIME, ts);
}
