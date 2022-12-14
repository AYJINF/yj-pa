#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
#include <assert.h>

int main() {
  struct timeval tval;
  int ret = gettimeofday(&tval, NULL);
  if(ret == -1) assert(0);
  int t0 = tval.tv_sec * 100 + tval.tv_usec / 10000;
  while (1)
  {
    gettimeofday(&tval, NULL);
    int t1 = tval.tv_sec * 100 + tval.tv_usec / 10000;
    // printf("t0 = %d, t1 = %d\n", t0 / 100000, t1 / 100000);
    if(t1-t0 == 5){
        printf("t0 = %d, t1 = %d\n", t0 / 100000, t1 / 100000);
        t0 = t1;
    }
  }
}
