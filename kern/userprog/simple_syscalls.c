


#include <kern/unistd.h>

int sys_helloworld(void){
	kprintf("Hey world, how's it goin'\n");

	return 0;
}	
