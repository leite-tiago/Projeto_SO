/**
 * Grupo 40
 * Rodrigo Frutuoso - 61865
 * Simão Alexandre - 61874
 * Tiago Leite - 61863
*/

#include "../inc/server.h"

int execute_server(int server_id, struct info_container* info, struct buffers* buffs);
void server_receive_transaction(struct transaction* tx, struct info_container* info, struct buffers* buffs);
void server_process_transaction(struct transaction* tx, int server_id, struct info_container* info);
void server_send_transaction(struct transaction* tx, struct info_container* info, struct buffers* buffs);
