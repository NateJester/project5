#include "mutex.h"
#include "defs.h"
#include "types.h"
#include "spinlock.h"
void minit(mutex* m) {
  initlock(&m->lk, "sleep lock");
  m->locked = 0;
  m->pid = 0;
}
