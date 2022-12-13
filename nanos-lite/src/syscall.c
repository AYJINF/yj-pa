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

void sys_write(Context *c){
  c->GPRx = -1;
  int o_or_e = (int)c->GPR2;
  char *ch = (char *)c->GPR3;
  int len = (int)c->GPR4;
  printf("o_or_e = %d, len = %d\n", o_or_e, len);
  if(o_or_e == 1 || o_or_e == 2){
    for(int i = 0; i < len; i++){
      putch(*ch);
      ch++;
    }
    c->GPRx = len;
  }
}

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  #ifdef CONFIG_STRACE
  Log("Syscall: mcause = 0x%x, GPR1 = 0x%x, GPR2 = 0x%x, GPR3 = 0x%x, GPR4 = 0x%x", c->mcause, c->GPR1, c->GPR2, c->GPR3, c->GPR4);
  #endif

  switch (a[0]) {
    case 0: sys_exit(c); break;
    case 1: sys_yield(c); break;
    case 4: sys_write(c); break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
