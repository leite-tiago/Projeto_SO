/**
 * Grupo 40
 * Rodrigo Frutuoso - 61865
 * Simão Alexandre - 61874
 * Tiago Leite - 61863
*/

#include "../inc/wallet.h"
#include <stdio.h>  // printf(), perror()
#include <stdlib.h> // exit(), malloc(), free()
#include <unistd.h> // fork(), execl(), getpid(), getppid()
#include <string.h>
#include <time.h>

int execute_wallet(int wallet_id, struct info_container* info, struct buffers* buffs) {
    int signed_transactions = 0;
    struct transaction tx;

    while (1) {
        if (*info->terminate == 1) {
            break;
        }

        wallet_receive_transaction(&tx, wallet_id, info, buffs);

        if (tx.id == -1) {
            usleep(1000); // Aguarda um curto período antes de verificar novamente
            continue;
        }

        wallet_process_transaction(&tx, wallet_id, info); // Processa a transação
        signed_transactions++;
        wallet_send_transaction(&tx, info, buffs); // Envia a transação assinada
    }

    return signed_transactions;
}

void wallet_receive_transaction(struct transaction* tx, int wallet_id, struct info_container* info, struct buffers* buffs) {
    if (*info->terminate == 1) {
        return;
    }

    read_main_wallets_buffer(buffs->buff_main_wallets, wallet_id, info->buffers_size, tx);

    if (tx->id == -1) {
        usleep(1000); // Nenhuma transação encontrada
        return;
    }

    if (tx->src_id != wallet_id) {
        tx->id = -1; // Transação não pertence a esta carteira
        return;
    }

    printf("[Wallet %d] Li a transação %d do buffer e a assinei!\n", wallet_id, tx->id);
}

void wallet_process_transaction(struct transaction* tx, int wallet_id, struct info_container* info) {
    if (tx->src_id == wallet_id) {
        tx->wallet_signature = wallet_id;
        info->wallets_stats[wallet_id]++;
    }
    //printf("Wallet %d assinou transação %d\n", wallet_id, tx->id);

}
void wallet_send_transaction(struct transaction* tx, struct info_container* info, struct buffers* buffs) {
    if (!tx || !info || !buffs) {
        exit(1);
    }

    //printf("[Wallet %d] Transação %d assinada e enviada para o buffer wallets_servers.\n", tx->src_id, tx->id);
    write_wallets_servers_buffer(buffs->buff_wallets_servers, info->buffers_size, tx);
}