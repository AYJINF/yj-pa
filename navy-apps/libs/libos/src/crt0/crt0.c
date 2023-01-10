#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args) {
  int argc = (int)*args++;
  printf("argc=%d\n", argc);  
  char *args_c = (char *)args;
  char **argv = (char **)args_c;
  while(*args_c) args_c++;
  while(!*args_c) args_c++;
  char **envp = (char **)args_c;
  environ = envp;
  exit(main(argc, argv, envp));
  assert(0);
}
