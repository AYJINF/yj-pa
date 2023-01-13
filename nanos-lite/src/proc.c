#include <proc.h>

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

extern void naive_uload(PCB *pcb, const char *filename);
void context_kload(PCB *pcb, void (*entry)(void *), void *arg);
// void context_uload(PCB *pcb, const char *filename);
void context_uload(PCB *pcb, const char *filename, char *const argv[], char *const envp[]);

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite with arg '%s' for the %dth time!", (uintptr_t)arg, j);
    j ++;
    yield();
  }
}

void init_proc() {
  // context_kload(&pcb[0], hello_fun, (void *)"ab");
  char *argv[] = {NULL};
  context_uload(&pcb[0], "/bin/dummy", argv, NULL);
  // context_kload(&pcb[1], hello_fun, (void *)"cd");
  // context_uload(&pcb[0], "/bin/hello");

  // context_uload(&pcb[0], "/bin/hello", NULL, NULL);
  // context_uload(&pcb[1], "/bin/pal", NULL, NULL);

  // char *argv[] = {"--skip", NULL};
  // context_uload(&pcb[1], "/bin/pal", argv, NULL);
  switch_boot_pcb();

  Log("Initializing processes...");

  // load program here
  // naive_uload(NULL, "/bin/nterm");

}

Context* schedule(Context *prev) {
// save the context pointer
current->cp = prev;

// always select pcb[0] as the new process
// current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);
current = &pcb[0];
// bool flag = (current == &pcb[0]);
// if(flag) printf("pcb[0]\n");
// else printf("pcb[1]\n");

// then return the new context
printf("prev pdir=%p\n", prev->pdir);
printf("current pdir=%p\n", current->cp->pdir);
return current->cp;
}

int execve(const char *filename, char *const argv[], char *const envp[]){
  // printf("filename=%s\n", filename);
  context_uload(&pcb[1], filename, argv, envp);
  switch_boot_pcb();
  yield();
  return 0;
};
