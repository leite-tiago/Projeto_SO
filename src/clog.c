/**
 * Grupo 40
 * Rodrigo Frutuoso - 61865
 * Simão Alexandre - 61874
 * Tiago Leite - 61863
*/

#include "../inc/clog.h"

void write_log(const char* filename, const char* operation) {
    FILE* f = fopen(filename, "a");
    if (!f) return;

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    struct tm* tm_info = localtime(&ts.tv_sec);

    char time_str[32];
    strftime(time_str, sizeof(time_str), "%Y%m%d %H:%M:%S", tm_info);

    fprintf(f, "%s.%03ld %s\n", time_str, ts.tv_nsec / 1000000, operation);
    fclose(f);
}
