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
// #ifdef CONFIG_TARGET_AM
//   am_init_monitor();
// #else
//   init_monitor(argc, argv);
// #endif

  /* Start engine. */
  // engine_start();
   /* test for EXPRs (the amount of data is not general) */
  FILE *fp = fopen("tools/gen-expr/gen-expr.c", "r");
  // while(!feof(fp))
  word_t ans = 0;
  if(fp==NULL)printf("shit\n");
  char test_expr[1000] = {"0"};
  for(int i = 0; i < 6622; i++){
    printf("i=%d\n", i);
    int w1 = fscanf(fp, "%d", &ans);
    printf("yyy\n");
    if(w1)printf("line%d, w1=%d", i+1, w1);
    char *w2 = fgets(test_expr, 1000, fp);
    if(w2 == NULL)printf("line%d, w2==NULL", i+1);
    bool if_success = true;
    word_t tmp = expr(test_expr, &if_success);
    if(if_success == false) printf("line %d fail to be cal\n", i+1);
    if(tmp != ans) printf("line%d is wrong. %d != %d", i+1, tmp, ans);
    ans = 0;
    memset(test_expr, 0, sizeof(test_expr));
  }

  // return is_exit_status_bad();
}
