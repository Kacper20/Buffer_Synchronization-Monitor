#include "tools.h"
int main(int argc, char **argv){
	int val;
	int i;
	void *ptr;
	sem_t *semaphore_full_table[17];
	sem_t *semaphore_mutex_table[17];
	sem_t *semaphore_free_table[17];
	int shmid_table[17];
	queue * queues [17];
	extern int tab_key[];
	extern char *tab_full[];
	extern char *tab_free[];
	extern char *mutex[];
	int sizeof_struct = sizeof(queue);
	for (i = 0; i < 17; i++)
	{
		if ((shmid_table[i] = shmget(tab_key[i], sizeof_struct, 0666 | IPC_CREAT)) < 0){
			perror("shmget");
			exit(1);
		}
		if ((ptr = shmat(shmid_table[i], NULL, 0)) == (int *)-1){
			perror("shmat");
			exit(1);
		}
		queues[i] = (queue *)ptr;
		queue_init(queues[i]);
		semaphore_full_table[i] = sem_open(tab_full[i], O_CREAT, 0644, 0);
		semaphore_free_table[i] = sem_open( tab_free[i], O_CREAT, 0644, 0); 
		semaphore_mutex_table[i] = sem_open( mutex[i], O_CREAT, 0644, 1);
		sem_init(semaphore_full_table[i],1, 0); 
		sem_init(semaphore_free_table[i],1, 0);
		sem_init(semaphore_mutex_table[i],1, 1); 
		
	}
	printf("Wpisz 0, by zakonczyc symulacje!\n");
	scanf("%d", &val);

	for (i = 0; i < 17; i++)
	{
		sem_destroy(semaphore_full_table[i]);
		sem_destroy(semaphore_mutex_table[i]);
		sem_destroy(semaphore_free_table[i]);
		shmctl(shmid_table[i], IPC_RMID, 0);
	}
	
	
}


