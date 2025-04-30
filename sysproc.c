#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

extern struct proc proc[NPROC];
extern struct spinlock proc_lock;

int sys_send(void)
{
  char *msg;

  // Retrieve the user message from space
  if (argptr(0, &msg, 128) < 0) {
    cprintf("sys_send: Invalid argument\n");
    return -1;
  }

  acquire(&proc_lock);  // Acquire the lock for safe access

  // Copy the message to the shared memory
  safestrcpy(proc->shared_msgbuf, msg, sizeof(proc->shared_msgbuf));
  proc->has_msg = 1;  // Indicate message is available

  cprintf("sys_send: Message sent: %s\n", msg);

  // If the receiver is sleeping, make it runnable
  if (proc->state == SLEEPING) {
    proc->state = RUNNABLE;  // Make it runnable
    release(&proc_lock);  // Release lock before scheduling
    sched();  // Yield CPU to allow the receiver to proceed
    return 0;
  }

  release(&proc_lock);  // Release the lock if no scheduling needed
  return 0;  // Success
}
int sys_recv(void)
{
  char *buf;

  // Retrieve the user buffer to store the message
  if (argptr(0, &buf, 128) < 0) {
    cprintf("sys_recv: Invalid argument\n");
    return -1;
  }

  acquire(&proc_lock);  // Acquire the lock for safe access

  // Wait for a message (blocking)
  while (proc->has_msg == 0) {
    proc->state = SLEEPING;  // Mark the process as sleeping
    sched();  // Yield CPU and let other processes run
  }

  // A message is available, copy it to the user buffer
  safestrcpy(buf, proc->shared_msgbuf, 128);  // Copy the message to the buffer
  proc->has_msg = 0;  // Clear the message flag

  release(&proc_lock);  // Release the lock before returning
  return 0;  // Success
}



int
sys_fork(void)
{
  return fork();
}
int sys_getprocs(void) {
    char *addr;
    int max;
  initlock(&proc_lock, "proc_lock");
    // Get the user-space buffer address and the maximum number of processes to retrieve
    if (argint(1, &max) < 0 || argptr(0, &addr, sizeof(struct proc_info) * max) < 0)
        return -1;

    struct proc_info info[max]; // Array to store process information
    int count = 0;
    struct proc *p;

    // Lock the process array to safely iterate over it
    acquire(&proc_lock);  // Assume proc_lock is a spinlock for the global proc array

    // Iterate through the process table (proc array) and fill in process info
    for (p = proc; p < &proc[NPROC] && count < max; p++) {
        if (p->state != UNUSED) {  // Only add processes that are not UNUSED
            info[count].pid = p->pid;
            safestrcpy(info[count].name, p->name, sizeof(info[count].name));
            count++;
        }
    }

    // Release the lock
    release(&proc_lock);

    // Copy the process information to the user-space buffer
    if (copyout(myproc()->pgdir, (uint)addr, (char *)info, count * sizeof(struct proc_info)) < 0)
        return -1;

    return count;  // Return the number of processes copied
}
int
sys_exit(void)
{
  int status;
  if(argint(0, &status) < 0)
    return -1;
  exit();
  return 0; // not reached
}
int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
