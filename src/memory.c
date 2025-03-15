/**
 * Grupo 40
 * Rodrigo Frutuoso - 61865
 * Simão Alexandre - 61874
 * Tiago Leite - 61863
*/

#include "../inc/memory.h"

void* allocate_dynamic_memory(int size);
void* create_shared_memory(char* name, int size);
void deallocate_dynamic_memory(void* ptr);
void destroy_shared_memory(char* name, void* ptr, int size);
void write_main_wallets_buffer(struct ra_buffer* buffer, int buffer_size, struct transaction* tx);
void write_wallets_servers_buffer(struct circ_buffer* buffer, int buffer_size, struct transaction* tx);
void write_servers_main_buffer(struct ra_buffer* buffer, int buffer_size, struct transaction* tx);
void read_main_wallets_buffer(struct ra_buffer* buffer, int wallet_id, int buffer_size, struct transaction* tx);
void read_wallets_servers_buffer(struct circ_buffer* buffer, int buffer_size, struct transaction* tx);
void read_servers_main_buffer(struct ra_buffer* buffer, int tx_id, int buffer_size, struct transaction* tx);
