#include <am.h>
#include <nemu.h>
#include <klib.h>

static AddrSpace kas = {};
static void* (*pgalloc_usr)(int) = NULL;
static void (*pgfree_usr)(void*) = NULL;
static int vme_enable = 0;

static Area segments[] = {      // Kernel memory mappings
  NEMU_PADDR_SPACE
};

#define USER_SPACE RANGE(0x40000000, 0x80000000)

static inline void set_satp(void *pdir) {
  uintptr_t mode = 1ul << (__riscv_xlen - 1);
  asm volatile("csrw satp, %0" : : "r"(mode | ((uintptr_t)pdir >> 12)));
}

static inline uintptr_t get_satp() {
  uintptr_t satp;
  asm volatile("csrr %0, satp" : "=r"(satp));
  return satp << 12;
}

bool vme_init(void* (*pgalloc_f)(int), void (*pgfree_f)(void*)) {
  pgalloc_usr = pgalloc_f;
  pgfree_usr = pgfree_f;

  kas.ptr = pgalloc_f(PGSIZE);

  int i;
  for (i = 0; i < LENGTH(segments); i ++) {
    void *va = segments[i].start;
    for (; va < segments[i].end; va += PGSIZE) {
      map(&kas, va, va, 0);
    }
  }

  set_satp(kas.ptr);
  vme_enable = 1;

  return true;
}

void protect(AddrSpace *as) {
  PTE *updir = (PTE*)(pgalloc_usr(PGSIZE));
  as->ptr = updir;
  as->area = USER_SPACE;
  as->pgsize = PGSIZE;
  // map kernel space
  memcpy(updir, kas.ptr, PGSIZE);
}

void unprotect(AddrSpace *as) {
}

void __am_get_cur_as(Context *c) {
  c->pdir = (vme_enable ? (void *)get_satp() : NULL);
}

void __am_switch(Context *c) {
  if (vme_enable && c->pdir != NULL) {
    set_satp(c->pdir);
  }
}

/*
              Riscv32 Sv32 Page-Table Entry(PTE)
---------------------------------------------------------------
|31      20|19      10|9     8| 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
|  PPN[1]  |  PPN[0]  |  RSW  | D | A | G | U | X | W | R | V |
|    12    |    10    |   2   | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 |
---------------------------------------------------------------

              Sv32 virtual address(va)
  ---------------------------------------------
  |31           22|21      12|11              0|
  |     VPN[1]    |  VPN[0]  |   page offset   |
  |      10       |    10    |        12       |
  ---------------------------------------------

              Sv32 physical address(pa)
------------------------------------------------
|33            22 |21      12|11              0|
|      PPN[1]     |  PPN[0]  |   page offset   |
|       12        |    10    |       12        |
------------------------------------------------
*/
#define MY_PN 0xfffff000
#define MY_VPN_1 0xffc00000
#define MY_PAGE_NUMBER 0xfffff000
void map(AddrSpace *as, void *va, void *pa, int prot) {
  PTE *pte = as->ptr + (((uintptr_t)va & MY_VPN_1) >> 22) * 4;
  if((*pte & PTE_V) == 0){
    printf("www=%d\n", PTE_V);
  }

  // uintptr_t va_vpn = ((uintptr_t)va & MY_PN);
  // uintptr_t pa_ppn = ((uintptr_t)pa & MY_PN);
  // va_vpn = pa_ppn;
}

Context *ucontext(AddrSpace *as, Area kstack, void *entry) {
  Context *ret = kstack.end - sizeof(Context);
  ret->mstatus = 0x18000;
  ret->mepc = (uintptr_t)entry;
  return ret;
}
