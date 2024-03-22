#include "spinlock.h"
typedef struct {
 	uint locked;       // Is the lock held?
  	struct spinlock lk; // spinlock protecting this mutex
  	int pid;           // Process holding lock
} mutex;
