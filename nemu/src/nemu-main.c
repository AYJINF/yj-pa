/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <common.h>
#include <stdio.h>
#include <stdlib.h>

void init_monitor(int, char *[]);
void am_init_monitor();
void engine_start();
int is_exit_status_bad();
word_t expr(char *e, bool *success);


int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
#ifdef CONFIG_TARGET_AM
  am_init_monitor();
#else
  init_monitor(argc, argv);
#endif

  /* Start engine. */
  engine_start();
  
  /* Test for EXPRs (Make engine_start() noted if use it) */
  // FILE *fp = fopen("tools/gen-expr/input", "r");
  // unsigned a = 65091;
  // char test_expr[1000]="0xfe43";
  // int flag[6622] = {0}; // to see if there are wrong answers
  // unsigned ans[6622] = {0};
  // unsigned r[6622] = {0};
  // if(fp==NULL){
  //   printf("The fp is error\n");
  // }
  // int j = 0;
  // for(int i = 0; i < 1; i++){
    // memset(test_expr,0,sizeof(test_expr));
    // int w1=fscanf(fp, "%u", &a);
    // char *w2=fgets(test_expr, 1000, fp);
    // if(w1&&w2){
    //   w1=0;
    // }
  //   test_expr[strlen(test_expr)-1]='\0';
  //   printf("ans=%d expr=%s\n",a,test_expr);
  //   bool check;
  //   word_t t = expr(test_expr,&check);
  //   printf("t=%u\n", t);
  //   if(a!=t){
  //     // flag[j] = i+1;
  //     // ans[j] = a;
  //     // r[j] = t;
  //     // j++;
  //     printf("uyyyy\n");
  //   }
  // }
  // printf("flag=%d", flag);
  // for(int k = 0; flag[k] != 0; k++){
  //   printf("line=%d, ans=%u, result=%u\n", flag[k], ans[k], r[k]);
  // }
  // nemu_state.state = NEMU_QUIT;

  return is_exit_status_bad();
}
