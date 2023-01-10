#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args) {
  int argc = (int)*args++;
  char *args_c = (char *)args;
  char **argv = (char **)args_c;
  printf("args_c=%ls\n", (uintptr_t *)*args);
  while(*args_c) {
    printf("aaaaaaaaaaaaa\n");
    args_c++;
  }
  args_c++;
  if(*args_c) printf("qqqqqqqqqqqqqq\n");
  char **envp = (char **)args_c;
  // printf("argc=%d\n", argc);
  if(*envp) {
    printf("uuuuuuuuuuuuuu\n");
    // printf("abb=%s", (char *)*args_c);
  }
  environ = envp;
  exit(main(argc, argv, envp));
  assert(0);
}
