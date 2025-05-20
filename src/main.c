/**
 * Grupo 40
 * Rodrigo Frutuoso - 61865
 * Simão Alexandre - 61874
 * Tiago Leite - 61863
*/

#include "../inc/main.h"
#include "../inc/memory.h"
#include "../inc/process.h"
#include "../inc/csettings.h"
#include "../inc/clog.h"
#include "../inc/ctime.h"
#include "../inc/csignal.h"
#include "../inc/cstats.h"
#include "../inc/synchronization.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

#define MAX_TXS 1000

struct timestamps tx_times[MAX_TXS];

void wakeup_processes(struct info_container* info){
    // Acorda todos os processos bloqueados nos semáforos dos buffers
    int i;
    if (!info || !info->sems) return;

    // Main <-> Wallet
    for (i = 0; i < info->buffers_size; i++) {
        sem_post(info->sems->main_wallet->unread);
        sem_post(info->sems->main_wallet->free_space);
    }
    // Wallet <-> Server
    for (i = 0; i < info->buffers_size; i++) {
        sem_post(info->sems->wallet_server->unread);
        sem_post(info->sems->wallet_server->free_space);
    }
    // Server <-> Main
    for (i = 0; i < info->buffers_size; i++) {
        sem_post(info->sems->server_main->unread);
        sem_post(info->sems->server_main->free_space);
    }
    // Mutex para terminação (caso alguém esteja à espera)
    sem_post(info->sems->terminate_mutex);
}

void main_args(int argc, char *argv[], struct info_container *info) {
    // Lê argumentos de ficheiro args.txt
    if (argc < 3) {
        fprintf(stderr, "[Main] Uso: ./SOchain args.txt settings.txt\n");
        exit(1);
    }
    read_args_file(argv[1], info);

    if (info->init_balance < 0 || info->n_wallets <= 0 || info->n_servers <= 0 || info->buffers_size <= 0 || info->max_txs <= 0) {
        fprintf(stderr, "[Main] Erro: Argumentos inválidos.\n");
        exit(1);
    }

    printf("[Main] Parâmetros corretos!\n\n");
    // printf("[Main] Saldo inicial: %.2f\n", info->init_balance);
    // printf("[Main] Número de carteiras: %d\n", info->n_wallets);
    // printf("[Main] Número de servidores: %d\n", info->n_servers);
    // printf("[Main] Tamanho dos buffers: %d\n", info->buffers_size);
    // printf("[Main] Número máximo de transações: %d\n\n", info->max_txs);
}

void create_dynamic_memory_structs(struct info_container* info, struct buffers* buffs) {
    // Inicializar os saldos
    info->balances = allocate_dynamic_memory(info->n_wallets * sizeof(float));
    for (int i = 0; i < info->n_wallets; i++) {
        info->balances[i] = info->init_balance;
    }

    // Inicializar com 0 os elementos de info
    info->wallets_pids = allocate_dynamic_memory(info->n_wallets * sizeof(int));
    memset(info->wallets_pids, 0, info->n_wallets * sizeof(int));

    info->wallets_stats = allocate_dynamic_memory(info->n_wallets * sizeof(int));
    memset(info->wallets_stats, 0, info->n_wallets * sizeof(int));

    info->servers_pids = allocate_dynamic_memory(info->n_servers * sizeof(int));
    memset(info->servers_pids, 0, info->n_servers * sizeof(int));

    info->servers_stats = allocate_dynamic_memory(info->n_servers * sizeof(int));
    memset(info->servers_stats, 0, info->n_servers * sizeof(int));

    info->terminate = allocate_dynamic_memory(sizeof(int));
    *info->terminate = 0;

    // Inicializar com 0 os elementos de buffs
    buffs->buff_main_wallets = allocate_dynamic_memory(sizeof(struct ra_buffer));
    buffs->buff_main_wallets->ptrs = NULL;
    buffs->buff_main_wallets->buffer = NULL;

    buffs->buff_wallets_servers = allocate_dynamic_memory(sizeof(struct circ_buffer));
    buffs->buff_wallets_servers->ptrs = NULL;
    buffs->buff_wallets_servers->buffer = NULL;

    buffs->buff_servers_main = allocate_dynamic_memory(sizeof(struct ra_buffer));
    buffs->buff_servers_main->ptrs = NULL;
    buffs->buff_servers_main->buffer = NULL;
}

void create_shared_memory_structs(struct info_container* info, struct buffers* buffs) {
    // Inicializar com 0 buffer main_wallets
    buffs->buff_main_wallets->ptrs = create_shared_memory(ID_SHM_MAIN_WALLETS_PTR, info->buffers_size * sizeof(int));
    memset(buffs->buff_main_wallets->ptrs, 0, info->buffers_size * sizeof(int));

    buffs->buff_main_wallets->buffer = create_shared_memory(ID_SHM_MAIN_WALLETS_BUFFER, info->buffers_size * sizeof(struct transaction));
    memset(buffs->buff_main_wallets->buffer, 0, info->buffers_size * sizeof(struct transaction));

    // Inicializar com 0 buffer wallets_servers
    buffs->buff_wallets_servers->ptrs = create_shared_memory(ID_SHM_WALLETS_SERVERS_PTR, sizeof(struct pointers));
    memset(buffs->buff_wallets_servers->ptrs, 0, sizeof(struct pointers));

    buffs->buff_wallets_servers->buffer = create_shared_memory(ID_SHM_WALLETS_SERVERS_BUFFER, info->buffers_size * sizeof(struct transaction));
    memset(buffs->buff_wallets_servers->buffer, 0, info->buffers_size * sizeof(struct transaction));

    // Inicializar com 0 buffer servers_main
    buffs->buff_servers_main->ptrs = create_shared_memory(ID_SHM_SERVERS_MAIN_PTR, info->buffers_size * sizeof(int));
    memset(buffs->buff_servers_main->ptrs, 0, info->buffers_size * sizeof(int));

    buffs->buff_servers_main->buffer = create_shared_memory(ID_SHM_SERVERS_MAIN_BUFFER, info->buffers_size * sizeof(struct transaction));
    memset(buffs->buff_servers_main->buffer, 0, info->buffers_size * sizeof(struct transaction));

    // Inicializar com 0 estatísticas e outros dados
    info->wallets_stats = create_shared_memory(ID_SHM_WALLETS_STATS, info->n_wallets * sizeof(int));
    memset(info->wallets_stats, 0, info->n_wallets * sizeof(int));

    info->servers_stats = create_shared_memory(ID_SHM_SERVERS_STATS, info->n_servers * sizeof(int));
    memset(info->servers_stats, 0, info->n_servers * sizeof(int));

    // Inicializar os saldos
    info->balances = create_shared_memory(ID_SHM_BALANCES, info->n_wallets * sizeof(float));
    for (int i = 0; i < info->n_wallets; i++) {
        info->balances[i] = info->init_balance;
    }

    info->terminate = create_shared_memory(ID_SHM_TERMINATE, sizeof(int));
    *info->terminate = 0;

    info->tx_times = create_shared_memory(ID_SHM_TX_TIMES, info->max_txs * sizeof(struct timestamps));
    memset(info->tx_times, 0, info->max_txs * sizeof(struct timestamps));
}

void destroy_dynamic_memory_structs(struct info_container* info, struct buffers* buffs) {
    // Confirma se a memória foi alocada antes de liberar e define como NULL
    if (info && info->balances) {
        deallocate_dynamic_memory(info->balances);
        info->balances = NULL;
    }
    if (info && info->wallets_pids) {
        deallocate_dynamic_memory(info->wallets_pids);
        info->wallets_pids = NULL;
    }
    if (info && info->wallets_stats) {
        deallocate_dynamic_memory(info->wallets_stats);
        info->wallets_stats = NULL;
    }
    if (info && info->servers_pids) {
        deallocate_dynamic_memory(info->servers_pids);
        info->servers_pids = NULL;
    }
    if (info && info->servers_stats) {
        deallocate_dynamic_memory(info->servers_stats);
        info->servers_stats = NULL;
    }
    if (info && info->terminate) {
        deallocate_dynamic_memory(info->terminate);
        info->terminate = NULL;
    }

    if (buffs && buffs->buff_main_wallets) {
        deallocate_dynamic_memory(buffs->buff_main_wallets);
        buffs->buff_main_wallets = NULL;
    }
    if (buffs && buffs->buff_wallets_servers) {
        deallocate_dynamic_memory(buffs->buff_wallets_servers);
        buffs->buff_wallets_servers = NULL;
    }
    if (buffs && buffs->buff_servers_main) {
        deallocate_dynamic_memory(buffs->buff_servers_main);
        buffs->buff_servers_main = NULL;
    }
}

void destroy_shared_memory_structs(struct info_container* info, struct buffers* buffs) {
    // Confirma se a memória foi alocada antes de liberar e define como NULL
    if (buffs && buffs->buff_main_wallets && buffs->buff_main_wallets->ptrs) {
        destroy_shared_memory(ID_SHM_MAIN_WALLETS_PTR, buffs->buff_main_wallets->ptrs, info->buffers_size * sizeof(int));
        buffs->buff_main_wallets->ptrs = NULL;
    }
    if (buffs && buffs->buff_main_wallets && buffs->buff_main_wallets->buffer) {
        destroy_shared_memory(ID_SHM_MAIN_WALLETS_BUFFER, buffs->buff_main_wallets->buffer, info->buffers_size * sizeof(struct transaction));
        buffs->buff_main_wallets->buffer = NULL;
    }

    if (buffs && buffs->buff_wallets_servers && buffs->buff_wallets_servers->ptrs) {
        destroy_shared_memory(ID_SHM_WALLETS_SERVERS_PTR, buffs->buff_wallets_servers->ptrs, sizeof(struct pointers));
        buffs->buff_wallets_servers->ptrs = NULL;
    }
    if (buffs && buffs->buff_wallets_servers && buffs->buff_wallets_servers->buffer) {
        destroy_shared_memory(ID_SHM_WALLETS_SERVERS_BUFFER, buffs->buff_wallets_servers->buffer, info->buffers_size * sizeof(struct transaction));
        buffs->buff_wallets_servers->buffer = NULL;
    }

    if (buffs && buffs->buff_servers_main && buffs->buff_servers_main->ptrs) {
        destroy_shared_memory(ID_SHM_SERVERS_MAIN_PTR, buffs->buff_servers_main->ptrs, info->buffers_size * sizeof(int));
        buffs->buff_servers_main->ptrs = NULL;
    }
    if (buffs && buffs->buff_servers_main && buffs->buff_servers_main->buffer) {
        destroy_shared_memory(ID_SHM_SERVERS_MAIN_BUFFER, buffs->buff_servers_main->buffer, info->buffers_size * sizeof(struct transaction));
        buffs->buff_servers_main->buffer = NULL;
    }

    if (info && info->wallets_stats) {
        destroy_shared_memory(ID_SHM_WALLETS_STATS, info->wallets_stats, info->n_wallets * sizeof(int));
        info->wallets_stats = NULL;
    }
    if (info && info->servers_stats) {
        destroy_shared_memory(ID_SHM_SERVERS_STATS, info->servers_stats, info->n_servers * sizeof(int));
        info->servers_stats = NULL;
    }
    if (info && info->balances) {
        destroy_shared_memory(ID_SHM_BALANCES, info->balances, info->n_wallets * sizeof(float));
        info->balances = NULL;
    }
    if (info && info->terminate) {
        destroy_shared_memory(ID_SHM_TERMINATE, info->terminate, sizeof(int));
        info->terminate = NULL;
    }
    if (info && info->tx_times) {
        destroy_shared_memory(ID_SHM_TX_TIMES, info->tx_times, info->max_txs * sizeof(struct timestamps));
        info->tx_times = NULL;
    }
}

void create_processes(struct info_container* info, struct buffers* buffs) {
    for (int i = 0; i < info->n_wallets; i++) {
        info->wallets_pids[i] = launch_wallet(i, info, buffs);
        if (info->wallets_pids[i] <= 0) {
            perror("Erro ao criar processo de carteira");
            exit(1);
        }
    }

    for (int i = 0; i < info->n_servers; i++) {
        info->servers_pids[i] = launch_server(i, info, buffs);
        if (info->servers_pids[i] <= 0) {
            perror("Erro ao criar processo de servidor");
            exit(1);
        }
    }
}

extern volatile sig_atomic_t sigint_received; // declara a flag como externa

void user_interaction(struct info_container* info, struct buffers* buffs) {
    char command[256];
    int tx_counter = 0;

    while (1) {
        if (sigint_received) {
            end_execution(info, buffs);
            break;
        }
        printf("\n[Main] Introduzir operação: ");
        scanf("%s", command);

        if (strcmp(command, "bal") == 0) {
            print_balance(info);
            write_log(get_log_filename(), "bal");
        } else if (strcmp(command, "trx") == 0) {
            create_transaction(&tx_counter, info, buffs);
            write_log(get_log_filename(), "trx");
        } else if (strcmp(command, "rcp") == 0) {
            receive_receipt(info, buffs);
            write_log(get_log_filename(), "rcp");
        } else if (strcmp(command, "stat") == 0) {
            print_stat(tx_counter, info);
            write_log(get_log_filename(), "stat");
        } else if (strcmp(command, "help") == 0) {
            help();
            write_log(get_log_filename(), "help");
        } else if (strcmp(command, "end") == 0) {
            end_execution(info, buffs);
            write_log(get_log_filename(), "end");
            break;
        } else {
            printf("[Main] Comando inválido. Digite 'help' para ajuda.\n");
        }
        usleep(3000);
    }
}

void write_final_statistics(struct info_container* info) {
    printf("[Main] A encerrar a execução do SOchain! As estatísticas da execução são:\n");
    for (int i = 0; i < info->n_wallets; i++) {
        printf("[Main] A carteira %d assinou %d transações e terminou com %.2f SOT!\n",
               i, info->wallets_stats[i], info->balances[i]);
    }
    for (int i = 0; i < info->n_servers; i++) {
        printf("[Main] O servidor %d assinou %d transações!\n", i, info->servers_stats[i]);
    }
}

void end_execution(struct info_container* info, struct buffers* buffs) {
    *info->terminate = 1;
    wakeup_processes(info);

    for (int i = 0; i < info->n_wallets; i++) {
        wait_process(info->wallets_pids[i]);
    }
    for (int i = 0; i < info->n_servers; i++) {
        wait_process(info->servers_pids[i]);
    }

    write_final_statistics(info);

    destroy_shared_memory_structs(info, buffs);
    destroy_all_semaphores(info->sems);
    destroy_dynamic_memory_structs(info, buffs);
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
        printf("[Main] Erro: ID da carteira inválido.\n");
        return;
    }

    printf("[Main] O saldo da carteira %d é de %.2f SOT atualmente.\n", wallet_id, info->balances[wallet_id]);
}

void create_transaction(int* tx_counter, struct info_container* info, struct buffers* buffs) {
    if (*tx_counter >= info->max_txs) {
        int src_id, dest_id;
        float amount;
        scanf("%d %d %f", &src_id, &dest_id, &amount);

        printf("[Main] O número máximo de transações foi alcançado!\n");
        return;
    }

    struct transaction tx = {0};
    scanf("%d %d %f", &tx.src_id, &tx.dest_id, &tx.amount);

    if (tx.src_id < 0 || tx.src_id >= info->n_wallets ||
        tx.dest_id < 0 || tx.dest_id >= info->n_wallets ||
        tx.amount <= 0 || tx.src_id == tx.dest_id) {
        printf("[Main] Erro: Dados da transação inválidos. Origem: %d, Destino: %d, Valor: %.2f\n",
               tx.src_id, tx.dest_id, tx.amount);
        return;
    }

    tx.id = (*tx_counter)++;
    tx.wallet_signature = -1;
    tx.server_signature = -1;

    set_timestamp(&info->tx_times[tx.id].created);

    printf("[Main] A transação %d foi criada para transferir %.2f SOT da carteira %d para a carteira %d!\n",
           tx.id, tx.amount, tx.src_id, tx.dest_id);

    write_main_wallets_buffer(buffs->buff_main_wallets, info->buffers_size, &tx);
}

void receive_receipt(struct info_container* info, struct buffers* buffs) {
    int tx_id;
    scanf("%d", &tx_id);

    struct transaction tx;
    read_servers_main_buffer(buffs->buff_servers_main, tx_id, info->buffers_size, &tx);


    if (tx.id == -1) {
        printf("[Main] O comprovativo da execução da transação %d não está disponível.\n", tx.id);
        return;
    }

    printf("[Main] O comprovativo da execução da transação %d foi obtido.\n", tx.id);
    printf("[Main] O comprovativo da transação id %d contém src_id %d, dest_id %d, amount %.2f e foi assinado pela carteira %d e servidor %d.\n",
           tx.id, tx.src_id, tx.dest_id, tx.amount, tx.wallet_signature, tx.server_signature);
    //QUANDO TESTARMOS USAMOS O
    // print_timestamps(tx.id);
}

void print_stat(int tx_counter, struct info_container* info) {
    printf("- Configuração inicial:\n");
    printf("        Propriedade     Valor\n");
    printf("        init_balance    %.2f\n", info->init_balance);
    printf("        n_wallets       %d\n", info->n_wallets);
    printf("        n_servers       %d\n", info->n_servers);
    printf("        buffers_size    %d\n", info->buffers_size);
    printf("        max_txs         %d\n", info->max_txs);

    printf("- Variáveis atuais:\n");
    printf("        terminate       %d\n", *info->terminate);
    printf("        tx_count        %d\n", tx_counter);

    printf("- Informações sobre as carteiras:\n");
    printf("        Carteira        PID             Saldo           Transações Assinadas\n");
    for (int i = 0; i < info->n_wallets; i++) {
        char saldo_str[20];
        snprintf(saldo_str, sizeof(saldo_str), "%.2f SOT", info->balances[i]);

        printf("        %-12d    %-12d    %-13s   %d\n",
            i,
            info->wallets_pids[i],
               saldo_str,
            info->wallets_stats[i]
        );
    }

    printf("- Informações sobre os servidores:\n");
    printf("        Servidor        PID             Transações Processadas\n");
    for (int i = 0; i < info->n_servers; i++) {
        printf("        %-15d %-15d %-12d\n",
            i,
            info->servers_pids[i],
            info->servers_stats[i]
        );
    }
}

void help() {
    printf("[Main] Operações disponíveis:\n");
    printf("[Main]  bal id - consultar o saldo da carteira identificada por id.\n");
    printf("[Main]  trx src_id dest_id amount - criar uma nova transação.\n");
    printf("[Main]  rcp id - obter o comprovativo da transação de número id.\n");
    printf("[Main]  help - imprime a informação sobre as operações disponíveis.\n");
    printf("[Main]  end - termina a execução do SOchain.\n");
}

//FUNÇÃO PARA DEBUG, AINDA NÃO TESTEI NADAAAAAAA
void print_timestamps(int tx_id) {
    printf("Tempos da transação %d:\n", tx_id);
    printf("  Criada: %ld.%09ld\n", tx_times[tx_id].created.tv_sec, tx_times[tx_id].created.tv_nsec);
    printf("  Wallet: %ld.%09ld\n", tx_times[tx_id].wallet_signed.tv_sec, tx_times[tx_id].wallet_signed.tv_nsec);
    printf("  Server: %ld.%09ld\n", tx_times[tx_id].server_signed.tv_sec, tx_times[tx_id].server_signed.tv_nsec);
}

int main(int argc, char *argv[]) {
    struct info_container* info = allocate_dynamic_memory(sizeof(struct info_container));
    struct buffers* buffs = allocate_dynamic_memory(sizeof(struct buffers));

    main_args(argc, argv, info);

    read_settings_file(argv[2]);

    create_dynamic_memory_structs(info, buffs);
    create_shared_memory_structs(info, buffs);

    struct semaphores* sems = create_all_semaphores(info->buffers_size);
    info->sems = sems;

    setup_sigint_handler(info);

    setup_periodic_alarm(get_period(), info);

    create_processes(info, buffs);
    user_interaction(info, buffs);

    destroy_shared_memory_structs(info, buffs);
    destroy_all_semaphores(info->sems);
    destroy_dynamic_memory_structs(info, buffs);
    return 0;
}
