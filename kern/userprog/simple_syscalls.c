#include <types.h>
#include <kern/unistd.h>
#include <thread.h>
#include <lib.h>
#include <kern/errno.h>

int _helloworld(void){
	kprintf("Hey world, how's it goin'\n");

	return 0;
}

int _printint(int value){
	return kprintf("%d", value);
}

int _printstring(char *string, int numchars){
	int i;

	// FIXME Use copyin here?
	
	char arr[numchars];
	for(i = 0; i < numchars; i++){
		arr[i] = string[i];
	}

	return kprintf(arr);
}


int __exit(int code){
	thread_exit();
	return 0;
}

int __write(int src, const void * dest, size_t nbytes){
	if (src != STDOUT_FILENO) 
		return -EBADF;
	
	/*
	if (nbytes >= KBUFSIZE) 
		return -EFAULT;
	*/

	int err;
	if (!(err = copyin ((const_userptr_t)src, dest, nbytes))){
		kprintf("src: %u, dest %s, numbytes %u\n", src, (char *)dest, nbytes);
		return -err;
	}

	char * str = (char *) dest;

	/* make sure the string is 0 terminated */
	str[nbytes] = '\0';
	
	/* kprintf returns the number of chars printed */
	return kprintf("%s", str);
}
