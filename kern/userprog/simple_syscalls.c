

#include <types.h>
#include <kern/unistd.h>
#include <thread.h>

int sys_helloworld(void){
	kprintf("Hey world, how's it goin'\n");

	return 0;
}

int sys_printint(int value){
	return kprintf("%d", value);
}


int sys__exit(int code){
	thread_exit();
	return 0;
}
