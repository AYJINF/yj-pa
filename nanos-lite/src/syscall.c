#include <common.h>
#include "syscall.h"

// #define CONFIG_STRACE 0;

void sys_yield(Context *c){
  yield();
  c->GPRx = 0;
}

void sys_exit(Context *c){
  halt(c->GPRx);
}


void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  #ifdef CONFIG_STRACE
  Log("Syscall: mcause = 0x%x, GPR1 = 0x%x, GPR2 = 0x%x, GPR3 = 0x%x, GPR4 = 0x%x\n", c->mcause, c->GPR1, c->GPR2, c->GPR3, c->GPR4);
  #endif

  switch (a[0]) {
    case 0: sys_exit(c); break;
    case 1: sys_yield(c); break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
