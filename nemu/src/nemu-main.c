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
  // engine_start();
  
  FILE *fp = fopen("tools/gen-expr/input", "r");
  unsigned a;
  char test_expr[1000] = {"(0x712)"};
  if(fp==NULL){
    printf("The fp is error\n");
  }
  for(int i = 0; i < 1; i++){
    // memset(test_expr,0,sizeof(test_expr));
    // int w1=fscanf(fp, "%u", &a);
    // char *w2=fgets(test_expr, 1000, fp);
    // if(w1&&w2){
    //   w1=0;
    // }
    a = 1810;
    printf("?\n");
    printf("len_expr=%ld\n", strlen(test_expr));
    test_expr[strlen(test_expr)]='\0';
    printf("%d %s\n",a,test_expr);
    bool check;
    word_t t = expr(test_expr,&check);
    if(a!=t){
      printf("shit\n");
    }
    nemu_state.state = NEMU_QUIT;

  }

  return is_exit_status_bad();
}
