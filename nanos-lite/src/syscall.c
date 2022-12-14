#include <common.h>
#include "syscall.h"
#include <fs.h>

// #define CONFIG_STRACE 0;

#ifdef CONFIG_STRACE
char *files[] = {"stdin", "stdout", "stderr", "/bin/file-test", "/bin/hello", "/bin/dummy", "/share/music/little-star.ogg", 
"/share/music/rhythm/Do.ogg", "/share/music/rhythm/Si.ogg", "/share/music/rhythm/Re.ogg", "/share/music/rhythm/empty.ogg", 
"/share/music/rhythm/Fa.ogg", "/share/music/rhythm/La.ogg", "/share/music/rhythm/Mi.ogg", "/share/music/rhythm/So.ogg", 
"/share/fonts/Courier-13.bdf", "/share/fonts/Courier-8.bdf", "/share/fonts/Courier-9.bdf", "/share/fonts/Courier-12.bdf", 
"/share/fonts/Courier-7.bdf", "/share/fonts/Courier-11.bdf", "/share/fonts/Courier-10.bdf", "/share/files/num", "/share/pictures/projectn.bmp"};
#endif

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
  c->GPRx = fs_write(c->GPR2, (void *)c->GPR3, c->GPR4); // 阿巴阿巴
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
  switch (a[0])
  {
  case SYS_exit: Log("Syscall: exit. GPR2 = 0x%x, GPR3 = 0x%x, GPR4 = 0x%x", c->GPR2, c->GPR3, c->GPR4);
    break;
  case SYS_yield: Log("Syscall: yield. GPR2 = 0x%x, GPR3 = 0x%x, GPR4 = 0x%x", c->GPR2, c->GPR3, c->GPR4);
    break;
  case SYS_open: Log("Syscall: open file %s.", (char *)c->GPR2);
    break;
  case SYS_read: Log("Syscall: read file %s, buf = 0x%08x, len = 0x%08x", (char *)files[c->GPR2], c->GPR3, c->GPR4);
    break;
  case SYS_write: Log("Syscall: write file %s, buf = 0x%08x, len = 0x%08x", (char *)files[c->GPR2], c->GPR3, c->GPR4);
    break;
  case SYS_close: Log("Syscall: close file %s.", (char *)files[c->GPR2]);
    break;
  case SYS_lseek: Log("Syscall: lseek file %s, offset = 0x%08x, whence = 0x%08x.", (char *)files[c->GPR2], c->GPR3, c->GPR4);
    break;
  case SYS_brk: Log("Syscall: sbrk, program_break = 0x%08x", c->GPR2);
    break;
  default: Log("Syscall: GPR1 = 0x%08x, GPR2 = 0x%08x, GPR3 = 0x%08x, GPR4 = 0x%08x", c->GPR1, c->GPR2, c->GPR3, c->GPR4);
    break;
  }
  #endif

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
