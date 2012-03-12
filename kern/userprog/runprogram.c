/*
 * Sample/test code for running a user program.  You can use this for
 * reference when implementing the execv() system call. Remember though
 * that execv() needs to do more than this function does.
 */

#include <types.h>
#include <kern/unistd.h>
#include <kern/errno.h>
#include <lib.h>
#include <addrspace.h>
#include <thread.h>
#include <curthread.h>
#include <vm.h>
#include <vfs.h>
#include <test.h>

static char **addNullTerminators(int nargs, char **args){
	int i;
	char **ret;
	
	for(i = 0; i < nargs; i++){
		char *src = args[i];
		int size = strlen(src);

		char withnull[size + 1];

		int j;
		for(j = 0; j < size; j++){
			char c = src[j];
			withnull[j] = c;
		}

		withnull[size + 1] = '\0';

		ret[i] = withnull;

		kprintf("withnull: %s prenull: %d postnull: %d\n", withnull, size, strlen(withnull));
	}

	kprintf("printing ret:\n");
	for(i = 0; i < nargs; i++){
		char *prin = ret[i];
		kprintf("ele: %s\n", prin);
	}

	return ret;
}

static int getCopySize(int nargs, char **args){
	int i, result = 0;

	for(i = 0; i < nargs; i++){
		char *arg = args[i];
		int length = strlen(arg);
		result += length * sizeof(char) + 1;
		kprintf("string: %s length: %d result: %d\n", arg, length, result);
	}

	return result;
}

/*
static void copyOutArgs(int nargs, void *nargspoint, char **args, void *argspoint){
	copyout(nargspoint, nargs, sizeof(nargs));
	copyout(argspoint, args, sizeof(args));
}
*/

/*
 * Load program "progname" and start running it in usermode.
 * Does not return except on error.
 *
 * Calls vfs_open on progname and thus may destroy it.
 */
int
runprogram(char *progname, int nargs, char **args)
{
	/*
	kprintf("in runprogram (%d args)\n", nargs);
	int i;
	for(i = 0; i < nargs; i++)
		kprintf("%s\n", args[i]);
	*/

	struct vnode *v;
	vaddr_t entrypoint, stackptr;
	int result;

	/* Open the file. */
	result = vfs_open(progname, O_RDONLY, &v);
	if (result) {
		return result;
	}

	/* We should be a new thread. */
	assert(curthread->t_vmspace == NULL);

	/* Create a new address space. */
	curthread->t_vmspace = as_create();
	if (curthread->t_vmspace==NULL) {
		vfs_close(v);
		return ENOMEM;
	}

	/* Activate it. */
	as_activate(curthread->t_vmspace);

	/* Load the executable. */
	result = load_elf(v, &entrypoint);
	if (result) {
		/* thread_exit destroys curthread->t_vmspace */
		vfs_close(v);
		return result;
	}

	/* Done with the file now. */
	vfs_close(v);

	/* Define the user stack in the address space */
	result = as_define_stack(curthread->t_vmspace, &stackptr);
	if (result) {
		/* thread_exit destroys curthread->t_vmspace */
		return result;
	}

	//char **argswithnull = copyArgs(nargs, args);

	char *argswithnull = addNullTerminators(nargs, args);
	int copysize = getCopySize(nargs, argswithnull);
	void *argspointer;

	int cpyresult;
	cpyresult = copyout((void *)argswithnull, argspointer, copysize);
	if(cpyresult){
		return cpyresult;
	}

	kprintf("copied out\n");

	/* Warp to user mode. */
	md_usermode(nargs /*argc*/, argspointer /*userspace addr of argv*/,
		    stackptr, entrypoint);
	
	/* md_usermode does not return */
	panic("md_usermode returned\n");
	return EINVAL;
}
