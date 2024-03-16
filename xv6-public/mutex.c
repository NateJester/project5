// Sleeping locks

#include "types.h"
#include "defs.h"
#include "param.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"
#include "sleeplock.h"

// Long-term locks for processes
typedef struct {
  uint locked;       // Is the lock held?
  struct spinlock lk; // spinlock protecting this sleep lock
  int pid;           // Process holding lock
} mutex;

void minit(mutex* m) {
  initlock(&m->lk, "sleep lock");
  m->locked = 0;
  m->pid = 0;
}

void macquire(mutex* m) {
  acquire(&m->lk);
  while (m->locked) {
    sleep(m, &m->lk);
  }
  m->locked = 1;
  m->pid = myproc()->pid;
  release(&m->lk);
}

void mrelease(mutex* m)
{
  acquire(&m->lk);
  m->locked = 0;
  m->pid = 0;
  wakeup(m);
  release(&m->lk);
}
