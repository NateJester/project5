#include "spinlock.h"
typedef struct {
 	uint locked;       // Is the lock held?
  	struct spinlock lk; // spinlock protecting this sleep lock
  	int pid;           // Process holding lock
} mutex;
