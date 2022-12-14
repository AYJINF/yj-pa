#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
#include <NDL.h>
#include <assert.h>

// int main() {
//   struct timeval tval;
//   int ret = gettimeofday(&tval, NULL);
//   if(ret == -1) assert(0);
//   int t0 = tval.tv_sec * 10 + tval.tv_usec / 100000; // 单位为0.1s（ds）
//   while (1)
//   {
//     gettimeofday(&tval, NULL);
//     int t1 = tval.tv_sec * 10 + tval.tv_usec / 100000;
//     if(t1-t0 == 5){
//         printf("t0 = %d, t1 = %d\n", t0, t1);
//         t0 = t1;
//     }
//   }
// }
int main(){
  NDL_Init(0);
  uint32_t t0 = NDL_GetTicks();
  while(1){
    uint32_t t1 = NDL_GetTicks();
    if(t1 - t0 == 500){
      printf("t0 = 0x%08x, t1 = 0x%08x\n", t0, t1);
      t0 = t1;
    }
  }
  NDL_Quit();
}