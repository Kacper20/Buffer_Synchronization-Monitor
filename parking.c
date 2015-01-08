
#include "tools.h"
/* Parking ONEONEONE*/
int main(int argc, char **argv){
	access_queue nowowiejska_queue;
	access_queue nowowiejska_left_queue;
	int i = 0;
	int auto_dest;
	srand(time(NULL));
	init_access_queue(&nowowiejska_queue, 0);
	init_access_queue(&nowowiejska_left_queue, 4);
	fflush(stdout);
	p_msg("rozmiar poczatek:", get_size(&nowowiejska_queue));
	while (1)
	{	
		auto_dest = random_auto_dest();
			put_in_buffer(&nowowiejska_queue, auto_dest);
			p_msg("Na Nowowiejska w kierunku ronda wjechal", auto_dest);
		randSleep();
		i++;	
	}	
	
}






