#include <common.h>

void do_syscall(Context *c);
Context* schedule(Context *prev);

static Context* do_event(Event e, Context* c) {
  switch (e.event) {
    case EVENT_YIELD:  // 自陷指令
      // printf("There is a YIELD event!\n"); 
      return schedule(c);
      // printf("There is a YIELD event!\n"); break;
    case EVENT_SYSCALL:  // 系统调用指令
      do_syscall(c); break;
    case EVENT_IRQ_TIMER:  // 时钟中断指令
      Log("Do_event accepts EVENT_IRQ_TIMER");
      return schedule(c);
    default: panic("Unhandled event ID = %d", e.event);
  }

  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}
