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

    if (pid == 0) {
        execute_server(server_id, info, buffs);
        exit(0);
    }

    return pid;
}

int wait_process(int process_id) {
    int status;
    int pid = waitpid(process_id, &status, 0);

    if (pid == -1) {
        perror("Error with waitpid");
        exit(1);
    }

    return pid;
}

