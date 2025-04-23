#include "synchronization.h"


sem_t* create_semaphore(char *name, unsigned v);

void destroy_semaphore(char *name, sem_t *sem);

struct semaphores* create_all_semaphores(unsigned v);

void print_semaphores(struct semaphores* sems);

void destroy_semaphores(struct semaphores* sems);

struct triplet_sems* create_triplet_sems(unsigned v, 
	char*freespace_name1,char*unread_name,char*mutex_name);

struct triplet_sems* create_main_wallet_sems(unsigned v);

struct triplet_sems* create_wallet_server_sems(unsigned v);

struct triplet_sems* create_server_main_sems(unsigned v);
