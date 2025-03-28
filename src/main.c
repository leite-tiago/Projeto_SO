/**
 * Grupo 40
 * Rodrigo Frutuoso - 61865
 * Simão Alexandre - 61874
 * Tiago Leite - 61863
*/

#include "../inc/main.h"


void main_args(int argc, char *argv[], struct info_container *info);
void create_dynamic_memory_structs(struct info_container* info, struct buffers* buffs);
void create_shared_memory_structs(struct info_container* info, struct buffers* buffs);
void destroy_dynamic_memory_structs(struct info_container* info, struct buffers* buffs);
void destroy_shared_memory_structs(struct info_container* info, struct buffers* buffs);
void create_processes(struct info_container* info, struct buffers* buffs);
void user_interaction(struct info_container* info, struct buffers* buffs);
void write_final_statistics(struct info_container* info);
void end_execution(struct info_container* info, struct buffers* buffs);
void wait_processes(struct info_container* info);
void print_balance(struct info_container* info);
void create_transaction(int* tx_counter, struct info_container* info, struct buffers* buffs);
void receive_receipt(struct info_container* info, struct buffers* buffs);
void print_stat(int tx_counter, struct info_container* info);
void help();

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
