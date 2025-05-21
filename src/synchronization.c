/**
 * Grupo 40
 * Rodrigo Frutuoso - 61865
 * Simão Alexandre - 61874
 * Tiago Leite - 61863
*/

#include "synchronization.h"
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>

sem_t* create_semaphore(char *name, unsigned v) {
    sem_t *sem = malloc(sizeof(sem_t));
    if (!sem) return NULL;
    if (sem_init(sem, 1, v) != 0) {
        free(sem);
        return NULL;
    }
    return sem;
}

void destroy_semaphore(char *name, sem_t *sem) {
    if (sem) {
        if (sem_destroy(sem) != 0) {
            perror("Erro ao destruir semáforo");
        }
        free(sem);
    }
}

struct semaphores* create_all_semaphores(unsigned v) {
    struct semaphores *sems = malloc(sizeof(struct semaphores));
    if (!sems) return NULL;
    sems->main_wallet = create_main_wallet_sems(v);
    sems->wallet_server = create_wallet_server_sems(v);
    sems->server_main = create_server_main_sems(v);
    sems->terminate_mutex = create_semaphore(STR_SEM_TERMINATE_MUTEX, 1);
    return sems;
}

void print_all_semaphores(struct semaphores* sems) {
    int val;
    sem_getvalue(sems->main_wallet->free_space, &val);
    printf("main_wallet free_space: %d\n", val);
    sem_getvalue(sems->main_wallet->unread, &val);
    printf("main_wallet unread: %d\n", val);
    sem_getvalue(sems->main_wallet->mutex, &val);
    printf("main_wallet mutex: %d\n", val);

    sem_getvalue(sems->wallet_server->free_space, &val);
    printf("wallet_server free_space: %d\n", val);
    sem_getvalue(sems->wallet_server->unread, &val);
    printf("wallet_server unread: %d\n", val);
    sem_getvalue(sems->wallet_server->mutex, &val);
    printf("wallet_server mutex: %d\n", val);

    sem_getvalue(sems->server_main->free_space, &val);
    printf("server_main free_space: %d\n", val);
    sem_getvalue(sems->server_main->unread, &val);
    printf("server_main unread: %d\n", val);
    sem_getvalue(sems->server_main->mutex, &val);
    printf("server_main mutex: %d\n", val);

    sem_getvalue(sems->terminate_mutex, &val);
    printf("terminate_mutex: %d\n", val);
}

void destroy_all_semaphores(struct semaphores* sems) {
    if (sems) {
        if (sems->terminate_mutex) {
            destroy_semaphore(STR_SEM_TERMINATE_MUTEX, sems->terminate_mutex);
            sems->terminate_mutex = NULL;
        }

        if (sems->main_wallet) {
            if (sems->main_wallet->free_space) {
                destroy_semaphore(STR_SEM_MAIN_WALLET_FREESPACE, sems->main_wallet->free_space);
                sems->main_wallet->free_space = NULL;
            }
            if (sems->main_wallet->unread) {
                destroy_semaphore(STR_SEM_MAIN_WALLET_UNREAD, sems->main_wallet->unread);
                sems->main_wallet->unread = NULL;
            }
            if (sems->main_wallet->mutex) {
                destroy_semaphore(STR_SEM_MAIN_WALLET_MUTEX, sems->main_wallet->mutex);
                sems->main_wallet->mutex = NULL;
            }
            free(sems->main_wallet);
            sems->main_wallet = NULL;
        }

        if (sems->wallet_server) {
            if (sems->wallet_server->free_space) {
                destroy_semaphore(STR_SEM_WALLET_SERVER_FREESPACE, sems->wallet_server->free_space);
                sems->wallet_server->free_space = NULL;
            }
            if (sems->wallet_server->unread) {
                destroy_semaphore(STR_SEM_WALLET_SERVER_UNREAD, sems->wallet_server->unread);
                sems->wallet_server->unread = NULL;
            }
            if (sems->wallet_server->mutex) {
                destroy_semaphore(STR_SEM_WALLET_SERVER_MUTEX, sems->wallet_server->mutex);
                sems->wallet_server->mutex = NULL;
            }
            free(sems->wallet_server);
            sems->wallet_server = NULL;
        }

        if (sems->server_main) {
            if (sems->server_main->free_space) {
                destroy_semaphore(STR_SEM_SERVER_MAIN_FREESPACE, sems->server_main->free_space);
                sems->server_main->free_space = NULL;
            }
            if (sems->server_main->unread) {
                destroy_semaphore(STR_SEM_SERVER_MAIN_UNREAD, sems->server_main->unread);
                sems->server_main->unread = NULL;
            }
            if (sems->server_main->mutex) {
                destroy_semaphore(STR_SEM_SERVER_MAIN_MUTEX, sems->server_main->mutex);
                sems->server_main->mutex = NULL;
            }
            free(sems->server_main);
            sems->server_main = NULL;
        }

        // NÃO faças free(sems) aqui!
        // O free(sems) deve ser feito fora desta função, depois de garantir que não é usado mais vezes.
    }
}

struct triplet_sems* create_triplet_sems(unsigned v, char* freespace_name, char* unread_name, char* mutex_name) {
    struct triplet_sems *triplet = malloc(sizeof(struct triplet_sems));
    if (!triplet) return NULL;
    triplet->free_space = create_semaphore(freespace_name, v);
    triplet->unread = create_semaphore(unread_name, 0);
    triplet->mutex = create_semaphore(mutex_name, 1);
    return triplet;
}


struct triplet_sems* create_main_wallet_sems(unsigned v){
    return create_triplet_sems(
        v,
        STR_SEM_MAIN_WALLET_FREESPACE,
        STR_SEM_MAIN_WALLET_UNREAD,
        STR_SEM_MAIN_WALLET_MUTEX
    );
}

struct triplet_sems* create_wallet_server_sems(unsigned v) {
    return create_triplet_sems(
        v,
        STR_SEM_WALLET_SERVER_FREESPACE,
        STR_SEM_WALLET_SERVER_UNREAD,
        STR_SEM_WALLET_SERVER_MUTEX
    );
}

struct triplet_sems* create_server_main_sems(unsigned v) {
    return create_triplet_sems(
        v,
        STR_SEM_SERVER_MAIN_FREESPACE,
        STR_SEM_SERVER_MAIN_UNREAD,
        STR_SEM_SERVER_MAIN_MUTEX
    );
}
