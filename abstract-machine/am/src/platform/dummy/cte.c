#include <am.h>

bool cte_init(Context*(*handler)(Event, Context*)) {
  printf("????????????\n");
  return false;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  return NULL;
}

void yield() {
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
