#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <math.h>
#include <sys/ipc.h>
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <time.h>
/*Nowowiejska left or right - problem */
#define QUEUE_SIZE 7

#define SEM_NOWOWIEJSKA_FULL_KEY "/nowowiejska_full3"
#define SEM_NOWOWIEJSKA_FREE_KEY "/nowowiejska_free3"
#define SEM_NOAKOWSKIEGO_FULL_KEY "/noakowskiego_full2"
#define SEM_NOAKOWSKIEGO_FREE_KEY "/noakowskiego_free2"
#define SEM_LWOWSKA_FULL_KEY "/lwowska_full2"
#define SEM_LWOWSKA_FREE_KEY "/lwowska_free2"
#define SEM_SNIADECKICH_FULL_KEY "/sniadeckich_full2"
#define SEM_SNIADECKICH_FREE_KEY "/sniadeckich_free2"
#define SEM_NOWOWIEJSKA_LEFT_FULL_KEY "/nowowiejska_left_full2"
#define SEM_NOWOWIEJSKA_LEFT_FREE_KEY "/nowowiejska_left_free2"
#define SEM_NOWOWIEJSKA_LEFT2_FULL_KEY "/nowowiejska_left2_full2"
#define SEM_NOWOWIEJSKA_LEFT2_FREE_KEY "/nowowiejska_left2_free2"
#define SEM_POLNA_FULL_KEY "/polna_full2"
#define SEM_POLNA_FREE_KEY "/polna_free2"
#define SEM_KOSZYKOWA_FREE_KEY "/koszykowa_free2"
#define SEM_KOSZYKOWA_FULL_KEY "/koszykowa_full2"
#define SEM_NIEPODLEGLOSCI_FREE_KEY "/niepodleglosci_free2"
#define SEM_NIEPODLEGLOSCI_FULL_KEY "/niepodleglosci_full2"
#define SEM_NIEPODLEGLOSCI_UP_FREE_KEY "/niepodleglosci_left_free2"
#define SEM_NIEPODLEGLOSCI_UP_FULL_KEY "/niepodleglosci_left_full2"
#define SEM_KOSZYKOWA_RIGHT_FREE_KEY "/koszykowa_right_free2"
#define SEM_KOSZYKOWA_RIGHT_FULL_KEY "/koszykowa_right_full2"
#define SEM_KOSZYKOWA_RIGHT2_FREE_KEY "/koszykowa_right2_free2"
#define SEM_KOSZYKOWA_RIGHT2_FULL_KEY "/koszykowa_right2_full2"
#define SEM_WAR_UP1_FREE_KEY "/war_up1_free2"
#define SEM_WAR_UP1_FULL_KEY "/war_up1_right_full2"
#define SEM_WAR_UP2_FREE_KEY "/war_up2_free2"
#define SEM_WAR_UP2_FULL_KEY "/war_up2_full2"
#define SEM_WAR_DOWN1_FREE_KEY "/war_down1_free2"
#define SEM_WAR_DOWN1_FULL_KEY "/war_down1_full2"
#define SEM_WAR_DOWN2_FREE_KEY "/war_down2_free2"
#define SEM_WAR_DOWN2_FULL_KEY "/war_down2_full2"
#define SEM_WAR_DOWN3_FREE_KEY "/war_down3_free2"
#define SEM_WAR_DOWN3_FULL_KEY "/war_down3_full2"
#define SEM_NOWOWIEJSKA "/nowowiejska_"
#define SEM_NOAKOWSKIEGO "/noakowskiego_"
#define SEM_LWOWSKA "/lwowska_"
#define SEM_SNIADECKICH "/sniadeckich_"
#define SEM_NOWOWIEJSKA_LEFT "/nowowiejska_left_"
#define SEM_NOWOWIEJSKA_LEFT2 "/nowowiejska_left2_"
#define SEM_POLNA "/polna_"
#define SEM_KOSZYKOWA "/koszykowa_"
#define SEM_NIEPODLEGLOSCI "/niepodleglosci_"
#define SEM_NIEPODLEGLOSCI_UP "/niepodleglosci_up_"
#define SEM_KOSZYKOWA_RIGHT "/koszykowa_right_"
#define SEM_KOSZYKOWA_RIGHT2 "/koszykowa_right2_"
#define SEM_WAR_UP1 "/war_up1_"
#define SEM_WAR_UP2 "/war_up2_"
#define SEM_WAR_DOWN1 "/war_down1_"
#define SEM_WAR_DOWN2 "/war_down2_"
#define SEM_WAR_DOWN3 "/war_down3_"
#define SHM_KEY_NOWOWIEJSKA 3001
#define SHM_KEY_NOAKOWSKIEGO 3002
#define SHM_KEY_LWOWSKA 3003
#define SHM_KEY_SNIADECKICH 3004
#define SHM_KEY_NOWOWIEJSKA_LEFT 3005
#define SHM_KEY_NOWOWIEJSKA_LEFT2 3017
#define SHM_KEY_POLNA 3006
#define SHM_KEY_KOSZYKOWA 3007
#define SHM_KEY_NIEPODLEGLOSCI 3008
#define SHM_KEY_NIEPODLEGLOSCI_UP 3009
#define SHM_KEY_KOSZYKOWA_RIGHT 3010
#define SHM_KEY_KOSZYKOWA_RIGHT2 3011
#define SHM_KEY_WAR_UP1 3012
#define SHM_KEY_WAR_UP2 3013
#define SHM_KEY_WAR_DOWN1 3014
#define SHM_KEY_WAR_DOWN2 3015
#define SHM_KEY_WAR_DOWN3 3016
#define DESTINATION_POINTS 7
#define MAX 7

typedef struct condition
{
	int waiting_count;
} condition;

typedef struct queue{
	condition slot_free;
	condition slot_full;
	int in, out, count;
	int buffer[MAX];
} queue;

typedef struct access_queue{
	queue *shared_queue;
	sem_t  *lock_semaphore;
	sem_t  *slot_free_semaphore;
	sem_t  *slot_full_semaphore;
} access_queue;


void init_access_queue(access_queue *access_q, int queue_id);
void enter(access_queue *access_q);
void leave(access_queue *access_q);
void put_in_buffer(access_queue *access_q, int item);
int get_from_buffer(access_queue *access_q);
int get_size(access_queue *access_q);

void wait (access_queue *access_q, condition *condition, sem_t *semaphore);
void notify (access_queue *access_q, condition *condition, sem_t *semaphore);

void p_msg(const char *msg, int number);
void get_queue(queue **q, int number );
void queue_init(queue *queue);

int random_auto_dest();
void randSleep1();
void randSleep();
/* Method to sleep after checking, that we couldn't take car from any of streets */
void sleepWhile(); 
