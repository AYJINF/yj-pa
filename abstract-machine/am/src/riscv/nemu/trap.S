#define concat_temp(x, y) x ## y
#define concat(x, y) concat_temp(x, y)
#define MAP(c, f) c(f)

#if __riscv_xlen == 32
#define LOAD  lw
#define STORE sw
#define XLEN  4
#else
#define LOAD  ld
#define STORE sd
#define XLEN  8
#endif

#define REGS(f) \
      f( 1)       f( 3) f( 4) f( 5) f( 6) f( 7) f( 8) f( 9) \
f(10) f(11) f(12) f(13) f(14) f(15) f(16) f(17) f(18) f(19) \
f(20) f(21) f(22) f(23) f(24) f(25) f(26) f(27) f(28) f(29) \
f(30) f(31)

#define PUSH(n) STORE concat(x, n), (n * XLEN)(sp);
#define POP(n)  LOAD  concat(x, n), (n * XLEN)(sp);

#define CONTEXT_SIZE  ((32 + 4 + 1) * XLEN)

#define OFFSET_SP     ( 2 * XLEN)
#define OFFSET_NP     (35 * XLEN)

#define OFFSET_CAUSE  (32 * XLEN)
#define OFFSET_STATUS (33 * XLEN)
#define OFFSET_EPC    (34 * XLEN)

#define MY_KERNEL    0
#define MY_USER      1


.align 3
.globl __am_asm_trap
__am_asm_trap:
  csrrw sp, mscratch, sp   # (1) atomically exchange sp and mscratch
  bnez sp, ksp_is_user    # (2) sp != 0时跳转 take the branch if we trapped from user
  csrr sp, mscratch        # (3) if we trapped from kernel, restore the original sp

# 从kernel进入时
  addi sp, sp, -CONTEXT_SIZE
  MAP(REGS, PUSH)              # 压栈
  li t3, MY_KERNEL
  STORE t3, OFFSET_NP(sp)      # c->np = KERNEL(=0)
  csrw mscratch, x0            # ksp = 0
  jal save_context

# 从user进入时
ksp_is_user:
  addi sp, sp, -CONTEXT_SIZE
  MAP(REGS, PUSH)              # 压栈
  csrr t3, mscratch
  STORE t3, OFFSET_SP(sp)      # $sp = ksp
  li t3, MY_USER
  STORE t3, OFFSET_NP(sp)      # c->np = USER(=1)
  csrw mscratch, x0            # ksp = 0

# 保存上下文
save_context:
  csrr t0, mcause
  csrr t1, mstatus
  csrr t2, mepc

  STORE t0, OFFSET_CAUSE(sp)
  STORE t1, OFFSET_STATUS(sp)
  STORE t2, OFFSET_EPC(sp)

# set mstatus.MPRV to pass difftest
  li a0, (1 << 17)
  or t1, t1, a0
  csrw mstatus, t1

  mv a0, sp
  jal __am_irq_handle
  mv sp, a0

# 恢复上下文
  LOAD t1, OFFSET_STATUS(sp)
  LOAD t2, OFFSET_EPC(sp)
  csrw mstatus, t1
  csrw mepc, t2

  LOAD t3, OFFSET_NP(sp)
  bnez t3, switch_sp_for_user            # np != 0即np为USER时跳转

  MAP(REGS, POP)                         # 出栈

# 恢复为kernel
  addi sp, sp, CONTEXT_SIZE              # $sp = c->sp
  mret

# 恢复为user
switch_sp_for_user:
  addi t3, sp, CONTEXT_SIZE
  csrw mscratch, t3                      # ksp = $sp;

  MAP(REGS, POP)                         # 出栈

  LOAD sp, OFFSET_SP(sp)                 # $sp = c->sp
  mret
