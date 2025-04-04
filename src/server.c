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

int execute_server(int server_id, struct info_container* info, struct buffers* buffs)
{
    struct transaction tx;
    int processed_count = 0;

    while (1)
    {
        if (info->terminate)
            return processed_count;
        server_receive_transaction(&tx, info, buffs);
        if (tx.id == -1)
        {
            usleep(3000);
            continue;
        }
        server_process_transaction(&tx, server_id, info);
        server_send_transaction(&tx, info, buffs);
        processed_count++;
    }
    return processed_count; //número de transações processadas
}

void server_receive_transaction(struct transaction* tx, struct info_container* info, struct buffers* buffs)
{
    if (!tx || !info || !buffs)
    {
        perror("Error with attributes on server_receive_transaction");
        exit(1);
    }

    if (info->terminate)
        return;

    read_wallets_servers_buffer(buffs->buff_wallets_servers, info->buffers_size, tx);
}

void server_process_transaction(struct transaction* tx, int server_id, struct info_container* info)
{
    if (!tx || !info || !info->balances || !info->wallets_stats || !info->servers_stats)
    {
        perror("Error with attributes on server_process_transaction");
        exit(1);
    }
    if (tx->id == -1 || tx->src_id < 0 || tx->src_id >= info->n_wallets ||
        tx->dest_id < 0 || tx->dest_id >= info->n_wallets || tx->src_id == tx->dest_id)
        return;
    if (info->balances[tx->src_id] < tx->amount)
        return;
    if (tx->wallet_signature != info->wallets_stats[tx->src_id])
        return;
    info->balances[tx->src_id] -= tx->amount;
    info->balances[tx->dest_id] += tx->amount;
    tx->server_signature = server_id;
    info->servers_stats[server_id]++;
}

void server_send_transaction(struct transaction* tx, struct info_container* info, struct buffers* buffs)
{
    if (!tx || !info || !buffs || !buffs->buff_servers_main)
    {
        perror("Error with attributes on server_send_transaction");
        exit(1);
    }
    if (tx->server_signature == -1)
        return;
    write_servers_main_buffer(buffs->buff_servers_main, info->buffers_size, tx);
}
