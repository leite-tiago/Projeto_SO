/**
 * Grupo 40
 * Rodrigo Frutuoso - 61865
 * Simão Alexandre - 61874
 * Tiago Leite - 61863
*/

#include "../inc/wallet.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "../inc/ctime.h"

int execute_wallet(int wallet_id, struct info_container* info, struct buffers* buffs) {
    int signed_transactions = 0;
    struct transaction tx;

    while (1) {
        if (*info->terminate == 1) {
            break;
        }

        wallet_receive_transaction(&tx, wallet_id, info, buffs);

        if (tx.id == -1) {
            usleep(1000);
            continue;
        }

        wallet_process_transaction(&tx, wallet_id, info);
        signed_transactions++;
        wallet_send_transaction(&tx, info, buffs);
    }

    return signed_transactions;
}

void wallet_receive_transaction(struct transaction* tx, int wallet_id, struct info_container* info, struct buffers* buffs) {
    if (*info->terminate == 1) {
        return;
    }

    read_main_wallets_buffer(buffs->buff_main_wallets, wallet_id, info->buffers_size, tx);

    if (tx->id == -1) {
        usleep(1000);
        return;
    }

    if (tx->src_id != wallet_id) {
        tx->id = -1;
        return;
    }

    printf("[Wallet %d] Li a transação %d do buffer e a assinei!\n", wallet_id, tx->id);
}

void wallet_process_transaction(struct transaction* tx, int wallet_id, struct info_container* info) {
    if (tx->src_id == wallet_id) {
        tx->wallet_signature = wallet_id;
        info->wallets_stats[wallet_id]++;
        set_timestamp(&tx->change_time.wallet_signed);
    }
}
void wallet_send_transaction(struct transaction* tx, struct info_container* info, struct buffers* buffs) {
    if (!tx || !info || !buffs) {
        exit(1);
    }
    write_wallets_servers_buffer(buffs->buff_wallets_servers, info->buffers_size, tx);
}
