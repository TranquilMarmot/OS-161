#include <types.h>
#include <lib.h>
#include <synch.h>
#include <thread.h>
#include <test.h>

#define THREAD_RUNTIME 100

static void talkingThread(void* whatever, unsigned long threadNum){
	/* what this thread will print out */
	int wat = '0' + threadNum;
	
	/* get rid of the junk */
	(void)whatever;

	int i;
	for(i = 0; i < THREAD_RUNTIME; i++)
		kprintf("|%d|", wat - '0');
}

int threadfun(int nargs, char **args){
	int numthreads;
	if(nargs == 1){
		/* let's just make one if there's no args */
		numthreads = 1;
	} else if(nargs > 3){
		/* too many args! */
		kprintf("Usage: tfun1 [numthreads]");
		return 0;
	} else{
		/* grab the first character of the second arg as an int (neat trick!) */
		numthreads = args[1][0] - '0';
	}


	kprintf("Thread fun! Making %d threads...\n", numthreads);
	
	(void)args;

	char name[16];

	int i, result;
	for(i = 0; i < numthreads; i++){
		snprintf(name, sizeof(name), "threadfun%d", i);

		result = thread_fork(
				name,
				NULL,
				i,
				talkingThread,
				NULL
			);

		if(result)
			panic(
				"threadtest: thread_fork failed %s)\n",
				strerror(result)
			);
	}

	return 0;
}
