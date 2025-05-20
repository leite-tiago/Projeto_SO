/**
 * Grupo 40
 * Rodrigo Frutuoso - 61865
 * Simão Alexandre - 61874
 * Tiago Leite - 61863
*/

#include "../inc/server.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "../inc/ctime.h"

int current_server_id;

int execute_server(int server_id, struct info_container* info, struct buffers* buffs) {
    int processed_transactions = 0;
    struct transaction tx;

    // Armazena o ID do servidor na variável global para uso em outras funções
    current_server_id = server_id;

    while (1) {
        if (*info->terminate == 1) {
            break;
        }

        tx.id = -1;
        server_receive_transaction(&tx, info, buffs);

        if (tx.id == -1) {
            usleep(1000);
            continue;
        }

        server_process_transaction(&tx, server_id, info);

        if (tx.server_signature == server_id) {
            server_send_transaction(&tx, info, buffs);
            processed_transactions++;
        }
    }
    return processed_transactions;
}

void server_receive_transaction(struct transaction* tx, struct info_container* info, struct buffers* buffs)
{
    if (!tx || !info || !buffs)
        return;

    read_wallets_servers_buffer(buffs->buff_wallets_servers, info->buffers_size, tx);

    if (tx->id == -1)
        return;

    printf("[Server %d] Li a transação %d do buffer e esta foi processada corretamente!\n", current_server_id, tx->id);
}

void server_process_transaction(struct transaction* tx, int server_id, struct info_container* info) {
    if (tx->wallet_signature != tx->src_id) {
        printf("[Server %d] A transação %d falhou (assinatura inválida)\n",
               server_id, tx->id);
        return;
    }

    if (info->balances[tx->src_id] < tx->amount) {
        printf("[Server %d] A transação %d falhou (saldo insuficiente)\n",
               server_id, tx->id);
        return;
    }

    info->balances[tx->src_id] -= tx->amount;
    info->balances[tx->dest_id] += tx->amount;

    tx->server_signature = server_id;
    info->servers_stats[server_id]++;
    set_timestamp(&info->tx_times[tx->id].server_signed);

    printf("[Server %d] ledger <- [tx.id %d, src_id %d, dest_id %d, amount %.2f]\n",
        server_id, tx->id, tx->src_id, tx->dest_id, tx->amount);
}

void server_send_transaction(struct transaction* tx, struct info_container* info, struct buffers* buffs) {
    if (!tx || !info || !buffs)
        return;
    write_servers_main_buffer(buffs->buff_servers_main, info->buffers_size, tx);
}
