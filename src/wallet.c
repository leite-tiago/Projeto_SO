/**
 * Grupo 40
 * Rodrigo Frutuoso - 61865
 * Simão Alexandre - 61874
 * Tiago Leite - 61863
*/

#include "../inc/wallet.h"

int execute_wallet(int wallet_id, struct info_container* info, struct buffers* buffs);
void wallet_receive_transaction(struct transaction* tx, int wallet_id, struct info_container* info, struct buffers* buffs);
void wallet_process_transaction(struct transaction* tx, int wallet_id, struct info_container* info);
void wallet_send_transaction(struct transaction* tx, struct info_container* info, struct buffers* buffs);
