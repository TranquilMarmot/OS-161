

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

int sys_printstring(char *string, int numchars){
	int i;

	// FIXME Use copyin here?
	
	char arr[numchars];
	for(i = 0; i < numchars; i++){
		arr[i] = string[i];
	}

	return kprintf(arr);
}


int sys__exit(int code){
	thread_exit();
	return 0;
}
