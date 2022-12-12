#include <common.h>

static Context* do_event(Event e, Context* c) {
  switch (e.event) {
    case 1: printf("There is a YIELD event!\n"); break; // 自陷指令
    case 2: printf("There is a SYSCALL event!\n"); break; // 系统调用指令
    default: panic("Unhandled event ID = %d", e.event);
  }

  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}
