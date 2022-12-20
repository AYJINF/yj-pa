#include <common.h>
#include "syscall.h"
#include <fs.h>
#include <sys/time.h>
#include <proc.h>

// #define CONFIG_STRACE 0; // 打开之前先对照fs.c检查files对不对

#ifdef CONFIG_STRACE
char *files[] = {"stdin", "stdout", "stderr", "/dev/events", "/dev/fb", "/proc/dispinfo", 
"/bin/file-test", "/bin/hello", "/bin/event-test", "/bin/bmp-test", "/bin/dummy", "/bin/timer-test", "/share/music/little-star.ogg", 
"/share/music/rhythm/Do.ogg", "/share/music/rhythm/Si.ogg", "/share/music/rhythm/Re.ogg", "/share/music/rhythm/empty.ogg", 
"/share/music/rhythm/Fa.ogg", "/share/music/rhythm/La.ogg", "/share/music/rhythm/Mi.ogg", "/share/music/rhythm/So.ogg", 
"/share/fonts/Courier-13.bdf", "/share/fonts/Courier-8.bdf", "/share/fonts/Courier-9.bdf", "/share/fonts/Courier-12.bdf", 
"/share/fonts/Courier-7.bdf", "/share/fonts/Courier-11.bdf", "/share/fonts/Courier-10.bdf", "/share/files/num", "/share/pictures/projectn.bmp"};
#endif

extern void naive_uload(PCB *pcb, const char *filename);

void sys_exit(Context *c){
  naive_uload(NULL, "/bin/nterm");
  c->GPRx = 0;
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

void sys_execve(Context *c){
  naive_uload(NULL, (char *)c->GPR2);
  c->GPRx = 0;
}

void sys_gettimeofday(Context *c){
  struct timeval *tval = (struct timeval *)c->GPR2;
  uint64_t us = io_read(AM_TIMER_UPTIME).us;
  tval->tv_sec = us / 1000000;
  tval->tv_usec = us % 1000000;
  c->GPRx = 0; // 阿巴阿巴不知道什么情况会是-1
}

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;

  #ifdef CONFIG_STRACE
  switch (a[0])
  {
  case SYS_exit: Log("Syscall: exit. GPR2 = %x, GPR3 = %x, GPR4 = %x", c->GPR2, c->GPR3, c->GPR4);
    break;
  case SYS_yield: Log("Syscall: yield. GPR2 = %x, GPR3 = %x, GPR4 = %x", c->GPR2, c->GPR3, c->GPR4);
    break;
  case SYS_open: Log("Syscall: open file %s.", (char *)c->GPR2);
    break;
  case SYS_read: Log("Syscall: read file %s, buf = %08x, len = %08x", (char *)files[c->GPR2], c->GPR3, c->GPR4);
    break;
  case SYS_write: Log("Syscall: write file %s, buf = %08x, len = %08x", (char *)files[c->GPR2], c->GPR3, c->GPR4);
    break;
  case SYS_close: Log("Syscall: close file %s.", (char *)files[c->GPR2]);
    break;
  case SYS_lseek: Log("Syscall: lseek file %s, offset = %08x, whence = %08x.", (char *)files[c->GPR2], c->GPR3, c->GPR4);
    break;
  case SYS_brk: Log("Syscall: sbrk, program_break = %08x", c->GPR2);
    break;
  case SYS_gettimeofday: Log("Syscall: gettimeofday");
    break;
  default: Log("Syscall: GPR1 = %08x, GPR2 = %08x, GPR3 = %08x, GPR4 = %08x", c->GPR1, c->GPR2, c->GPR3, c->GPR4);
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
    case SYS_execve: sys_execve(c); break;
    case SYS_gettimeofday: sys_gettimeofday(c); break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
