#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "mutex.h"

int
sys_fork(void)
{
  return fork();
}

int sys_clone(void)
{
  int fn, stack, arg;
  argint(0, &fn);
  argint(1, &stack);
  argint(2, &arg);
  return clone((void (*)(void*))fn, (void*)stack, (void*)arg);
}

int sys_nice(void)
{
  int inc;
  if (argint(0, &inc) < 0) {
  	return -1;
  }
  argint(0, &inc);
  struct proc *currProc = myproc();
  currProc->nice += inc;
  if (currProc->nice > 19) {
  	currProc->nice = 19;
  }
  if (currProc->nice < -20) {
  	currProc->nice = -20;
  }
  return 0;
}

void sys_macquire(void)
{
  mutex* m;
  argptr(0, (void*)&m, sizeof(*m));
  acquire(&m->lk);
  while (m->locked) {
    myproc()->isWaiting = 1;
    myproc()->wait_lock = m;
    sleep(m, &m->lk);
 }
  myproc()->isWaiting = 0;
  myproc()->locks_held[myproc()->numLocksHeld] = m;
  myproc()->numLocksHeld++;
  m->locked = 1;
  m->pid = myproc()->pid;
  release(&m->lk);
}

void sys_mrelease(void)
{
  mutex *m;
  argptr(0, (void*)&m, sizeof(*m));
  acquire(&m->lk);
  myproc()->numLocksHeld--;
  m->locked = 0;
  m->pid = 0;
  wakeup(m);
 release(&m->lk);
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
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
  if (n == 0) {
    yield();
    return 0;
  }
  acquire(&tickslock);
  ticks0 = ticks;
  myproc()->sleepticks = n;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  myproc()->sleepticks = -1;
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
