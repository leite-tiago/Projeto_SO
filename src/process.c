/**
 * Grupo 40
 * Rodrigo Frutuoso - 61865
 * Simão Alexandre - 61874
 * Tiago Leite - 61863
*/

#include "../inc/process.h"
#include "../inc/wallet.h"
#include "../inc/server.h"
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int launch_wallet(int wallet_id, struct info_container* info, struct buffers* buffs){
    int pid = fork();

    if (pid == -1) {
        perror("Error with fork in launch_wallet");
        exit(1);
    }
    if (pid == 0) {
        //printf("Wallet %d created with PID %d\n", wallet_id, getpid());
        exit(execute_wallet(wallet_id, info, buffs));
    }
    return pid;
}

int launch_server(int server_id, struct info_container* info, struct buffers* buffs) {
    int pid = fork();

    if (pid == -1) {
        perror("Erro ao criar processo do servidor");
        exit(1);
    }

    // Processo filho
    if (pid == 0) {
        //printf("Servidor %d iniciado com PID %d\n", server_id, getpid());
        int processed_transactions = execute_server(server_id, info, buffs);
        //printf("Servidor %d processou %d transações\n", server_id, processed_transactions);
        exit(0);
    }

    // Processo pai - retorna o PID do servidor
    return pid;
}

int wait_process(int process_id) {
    int status;
    int pid = waitpid(process_id, &status, 0);

    if (pid == -1) {
        perror("Error with waitpid");
        exit(1);
    }
    if (WIFEXITED(status)) {
        printf("Process %d terminated with exit code %d\n", pid, WEXITSTATUS(status));
    } else {
        printf("Process %d terminated unexpectedly\n", pid);
    }
    return pid;
}

