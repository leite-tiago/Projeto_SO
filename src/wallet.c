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
            usleep(300000);
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

void wallet_receive_transaction(struct transaction* tx, int wallet_id, struct info_container* info, struct buffers* buffs) {
    if (info->terminate == 1) {
        return;
    }

    // Lê uma transação do buffer de memória partilhada entre a main e as carteiras
    read_main_wallets_buffer(buffs->buff_main_wallets, wallet_id, info->buffers_size, tx);

    // Verifica se o src_id da transação corresponde ao wallet_id
    if (tx->src_id != wallet_id) {
        tx->id = -1; // Define o id da transação como -1 para indicar que a transação não é válida para esta carteira
    }
}

void wallet_process_transaction(struct transaction* tx, int wallet_id, struct info_container* info){
        // Verifica se a carteira de origem corresponde ao wallet_id
        if (tx->src_id == wallet_id) {
            // Assina a transação
            tx->wallet_signature = wallet_id;
            // Incrementa o contador de transações assinadas pela carteira
            info->wallets_stats[wallet_id]++;
        }
}
void wallet_send_transaction(struct transaction* tx, struct info_container* info, struct buffers* buffs){
    // Escreve a transação assinada no buffer de memória partilhada entre as carteiras e os servidores
    write_wallets_servers_buffer(buffs->buff_wallets_servers, info->buffers_size, tx);
}