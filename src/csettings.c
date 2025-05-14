#include "../inc/csettings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char log_filename[256];
static char statistics_filename[256];
static int period = 0;

void read_args_file(const char *filename, struct info_container *info) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("Erro ao abrir args.txt");
        exit(1);
    }
    // Espera-se uma linha por argumento, na ordem correta
    if (fscanf(f, "%f", &info->init_balance) != 1 ||
        fscanf(f, "%d", &info->n_wallets) != 1 ||
        fscanf(f, "%d", &info->n_servers) != 1 ||
        fscanf(f, "%d", &info->buffers_size) != 1 ||
        fscanf(f, "%d", &info->max_txs) != 1) {
        fprintf(stderr, "Erro ao ler argumentos de %s\n", filename);
        fclose(f);
        exit(1);
    }
    fclose(f);
}

void read_settings_file(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("Erro ao abrir settings.txt");
        exit(1);
    }
    
    if (!fgets(log_filename, sizeof(log_filename), f)) {
        fprintf(stderr, "Erro ao ler log_filename\n");
        fclose(f);
        exit(1);
    }
    log_filename[strcspn(log_filename, "\r\n")] = 0;

    if (!fgets(statistics_filename, sizeof(statistics_filename), f)) {
        fprintf(stderr, "Erro ao ler statistics_filename\n");
        fclose(f);
        exit(1);
    }
    statistics_filename[strcspn(statistics_filename, "\r\n")] = 0;

    if (fscanf(f, "%d", &period) != 1) {
        fprintf(stderr, "Erro ao ler period\n");
        fclose(f);
        exit(1);
    }
    fclose(f);
}

const char* get_log_filename() {
    return log_filename;
}
const char* get_statistics_filename() {
    return statistics_filename;
}
int get_period() {
    return period;
}
