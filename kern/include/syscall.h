#ifndef _SYSCALL_H_
#define _SYSCALL_H_

/*
 * Prototypes for IN-KERNEL entry points for system call implementations.
 */

int sys_reboot(int code);
int sys_helloworld(void);
int sys_printint(int value);
int sys__exit(int code);
int sys__write(int fd, const void * buf, size_t nbytes);



#endif /* _SYSCALL_H_ */
