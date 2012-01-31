

#include <types.h>
#include <kern/unistd.h>
#include <thread.h>

int sys_helloworld(void){
	kprintf("Hey world, how's it goin'\n");

	return 0;
}


int sys__exit(int code){
	thread_exit();
	return 0;
}
