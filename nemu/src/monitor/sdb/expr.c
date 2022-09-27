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

#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <string.h>

enum {
  TK_NOTYPE = 256, TK_EQ, TK_DIGIT

  /* TODO: Add more token types */

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},            // spaces
  {"\\+", '+'},                 // plus
  {"==", TK_EQ},                // equal
  {"-", '-' },                  // minus
  {"\\*", '*'},                 // time
  {"\\/", '/'},                 // divide 
  {"\\(", '('},                 // left parenthesis
  {"\\)", ')'},                 // right parenthesis
  {"[0-9]+", TK_DIGIT},         // decimal integer
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

/* Record tokens. */
static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start  = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        
        switch (rules[i].token_type) {
          case TK_NOTYPE:;
              break;
          case '+':
              tokens[nr_token].type = '+';
              nr_token++;
              break;
          case '-':
              tokens[nr_token].type = '-';
              nr_token++;
              break;
          case '*':
              tokens[nr_token].type = '*';
              nr_token++;
              break;
          case '/':
              tokens[nr_token].type = '/';
              nr_token++;
            break;
          case '(':
              tokens[nr_token].type = '(';
              nr_token++;
              break;
          case ')':
              tokens[nr_token].type = ')';
              nr_token++;
              break;
          case TK_DIGIT:
              tokens[nr_token].type = TK_DIGIT;
              strncpy(tokens[nr_token].str, substr_start, substr_len);
              nr_token++;
              break;
          default: TODO();
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}


static bool check_parentheses(int p, int q){
  int flag = 0;
  if(tokens[p].type != '(' || tokens[q].type != ')')return false;
  for(int i = p; i < q+1; i++){
    if(tokens[i].type == '(')flag++;
    if(tokens[i].type == ')'){
      flag--;
      if(i < q && flag == 0)return false;
    }
  }
  if(flag == 0)return true;
  return false;
}


int s_pri(char s){
  if(s == '+' || s == '-')return 1;
  if(s == '*' || s == '/')return 2;
  else return 100;
}


word_t expr_eval(int p, int q){
  if(p > q){
    printf("A bad expression!");
    assert(0);
  }
  else if(p == q){
    return atoi(tokens[p].str);
  }
  else if (check_parentheses(p, q) == true){
    return expr_eval(p + 1, q - 1);
  }
  else{
    int op = p;
    int flag_par = 0;   // if there is a pair of parentheses 
    for(int i = p; i < q+1; i++){
      if(tokens[i].type == '(')flag_par++;
      if(tokens[i].type == ')')flag_par--;
      if(flag_par == 0){
        int s1 = s_pri(tokens[op].type);
        int s2 = s_pri(tokens[i].type);
        if(s1 >= s2)op = i;
      }
    }
    int val1 = expr_eval(p, op - 1);
    int val2 = expr_eval(op + 1, q);

    switch (tokens[op].type) {
      case '+': return val1 + val2;
      case '-': return val1 - val2;
      case '*': return val1 * val2;
      case '/': return val1 / val2;
      default: assert(0);
    }
  }
}


word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  printf("%d\n", expr_eval(0, nr_token));
  // /* TODO: Insert codes to evaluate the expression. */
  // TODO();

  return 0;
}
