#include "tools.h"
key_t tab_key []= {SHM_KEY_NOWOWIEJSKA, SHM_KEY_NOAKOWSKIEGO, SHM_KEY_LWOWSKA, SHM_KEY_SNIADECKICH, SHM_KEY_NOWOWIEJSKA_LEFT, SHM_KEY_NOWOWIEJSKA_LEFT2, SHM_KEY_POLNA, SHM_KEY_KOSZYKOWA,
SHM_KEY_NIEPODLEGLOSCI, SHM_KEY_NIEPODLEGLOSCI_UP, SHM_KEY_KOSZYKOWA_RIGHT, SHM_KEY_KOSZYKOWA_RIGHT2, SHM_KEY_WAR_UP1, SHM_KEY_WAR_UP2, SHM_KEY_WAR_DOWN1, SHM_KEY_WAR_DOWN2, SHM_KEY_WAR_DOWN3 };
char * mutex[] = {SEM_NOWOWIEJSKA, SEM_NOAKOWSKIEGO, SEM_LWOWSKA, SEM_SNIADECKICH, SEM_NOWOWIEJSKA_LEFT, SEM_NOWOWIEJSKA_LEFT2, SEM_POLNA, SEM_KOSZYKOWA, SEM_NIEPODLEGLOSCI,
SEM_NIEPODLEGLOSCI_UP, SEM_KOSZYKOWA_RIGHT, SEM_KOSZYKOWA_RIGHT2, SEM_WAR_UP1, SEM_WAR_UP2, SEM_WAR_DOWN1, SEM_WAR_DOWN2, SEM_WAR_DOWN3};
char * tab_free[] = {SEM_NOWOWIEJSKA_FREE_KEY, SEM_NOAKOWSKIEGO_FREE_KEY, SEM_LWOWSKA_FREE_KEY, SEM_SNIADECKICH_FREE_KEY, SEM_NOWOWIEJSKA_LEFT_FREE_KEY, SEM_NOWOWIEJSKA_LEFT2_FREE_KEY,
SEM_POLNA_FREE_KEY, SEM_KOSZYKOWA_FREE_KEY, SEM_NIEPODLEGLOSCI_FREE_KEY, SEM_NIEPODLEGLOSCI_UP_FREE_KEY, SEM_KOSZYKOWA_RIGHT_FREE_KEY, SEM_KOSZYKOWA_RIGHT2_FREE_KEY,
SEM_WAR_UP1_FREE_KEY, SEM_WAR_UP2_FREE_KEY, SEM_WAR_DOWN1_FREE_KEY, SEM_WAR_DOWN2_FREE_KEY, SEM_WAR_DOWN3};
char * tab_full[] = {SEM_NOWOWIEJSKA_FULL_KEY, SEM_NOAKOWSKIEGO_FULL_KEY, SEM_LWOWSKA_FULL_KEY, SEM_SNIADECKICH_FULL_KEY, SEM_NOWOWIEJSKA_LEFT_FULL_KEY, SEM_NOWOWIEJSKA_LEFT2_FULL_KEY,
SEM_POLNA_FULL_KEY, SEM_KOSZYKOWA_FULL_KEY, SEM_NIEPODLEGLOSCI_FULL_KEY, SEM_NIEPODLEGLOSCI_UP_FULL_KEY, SEM_KOSZYKOWA_RIGHT_FULL_KEY, SEM_KOSZYKOWA_RIGHT2_FULL_KEY,
SEM_WAR_UP1_FULL_KEY, SEM_WAR_UP2_FULL_KEY, SEM_WAR_DOWN1_FULL_KEY, SEM_WAR_DOWN2_FULL_KEY, SEM_WAR_DOWN3_FULL_KEY};
/*Nowowiejska - 0
 * noakowskiego - 1
 *  lwowska - 2
 *  sniadeckich - 3
 *  nowowiejska_left - 4
 *  nowowiejska_left2 - 5
 *  polna - 6
 * koszykowa - 7
 * niepodleglosci - 8
 *  niepodleglosci_up - 9
 *  koszykowa_right - 1-
 * koszykows_right2 - 11
 * war_up1 - 12
 * war_up2 - 13
 * war_down1 - 14
 * war_down2 - 15
 * war_down3 - 16
 */
 


void init_access_queue(access_queue *access_q, int queue_id)
{
	int val;
	get_queue(&(access_q->shared_queue) , queue_id);
	access_q->lock_semaphore = sem_open(mutex[queue_id], 0);
	access_q->slot_free_semaphore = sem_open(tab_free[queue_id], 0);
	access_q->slot_full_semaphore = sem_open(tab_full[queue_id], 0);
}
void enter(access_queue *access_q){
	sem_wait(access_q->lock_semaphore);
}
void leave(access_queue *access_q){
	sem_post(access_q->lock_semaphore);
}

void put_in_buffer(access_queue *access_q, int item)
{
	queue *q = access_q->shared_queue;
	enter(access_q);
	if (q->count == MAX)
		wait(access_q, &(q->slot_free), access_q->slot_free_semaphore);
	q->buffer[q->in] = item;
	q->in = (q->in + 1) % MAX;
	q->count += 1;
	notify(access_q, &(q->slot_full), access_q->slot_full_semaphore);
	leave(access_q);
}
int get_from_buffer(access_queue *access_q) {
	int item = -1;
	queue *q = access_q->shared_queue;
	enter(access_q);
	if (q->count == 0)
		wait(access_q, &(q->slot_full), access_q->slot_full_semaphore);
	item = q->buffer[q->out];
	q->out = (q->out + 1 )% MAX;
	fflush(stdout); 
	q->count = q->count - 1;
	notify(access_q, &(q->slot_free), access_q->slot_free_semaphore);
	leave(access_q);
	return item;
}
int get_size(access_queue *access_q){
	int size;
	enter(access_q);
	size =  access_q->shared_queue->count;
	leave(access_q);
	return size;
}

void wait (access_queue *access_q, condition *condition, sem_t *semaphore)
{
	++(condition->waiting_count);
	leave(access_q);
	sem_wait(semaphore);
}
void notify (access_queue *access_q, condition *condition, sem_t *semaphore)
{
	if( condition->waiting_count )
		{
			--(condition->waiting_count);
			sem_post(semaphore);
			enter(access_q);
		}	
}

void get_queue(queue **q, int number){
	int shmid;
	void *ptr;
	if ((shmid = shmget(tab_key[number], QUEUE_SIZE, 0666 )) < 0){
		perror("shmget");
		exit(1);
	}
	if ((ptr = shmat(shmid, NULL, 0)) == (int *)-1){
		perror("shmat");
		exit(1);
	}
	else{
		*q = (queue *)ptr;
	}
	
}
/* kolejki teraz beda stablicowane */
void queue_init(queue *queue){
	queue->in = 0;
	queue->out = 0;
	queue->count = 0;
	queue->slot_free.waiting_count = 0;
	queue->slot_full.waiting_count = 0;
}
void p_msg(const char *msg, int number){
	printf("%s : %d\n", msg, number);
	fflush(stdout);
}

int random_auto_dest(){
	return (rand() % 7) + 1;
}
void randSleep() {
	struct timeval timeout;
	
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	select(0, NULL, NULL, NULL, &timeout);
}
void sleepWhile() {
		struct timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 200000;
		select(0, NULL, NULL, NULL, &timeout);
	}
void randSleep1() {
	double s = 0.01;
	double x;
	struct timeval timeout;
	x = rand()/(double)RAND_MAX;
	s = (-1)*s*log(1 - x) * 100;
	timeout.tv_sec = 7;
	timeout.tv_usec = 10*s;
	select(0, NULL, NULL, NULL, &timeout);
}
 



