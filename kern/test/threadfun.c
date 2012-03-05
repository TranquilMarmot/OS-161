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
		kprintf("|%d|", wat);
}

int threadfun(int nargs, char **args){
	kprintf("Thread fun! Making %d threads...\n", nargs);
	(void)args;

	char name[16];

	int i, result;
	for(i = 0; i < nargs; i++){
		//kprintf("Thread fun! Starting number %d\n", i);

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
