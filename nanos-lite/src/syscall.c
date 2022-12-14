#include <common.h>
#include "syscall.h"
#include <fs.h>

// #define CONFIG_STRACE 0;

void sys_exit(Context *c){
  halt(c->GPRx);
}

void sys_yield(Context *c){
  yield();
  c->GPRx = 0;
}

void sys_open(Context *c){
  c->GPRx = fs_open((char *)c->GPR2, c->GPR3, c->GPR4);
}

void sys_read(Context *c){
  c->GPRx = fs_read(c->GPR2, (void *)c->GPR3, c->GPR4);
}

void sys_write(Context *c){
  int o_or_e = (int)c->GPR2;
  char *ch = (char *)c->GPR3;
  int len = c->GPR4;
  if(o_or_e == 1 || o_or_e == 2){
    for(int i = 0; i < len; i++){
      putch(*ch);
      ch++;
    }
    c->GPRx = len;
  }
  else(c->GPRx = -1);
}

void sys_close(Context *c){
  c->GPRx = fs_close(c->GPR2);
}

void sys_lseek(Context *c){
 c->GPRx = fs_lseek(c->GPR2, c->GPR3, c->GPR4);
}

void sys_brk(Context *c){
  c->GPRx = 0;
}

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  #ifdef CONFIG_STRACE
  Log("Syscall: mcause = 0x%x, GPR1 = 0x%x, GPR2 = 0x%x, GPR3 = 0x%x, GPR4 = 0x%x", c->mcause, c->GPR1, c->GPR2, c->GPR3, c->GPR4);
  #endif
printf("jb\n");
  switch (a[0]) {
    case SYS_exit: sys_exit(c); break;
    case SYS_yield: sys_yield(c); break;
    case SYS_open: sys_open(c); break;
    case SYS_read: sys_read(c); break;
    case SYS_write: sys_write(c); break;
    case SYS_close: sys_close(c); break;
    case SYS_lseek: sys_lseek(c); break;
    case SYS_brk: sys_brk(c); break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
