#ifndef _SYSCALL_H_
#define _SYSCALL_H_

/*
 * Prototypes for IN-KERNEL entry points for system call implementations.
 */

int sys_reboot(int code);
int sys_helloworld(void);
int sys_printint(int value);


#endif /* _SYSCALL_H_ */
