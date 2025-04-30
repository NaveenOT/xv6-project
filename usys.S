#include "syscall.h"
#include "traps.h"

#define SYSCALL(name) \
  .globl name; \
  name: \
    movl $SYS_ ## name, %eax; \
    int $T_SYSCALL; \
    ret

SYSCALL(fork)
SYSCALL(exit)
SYSCALL(wait)
SYSCALL(pipe)
SYSCALL(read)
SYSCALL(write)
SYSCALL(close)
SYSCALL(kill)
SYSCALL(exec)
SYSCALL(getppid)
SYSCALL(open)
SYSCALL(reboot)
SYSCALL(mknod)
SYSCALL(unlink)
SYSCALL(pstree)
SYSCALL(fstat)
SYSCALL(link)
SYSCALL(mkdir)
SYSCALL(chdir)
SYSCALL(dup)
SYSCALL(getpid)
SYSCALL(send)
SYSCALL(recv)
SYSCALL(getstate)
SYSCALL(sbrk)
SYSCALL(sleep)
SYSCALL(uptime)
