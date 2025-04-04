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

int current_server_id; // Variável global para armazenar o ID do servidor

int execute_server(int server_id, struct info_container* info, struct buffers* buffs) {
    int processed_transactions = 0;
    struct transaction tx;

    printf("Servidor %d: iniciando processamento de transações\n", server_id);

    while (1) {
        if (*info->terminate == 1) {
            printf("Servidor %d: terminando execução\n", server_id);
            break;
        }

        // Lê uma transação do buffer wallets_servers
        read_wallets_servers_buffer(buffs->buff_wallets_servers, info->buffers_size, &tx);

        if (tx.id == -1) {
            // Nenhuma transação disponível, aguarda um pouco
            usleep(1000);
            continue;
        }

        // Processa a transação
        printf("Servidor %d: processando transação %d (origem: %d, destino: %d, valor: %.2f)\n",
               server_id, tx.id, tx.src_id, tx.dest_id, tx.amount);

        // Verifica se a transação está assinada pela carteira de origem
        if (tx.wallet_signature == tx.src_id) {
            // Verifica se há saldo suficiente
            if (info->balances[tx.src_id] < tx.amount) {
                printf("Servidor %d: transação %d rejeitada (saldo insuficiente)\n",
                      server_id, tx.id);
                continue;
            }

            // Atualiza saldos
            info->balances[tx.src_id] -= tx.amount;
            info->balances[tx.dest_id] += tx.amount;
            tx.server_signature = server_id;
            info->servers_stats[server_id]++;
            processed_transactions++;

            // Escreve a transação processada no buffer servers_main
            write_servers_main_buffer(buffs->buff_servers_main, info->buffers_size, &tx);
            printf("Servidor %d: transação %d processada com sucesso\n", server_id, tx.id);
        } else {
            printf("Servidor %d: transação %d rejeitada (assinatura inválida)\n", server_id, tx.id);
        }
    }

    return processed_transactions;
}

void server_receive_transaction(struct transaction* tx, struct info_container* info, struct buffers* buffs)
{
    if (!tx || !info || !buffs)
        return;

    // Ler transação do buffer wallets_servers
    read_wallets_servers_buffer(buffs->buff_wallets_servers, info->buffers_size, tx);

    if (tx->id == -1) // Nenhuma transação disponível
        return;

        printf("Servidor %d recebeu transação %d de %d para %d com valor %.2f\n", current_server_id, tx->id, tx->src_id, tx->dest_id, tx->amount);
}

void server_process_transaction(struct transaction* tx, int server_id, struct info_container* info) {
    // Verify that the wallet signature matches the source wallet
    if (tx->wallet_signature != tx->src_id) {
        printf("Servidor %d: transação %d rejeitada (assinatura inválida)\n",
               server_id, tx->id);
        return;
    }

    // Check if source wallet has sufficient funds
    if (info->balances[tx->src_id] < tx->amount) {
        printf("Servidor %d: transação %d rejeitada (saldo insuficiente)\n",
               server_id, tx->id);
        return;
    }

    // Process the transaction: debit source account, credit destination account
    info->balances[tx->src_id] -= tx->amount;
    info->balances[tx->dest_id] += tx->amount;
    info->servers_stats[server_id]++;

    printf("Servidor %d: transação %d processada com sucesso\n",
           server_id, tx->id);
}

void server_send_transaction(struct transaction* tx, struct info_container* info, struct buffers* buffs)
{
    if (!tx || !info || !buffs)
        return;

    // Escrever transação no buffer servers_main
    write_servers_main_buffer(buffs->buff_servers_main, info->buffers_size, tx);

    printf("Servidor %d enviou transação %d\n", current_server_id, tx->id);
}
