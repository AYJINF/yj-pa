#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
// #include <stdio.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;

void call_main(uintptr_t *args) {
  int argc = (int)*args++;
  char **argv = (char **)args;
  // printf("args=%ls\n", (uintptr_t *)*args);
  while(*args) {
    // printf("aaaaaaaaaaaaa\n");
    args++;
  }
  args++;
  // if(*args_c) printf("qqqqqqqqqqqqqq\n");
  char **envp = (char **)args;
  // printf("argc=%d\n", argc);
  if(*envp) {
    // printf("uuuuuuuuuuuuuu\n");
    // printf("abb=%ls\n", (uintptr_t *)*args);
    args++;
    // printf("next=%ls\n", (uintptr_t *)*args);
  }
  environ = envp;
  exit(main(argc, argv, envp));
  assert(0);
}
