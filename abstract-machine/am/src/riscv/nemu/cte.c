#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;
void __am_get_cur_as(Context *c);
void __am_switch(Context *c);

Context* __am_irq_handle(Context *c) {
  __am_get_cur_as(c);
  if (user_handler) {
    Event ev = {0};
    switch (c->mcause) {
      case 1: 
        if(c->GPR1 == -1) ev.event = EVENT_YIELD; // 自陷指令
        else ev.event = EVENT_SYSCALL; // 系统调用
        break;
      default: ev.event = EVENT_ERROR; break;
    }

    c = user_handler(ev, c);
    // printf("user pdir2=%p\n", c->pdir);
    assert(c != NULL);
    c->mepc += 4; // 时钟中断不用+4
  }
  __am_switch(c);
  return c;
}

extern void __am_asm_trap(void);

bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));

  // register event handler
  user_handler = handler;

  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  Context *ret = kstack.end - sizeof(Context);
  ret->mstatus = 0x18000;
  ret->mepc = (uintptr_t)entry;
  ret->GPRx = (uintptr_t)arg;
  ret->pdir = NULL;
  return ret;
}

void yield() {
  asm volatile("li a7, -1; ecall");
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
