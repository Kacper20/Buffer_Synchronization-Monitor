#include "tools.h"
int main(int argc, char **argv){
	int val = 0;
	int temp = 0;
	access_queue nowowiejska_queue;
	access_queue nowowiejska_left_queue;
	access_queue nowowiejska_left2_queue;
	access_queue polna_queue;
	access_queue noakowskiego_queue;
	access_queue sniadeckich_queue;
	access_queue lwowska_queue;
	init_access_queue(&nowowiejska_queue, 0);
	init_access_queue(&nowowiejska_left_queue, 4);
	init_access_queue(&nowowiejska_left2_queue, 5);
	init_access_queue(&polna_queue, 6);
	init_access_queue(&noakowskiego_queue, 1);
	init_access_queue(&sniadeckich_queue, 3);
	init_access_queue(&lwowska_queue, 2);
	p_msg("rozmiar: %d\n", get_size(&nowowiejska_queue));
	while(1){
		temp = get_size(&nowowiejska_queue);
		if (temp > 0 && val == 0){
			val = get_from_buffer(&nowowiejska_queue);
			p_msg("Na rondo od strony Nowowiejskiej wjechal", val);
		}
		if (temp == 0)
			temp = get_size(&noakowskiego_queue);
		if (temp > 0 && val == 0){
			val = get_from_buffer(&noakowskiego_queue);
			p_msg("Na rondo od strony Noakowskiego wjechal", val);
		}
		if (temp == 0){
			temp = get_size(&sniadeckich_queue);
			p_msg("Na rondo od strony Sniadeckich wjechal", val);
		}
		if (temp > 0 && val == 0){
			val = get_from_buffer(&nowowiejska_left2_queue);
			p_msg("Na rondo od strony Nowowiejskiej(wschod) wjechal", val);
		}
		if (temp != 0){
			randSleep1();
			switch(val){
				case 2:
				case 3:
				case 4:
					put_in_buffer(&polna_queue, val);
					p_msg("Z ronda na  ul.Polna zjechal", val);
					break;
				case 5:
				case 6:
				case 7:
					put_in_buffer(&lwowska_queue, val);
					p_msg("Z ronda na ul.Lwowska zjechal", val);
					break;
				case 1:
					put_in_buffer(&nowowiejska_left_queue, val);
					p_msg("Z ronda na Nowowiejska w strone Al. Niepodleglosci zjechal", val);
					break;		
			}
		}
		else{
			randSleep();
		}
		val = 0;
		temp = 0;
	}
		
}








