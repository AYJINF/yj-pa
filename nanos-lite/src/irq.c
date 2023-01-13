#include <common.h>

void do_syscall(Context *c);
Context* schedule(Context *prev);

static Context* do_event(Event e, Context* c) {
  switch (e.event) {
    case 1: 
      printf("There is a YIELD event!\n"); 
      return schedule(c);
      // printf("There is a YIELD event!\n"); break; // 自陷指令
    case 2: do_syscall(c); break; // 系统调用指令
    default: panic("Unhandled event ID = %d", e.event);
  }

  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}
