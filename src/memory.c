/**
 * Grupo 40
 * Rodrigo Frutuoso - 61865
 * Simão Alexandre - 61874
 * Tiago Leite - 61863
*/

#include "../inc/memory.h"
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

void* allocate_dynamic_memory(int size)
{
    void *ptr;

    if (size <= 0)
    {
        perror("Error with attributes on allocate_dynamic_memory");
        exit(1);
    }
    ptr = calloc(1, size);
    if (!ptr)
    {
        perror("Error with calloc");
        exit(1);
    }
    return (ptr);
}

void* create_shared_memory(char* name, int size)
{
    int fd;
    void *ptr;
    char memory_name[256];

    if (!name || size <= 0)
    {
        perror("Error with attributes on create_shared_memory");
        exit(1);
    }
    snprintf(memory_name, sizeof(memory_name), "%s_%d", name, getuid());
    fd = shm_open(memory_name, O_CREAT | O_RDWR, 0666);
    if (fd == -1)
    {
        perror("Error with shm_open");
        exit(1);
    }
    if (ftruncate(fd, size) == -1)
    {
        perror("Error with ftruncate");
        close(fd);
        shm_unlink(memory_name);
        exit(1);
    }
    ptr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED)
    {
        perror("Error with nmap");
        close(fd);
        shm_unlink(memory_name);
        exit(1);
    }
    memset(ptr, 0, size);
    return (ptr);
}

void deallocate_dynamic_memory(void* ptr)
{
    if (ptr)
        free(ptr);
}

void destroy_shared_memory(char* name, void* ptr, int size)
{
    if (!name || !ptr || size <= 0)
    {
        perror("Error with attributes on destroy_shared_memory");
        exit(1);
    }
    munmap(ptr, size);
    shm_unlink(name);
}

void write_main_wallets_buffer(struct ra_buffer* buffer, int buffer_size, struct transaction* tx);
void write_wallets_servers_buffer(struct circ_buffer* buffer, int buffer_size, struct transaction* tx);
void write_servers_main_buffer(struct ra_buffer* buffer, int buffer_size, struct transaction* tx);
void read_main_wallets_buffer(struct ra_buffer* buffer, int wallet_id, int buffer_size, struct transaction* tx);
void read_wallets_servers_buffer(struct circ_buffer* buffer, int buffer_size, struct transaction* tx);
void read_servers_main_buffer(struct ra_buffer* buffer, int tx_id, int buffer_size, struct transaction* tx);
