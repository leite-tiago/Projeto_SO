#include "../inc/cstats.h"
#include <stdio.h>
#include <unistd.h>

void write_statistics_file(const char *filename, struct info_container *info, int tx_counter, int receipts_read) {
    if (!filename || !info) return;

    FILE *f = fopen(filename, "w");
    if (!f) {
        perror("Erro ao abrir ficheiro de estatísticas");
        return;
    }

    // Process Statistics
    fprintf(f, "Process Statistics:\n");
    fprintf(f, "Main has PID [%d]\n", getpid());

    // Wallets
    fprintf(f, "There were %d Wallets, PIDs [", info->n_wallets);
    for (int i = 0; i < info->n_wallets; i++) {
        fprintf(f, "%d", info->wallets_pids[i]);
        if (i < info->n_wallets - 1) fprintf(f, ",");
    }
    fprintf(f, "]\n");

    // Servers
    fprintf(f, "There were %d Servers, PIDs [", info->n_servers);
    for (int i = 0; i < info->n_servers; i++) {
        fprintf(f, "%d", info->servers_pids[i]);
        if (i < info->n_servers - 1) fprintf(f, ",");
    }
    fprintf(f, "]\n");

    // Main transactions
    fprintf(f, "Main received %d transaction(s)!\n", tx_counter);

    // Wallet stats
    for (int i = 0; i < info->n_wallets; i++) {
        fprintf(f, "Wallet #%d signed %d transaction(s)!\n", i, info->wallets_stats[i]);
    }

    // Server stats
    for (int i = 0; i < info->n_servers; i++) {
        fprintf(f, "Server #%d processed %d transaction(s)!\n", i, info->servers_stats[i]);
    }

    // Receipts
    fprintf(f, "Main read %d receipts.\n", receipts_read);

    // Final balances
    fprintf(f, "Final Balances [");
    for (int i = 0; i < info->n_wallets; i++) {
        fprintf(f, "%.2f", info->balances[i]);
        if (i < info->n_wallets - 1) fprintf(f, ",");
    }
    fprintf(f, "] SOT\n");

    fclose(f);
}


