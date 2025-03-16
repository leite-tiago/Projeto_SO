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
        if (info->terminate == 1) {  //a execução termina e retorna o número de transações assinadas.
            break;
        }

        wallet_receive_transaction(&tx, wallet_id, info, buffs);
        
        if (tx.id == -1) { // é ignorada e espera-se alguns ms antes de tentar ler uma nova transação do buffer.
            usleep(3000);
            continue;
        }
        
        if (tx.src_id != wallet_id) { // lê uma transação da main apenas caso o src_id da transação seja o seu próprio id
            continue;
        }
        
        if (info->terminate == 0) { //a carteira assina autorizando a transação, encaminhando-a para os servidores.
            wallet_process_transaction(&tx, wallet_id, info);
            signed_transactions++;
            wallet_send_transaction(&tx, info, buffs);
        }
    }
    
    return signed_transactions;
}

void wallet_receive_transaction(struct transaction* tx, int wallet_id, struct info_container* info, struct buffers* buffs){
}
void wallet_process_transaction(struct transaction* tx, int wallet_id, struct info_container* info);
void wallet_send_transaction(struct transaction* tx, struct info_container* info, struct buffers* buffs);
