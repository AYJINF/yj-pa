#include <memory.h>
#include <proc.h>

static void *pf = NULL;

void* new_page(size_t nr_page) {
  void *ret = pf;
  pf += nr_page * PGSIZE;
  return ret;
}

#ifdef HAS_VME
static void* pg_alloc(int n) {
  assert(n % PGSIZE == 0);
  void *ret = new_page(n / PGSIZE);
  memset(ret, 0, n);
  return ret;
}
#endif

void free_page(void *p) {
  panic("not implement yet");
}

/* The brk() system call handler. */
int mm_brk(uintptr_t brk) {
  // if(brk >= current->max_brk){
  //   size_t nr_page = (brk >> 12) - (current->max_brk >> 12) + 1;
  //   void *tmp =  new_page(nr_page);
  //   for (int i = 0; i < nr_page; i++){
  //     map(&current->as, (void *)(current->max_brk + i * PGSIZE), (void *)(tmp + i * PGSIZE), 1);
  //     // printf("ss\n");
  //   }
      
  //   current->max_brk = ((brk >> 12) + 1) << 12;
  // }

  return 0;
}

void init_mm() {
  pf = (void *)ROUNDUP(heap.start, PGSIZE);
  Log("free physical pages starting from %p", pf);

#ifdef HAS_VME
  vme_init(pg_alloc, free_page);
#endif
}
