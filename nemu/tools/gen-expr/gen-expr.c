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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static int num = 0;
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

uint32_t choose(uint32_t n){
  return rand() % n;
}

/* add to buf*/
void gen(char s){
  buf[num] = s;
  buf[++ num] = '\0';
}

/* add a random number */
void gen_num(){
  int if_hex = choose(2);
  int len = choose(4) + 1; // flexible, to avoid overflow
  // decimal integer
  if(if_hex == 0){
    for(int i = 0; i < len; i++){
      gen('0' + choose(10));
    }
  }
  // hexadecimal-number
  else{
    gen('0');gen('x');
    for(int j = 0; j < len; j++){
      int if_le = choose(2);
      if(if_le == 0) gen('0' + choose(10));
      else{
        switch (choose(6))
        {
        case 0:
          gen('a');
          break;
        case 1:
          gen('b');
          break;
        case 2:
          gen('c');
          break;
        case 3:
          gen('d');
          break;
        case 4:
          gen('e');
          break;
        case 5:
          gen('f');
          break;
        default:
          break;
        }
      }
  }
    }
}
  

/* add an operation */
void gen_rand_op(){
  switch (choose(7))
  {
  case 0:
    gen('=');gen('=');
    break;
  case 1:
    gen('!');gen('=');
    break;
  case 2:
    gen('&');gen('&');
    break;
  case 3:
    gen('+');
    break;
  case 4:
    gen('-');
    break;
  case 5:
    gen('*');
    break;
  case 6:
    gen('/');
    break;
  default:
    break;
  }
}

static void gen_rand_expr() {
  buf[0] = '\0';
  switch (choose(5)) {
    case 0: gen_num(); break;
    case 1: gen('('); gen_rand_expr(); gen(')'); break;
    case 2: gen(' '); gen_rand_expr(); break;
    default: gen_rand_expr(); gen_rand_op(); gen_rand_expr(); break;
  }
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr -Werror"); // add "-Werror" to filter "/0" out
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");  
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
