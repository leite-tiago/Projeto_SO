/**
 * Grupo 40
 * Rodrigo Frutuoso - 61865
 * Simão Alexandre - 61874
 * Tiago Leite - 61863
*/

#include "../inc/main.h"
#include "../inc/memory.h"
#include "../inc/process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main_args(int argc, char *argv[], struct info_container *info) {
    if (argc != 6) {
        fprintf(stderr, "Usar: ./SOchain <init_balance> <n_wallets> <n_servers> <buff_size> <max_txs>\n");
        help();
        exit(1);
    }

    info->init_balance = atof(argv[1]);
    info->n_wallets = atoi(argv[2]);
    info->n_servers = atoi(argv[3]);
    info->buffers_size = atoi(argv[4]);
    info->max_txs = atoi(argv[5]);

    if (info->init_balance < 0 || info->n_wallets <= 0 || info->n_servers <= 0 || info->buffers_size <= 0 || info->max_txs <= 0) {
        fprintf(stderr, "Erro: Argumentos inválidos.\n");
        exit(1);
    }

    printf("Saldo inicial: %.2f\n", info->init_balance);
    printf("Número de carteiras: %d\n", info->n_wallets);
    printf("Número de servidores: %d\n", info->n_servers);
    printf("Tamanho dos buffers: %d\n", info->buffers_size);
    printf("Número máximo de transações: %d\n", info->max_txs);
}


void create_dynamic_memory_structs(struct info_container* info, struct buffers* buffs) {
    info->balances = allocate_dynamic_memory(info->n_wallets * sizeof(float));
    for (int i = 0; i < info->n_wallets; i++) {
        info->balances[i] = info->init_balance; // Initialize balances
    }
    info->wallets_pids = allocate_dynamic_memory(info->n_wallets * sizeof(int));
    info->wallets_stats = allocate_dynamic_memory(info->n_wallets * sizeof(int));
    info->servers_pids = allocate_dynamic_memory(info->n_servers * sizeof(int));
    info->servers_stats = allocate_dynamic_memory(info->n_servers * sizeof(int));
    info->terminate = allocate_dynamic_memory(sizeof(int));
    *info->terminate = 0;

    buffs->buff_main_wallets = allocate_dynamic_memory(sizeof(struct ra_buffer));
    buffs->buff_wallets_servers = allocate_dynamic_memory(sizeof(struct circ_buffer));
    buffs->buff_servers_main = allocate_dynamic_memory(sizeof(struct ra_buffer));
}

void create_shared_memory_structs(struct info_container* info, struct buffers* buffs) {
    buffs->buff_main_wallets->ptrs = create_shared_memory(ID_SHM_MAIN_WALLETS_PTR, info->buffers_size * sizeof(int));
    buffs->buff_main_wallets->buffer = create_shared_memory(ID_SHM_MAIN_WALLETS_BUFFER, info->buffers_size * sizeof(struct transaction));

    buffs->buff_wallets_servers->ptrs = create_shared_memory(ID_SHM_WALLETS_SERVERS_PTR, sizeof(struct pointers));
    buffs->buff_wallets_servers->buffer = create_shared_memory(ID_SHM_WALLETS_SERVERS_BUFFER, info->buffers_size * sizeof(struct transaction));

    buffs->buff_servers_main->ptrs = create_shared_memory(ID_SHM_SERVERS_MAIN_PTR, info->buffers_size * sizeof(int));
    buffs->buff_servers_main->buffer = create_shared_memory(ID_SHM_SERVERS_MAIN_BUFFER, info->buffers_size * sizeof(struct transaction));

    info->wallets_stats = create_shared_memory(ID_SHM_WALLETS_STATS, info->n_wallets * sizeof(int));
    info->servers_stats = create_shared_memory(ID_SHM_SERVERS_STATS, info->n_servers * sizeof(int));
    info->balances = create_shared_memory(ID_SHM_BALANCES, info->n_wallets * sizeof(float));
    for (int i = 0; i < info->n_wallets; i++) {
        info->balances[i] = info->init_balance; // Initialize balances
    }
    info->terminate = create_shared_memory(ID_SHM_TERMINATE, sizeof(int));
}

void destroy_dynamic_memory_structs(struct info_container* info, struct buffers* buffs) {
    deallocate_dynamic_memory(info->balances);
    deallocate_dynamic_memory(info->wallets_pids);
    deallocate_dynamic_memory(info->wallets_stats);
    deallocate_dynamic_memory(info->servers_pids);
    deallocate_dynamic_memory(info->servers_stats);
    deallocate_dynamic_memory(info->terminate);

    deallocate_dynamic_memory(buffs->buff_main_wallets);
    deallocate_dynamic_memory(buffs->buff_wallets_servers);
    deallocate_dynamic_memory(buffs->buff_servers_main);
}

void destroy_shared_memory_structs(struct info_container* info, struct buffers* buffs) {
    destroy_shared_memory(ID_SHM_MAIN_WALLETS_PTR, buffs->buff_main_wallets->ptrs, info->buffers_size * sizeof(int));
    destroy_shared_memory(ID_SHM_MAIN_WALLETS_BUFFER, buffs->buff_main_wallets->buffer, info->buffers_size * sizeof(struct transaction));

    destroy_shared_memory(ID_SHM_WALLETS_SERVERS_PTR, buffs->buff_wallets_servers->ptrs, sizeof(struct pointers));
    destroy_shared_memory(ID_SHM_WALLETS_SERVERS_BUFFER, buffs->buff_wallets_servers->buffer, info->buffers_size * sizeof(struct transaction));

    destroy_shared_memory(ID_SHM_SERVERS_MAIN_PTR, buffs->buff_servers_main->ptrs, info->buffers_size * sizeof(int));
    destroy_shared_memory(ID_SHM_SERVERS_MAIN_BUFFER, buffs->buff_servers_main->buffer, info->buffers_size * sizeof(struct transaction));

    destroy_shared_memory(ID_SHM_WALLETS_STATS, info->wallets_stats, info->n_wallets * sizeof(int));
    destroy_shared_memory(ID_SHM_SERVERS_STATS, info->servers_stats, info->n_servers * sizeof(int));
    destroy_shared_memory(ID_SHM_BALANCES, info->balances, info->n_wallets * sizeof(float));
    destroy_shared_memory(ID_SHM_TERMINATE, info->terminate, sizeof(int));
}

void create_processes(struct info_container* info, struct buffers* buffs) {
    for (int i = 0; i < info->n_wallets; i++) {
        info->wallets_pids[i] = launch_wallet(i, info, buffs);
    }
    for (int i = 0; i < info->n_servers; i++) {
        info->servers_pids[i] = launch_server(i, info, buffs);
    }
}
void user_interaction(struct info_container* info, struct buffers* buffs) {
    char command[256];
    int tx_counter = 0;

    while (1) {
        printf("> ");
        scanf("%s", command);

        if (strcmp(command, "bal") == 0) {
            print_balance(info);
        } else if (strcmp(command, "trx") == 0) {
            create_transaction(&tx_counter, info, buffs);
        } else if (strcmp(command, "rcp") == 0) {
            receive_receipt(info, buffs);
        } else if (strcmp(command, "stat") == 0) {
            print_stat(tx_counter, info);
        } else if (strcmp(command, "help") == 0) {
            help();
        } else if (strcmp(command, "end") == 0) {
            end_execution(info, buffs);
            break;
        } else {
            printf("Comando inválido. Digite 'help' para ajuda.\n");
        }
    }
}
void write_final_statistics(struct info_container* info) {
    printf("Estatísticas finais:\n");
    for (int i = 0; i < info->n_wallets; i++) {
        printf("Carteira %d assinou %d transações.\n", i, info->wallets_stats[i]);
    }
    for (int i = 0; i < info->n_servers; i++) {
        printf("Servidor %d processou %d transações.\n", i, info->servers_stats[i]);
    }
}
void end_execution(struct info_container* info, struct buffers* buffs) {
    *info->terminate = 1;
    wait_processes(info);
    write_final_statistics(info);
}

void wait_processes(struct info_container* info) {
    for (int i = 0; i < info->n_wallets; i++) {
        wait_process(info->wallets_pids[i]);
    }
    for (int i = 0; i < info->n_servers; i++) {
        wait_process(info->servers_pids[i]);
    }
}
void print_balance(struct info_container* info) {
    int wallet_id;
    scanf("%d", &wallet_id);

    if (wallet_id < 0 || wallet_id >= info->n_wallets) {
        printf("Erro: ID da carteira inválido.\n");
        return;
    }

    printf("Saldo da carteira %d: %.2f SOT\n", wallet_id, info->balances[wallet_id]);
}

void create_transaction(int* tx_counter, struct info_container* info, struct buffers* buffs) {
    if (*tx_counter >= info->max_txs) {
        printf("Erro: Número máximo de transações atingido.\n");
        return;
    }

    struct transaction tx;
    scanf("%d %d %f", &tx.src_id, &tx.dest_id, &tx.amount);

    if (tx.src_id < 0 || tx.src_id >= info->n_wallets || tx.dest_id < 0 || tx.dest_id >= info->n_wallets || tx.amount <= 0) {
        printf("Erro: Dados da transação inválidos.\n");
        return;
    }

    tx.id = (*tx_counter)++;
    tx.wallet_signature = -1;
    tx.server_signature = -1;

    // Incrementar o número de transações assinadas pela carteira de origem
    info->wallets_stats[tx.src_id]++;

    write_main_wallets_buffer(buffs->buff_main_wallets, info->buffers_size, &tx);
    printf("Transação %d criada.\n", tx.id);
}

void receive_receipt(struct info_container* info, struct buffers* buffs) {
    int tx_id;
    scanf("%d", &tx_id);

    struct transaction tx;
    read_servers_main_buffer(buffs->buff_servers_main, tx_id, info->buffers_size, &tx);

    if (tx.id == -1) {
        printf("Erro: Recibo não encontrado.\n");
        return;
    }

    // Atualizar o saldo das carteiras
    info->balances[tx.src_id] -= tx.amount;
    info->balances[tx.dest_id] += tx.amount;

    // Incrementar o número de transações processadas pelo servidor
    info->servers_stats[tx.server_signature]++;

    printf("Recibo da transação %d:\n", tx.id);
    printf("  Origem: %d\n", tx.src_id);
    printf("  Destino: %d\n", tx.dest_id);
    printf("  Quantia: %.2f SOT\n", tx.amount);
    printf("  Assinatura da carteira: %d\n", tx.wallet_signature);
    printf("  Assinatura do servidor: %d\n", tx.server_signature);
}

void print_stat(int tx_counter, struct info_container* info) {
    printf("Estado atual do sistema:\n");
    printf("  Transações criadas: %d\n", tx_counter);
    printf("  Terminate: %d\n", *info->terminate);

    printf("\nCarteiras:\n");
    for (int i = 0; i < info->n_wallets; i++) {
        printf("  Carteira %d:\n", i);
        printf("    PID: %d\n", info->wallets_pids[i]);
        printf("    Saldo: %.2f\n", info->balances[i]);
        printf("    Transações assinadas: %d\n", info->wallets_stats[i]);
    }

    printf("\nServidores:\n");
    for (int i = 0; i < info->n_servers; i++) {
        printf("  Servidor %d:\n", i);
        printf("    PID: %d\n", info->servers_pids[i]);
        printf("    Transações processadas: %d\n", info->servers_stats[i]);
    }
}

void help() {
    printf("Uso: ./SOchain init_balance n_wallets n_servers buff_size max_txs\n");
    printf("Comandos disponíveis:\n");
    printf("  bal <id> - Mostra o saldo da carteira <id>\n");
    printf("  trx <src_id> <dest_id> <amount> - Cria uma transação\n");
    printf("  rcp <id> - Obtém o comprovativo de uma transação\n");
    printf("  stat - Mostra o estado atual do sistema\n");
    printf("  help - Mostra este menu de ajuda\n");
    printf("  sair - Encerra o programa\n");
}

int main(int argc, char *argv[]) {
    //init data structures
    struct info_container* info = allocate_dynamic_memory(sizeof(struct info_container));
    struct buffers* buffs = allocate_dynamic_memory(sizeof(struct buffers));
    //execute main code
    main_args(argc, argv, info);
    create_dynamic_memory_structs(info, buffs);
    create_shared_memory_structs(info, buffs);
    create_processes(info, buffs);
    user_interaction(info, buffs);
    //release memory before terminating
    destroy_shared_memory_structs(info, buffs);
    destroy_dynamic_memory_structs(info, buffs);
    return 0;
}
