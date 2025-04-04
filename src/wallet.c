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

        // Use wallet_receive_transaction instead of directly reading from buffer
        wallet_receive_transaction(&tx, wallet_id, info, buffs);

        if (tx.id == -1) {
            usleep(1000); // Aguarda um curto período antes de verificar novamente
            continue;
        }

        wallet_process_transaction(&tx, wallet_id, info); // Assina a transação
        signed_transactions++;
        wallet_send_transaction(&tx, info, buffs); // Envia a transação assinada
    }

    return signed_transactions;
}

void wallet_receive_transaction(struct transaction* tx, int wallet_id, struct info_container* info, struct buffers* buffs) {
    if (*info->terminate == 1) {
        return;
    }

    // The correct way is to look for transactions where this wallet is the source
    read_main_wallets_buffer(buffs->buff_main_wallets, wallet_id, info->buffers_size, tx);
    
    if (tx->id == -1) {
        // No transaction found, don't print anything to avoid excessive messages
        usleep(1000);
        return;
    }

    // Only proceed if this wallet is the source of the transaction
    if (tx->src_id != wallet_id) {
        tx->id = -1; // Mark as invalid
        return;
    }

    printf("Wallet %d recebeu transação %d: origem %d, destino %d, valor %.2f\n", 
           wallet_id, tx->id, tx->src_id, tx->dest_id, tx->amount);
}

void wallet_process_transaction(struct transaction* tx, int wallet_id, struct info_container* info){
    // Only sign if this wallet is the source of the transaction
    if (tx->src_id == wallet_id) {
        tx->wallet_signature = wallet_id;
        info->wallets_stats[wallet_id]++;
        printf("Wallet %d assinou transação %d\n", wallet_id, tx->id);
    }
}
void wallet_send_transaction(struct transaction* tx, struct info_container* info, struct buffers* buffs) {
    if (!tx || !info || !buffs) {
        perror("Error with attributes on wallet_send_transaction");
        exit(1);
    }

    printf("Wallet %d enviando transação %d para o buffer wallets_servers\n", tx->src_id, tx->id);
    write_wallets_servers_buffer(buffs->buff_wallets_servers, info->buffers_size, tx);
}