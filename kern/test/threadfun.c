#include <types.h>
#include <lib.h>
#include <synch.h>
#include <thread.h>
#include <test.h>

#define THREAD_RUNTIME 100

static struct semaphore **sem = NULL;

static int counter = 0;

static void talkingThread(void* whatever, unsigned long threadNum){
	/* what this thread will print out */
	int wat = '0' + threadNum;
	
	/* get rid of the junk */
	(void)whatever;

	int i;
	for(i = 0; i < THREAD_RUNTIME; i++)
		kprintf("|%d|", wat - '0');

	V(sem);
}

static void unsafecountingThread(void* whatever, unsigned long threadNum){
	/* only way i could get this to work (???) */
	int incamount = '0' +  (threadNum - '0');

	(void)whatever;

	int i;
	for(i = 0; i < incamount; i++){
		counter++;
		kprintf("|%d|", counter);
	}
	
	V(sem);
}

static void initSem(void){
	if(sem == NULL){
		sem = sem_create("sem", 0);
		if(sem == NULL)
			panic("threadtest: sem_create failed\n");
	}
}

int unsafecounter(int nargs, char **args){
	int numthreads, incamount;

        if(nargs == 1){
	        numthreads = 15;
		incamount = 30;
	}else{
	        numthreads = args[1][0] - '0';
		incamount = args[2][0] - '0';
	}

	kprintf("Let's count, without mutex! Making %d threads that count by %d each...\n", numthreads, incamount);

	(void)nargs;
	(void)args;

	initSem();

	char name[16];

	int i, result;
	for(i = 0; i < numthreads; i++){
		snprintf(name, sizeof(name), "count%d", i);

		result = thread_fork(
				name,
				NULL,
				incamount,
				unsafecountingThread,
				NULL
			);

		if(result)
			panic(
				"unsafecount: thread_fork failed %s)\n",
				strerror(result)
			 );
	}

	for(i = 0; i < numthreads; i++)
		P(sem);

	kprintf("\nDone counting! Counter is now %d (expected: %d)\n", counter, numthreads * incamount);

	counter = 0;

	return 0;
}

int threadfun(int nargs, char **args){
	int numthreads;

	if(nargs == 1)
		numthreads = 1;
	else
		numthreads = args[1][0] - '0';


	kprintf("Thread fun! Making %d threads...\n", numthreads);
	
	(void)nargs;
	(void)args;

	initSem();

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
				"threadfun: thread_fork failed %s)\n",
				strerror(result)
			);
	}

	for(i = 0; i < numthreads; i++)
		P(sem);

	return 0;
}
