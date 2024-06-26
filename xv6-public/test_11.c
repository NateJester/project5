#include "types.h"
#include "user.h"

mutex m;

void fn1(void* arg) {
  nice(10);
  macquire(&m);
  for (int i = 0; i < 10000; i++) {
    if (i % 1000 == 0) {
      // sleep(0) is equivalent to yield(). This gives the scheduler more chances to do scheduling
      sleep(0);
    }
  }
  mrelease(&m);
  sleep(0);
  for (int i = 0; i < 10000; i++) {
    if (i % 1000 == 0) {
      // sleep(0) is equivalent to yield(). This gives the scheduler more chances to do scheduling
      sleep(0);
    }
  }

  exit();
}

void fn2(void* arg) {
  sleep(0);
  nice(-10);
  macquire(&m);
  for (int i = 0; i < 10000; i++) {
    if (i % 1000 == 0) {
      sleep(0);
    }
  }
  mrelease(&m);
  
  exit();
}

int main() {
  minit(&m);
  char* stack1 = (char*)malloc(4096);
  char* stack2 = (char*)malloc(4096);

  clone(fn1, stack1 + 4096, 0);
  clone(fn2, stack2 + 4096, 0);
  sleep(0);

  for (int i = 0; i < 10000; i++) {
    if (i % 1000 == 0) {
      sleep(0);
    }
  }

  wait();
  wait();
  exit();
}
