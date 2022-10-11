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

#include "sdb.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}

/* Display states of the watchpoints in use */
void isa_wp_display(){
  WP *cur = head;
  if(cur == NULL)printf("No watchpoints\n");
  while(cur != NULL){
    printf("NO.%d,\t %s = %d\n", cur->NO, cur->var, cur->data);
    cur = cur->next;
  }
}

/* Return an empty watchpoint */
WP* new_wp(){
  WP *ans = NULL;
  if(free_ == NULL) assert(0);
  ans = free_;
  free_ = free_->next;
  ans->next = head;
  head = ans;
  return ans;
};

void delete_wp(int NO){
  WP *pre = NULL;
  WP *cur = head;
  while(cur != NULL){
    if(cur->NO == NO){
      if(pre != NULL)pre->next = cur->next;
      else {
        head = cur->next;
        pre = cur->next;
      }
      free_wp(cur);
      return;
    }
    pre = cur;
    cur = cur->next;
  }
  assert(0);
}

/* Free a watchpoint in use */
void free_wp(WP *wp){
  memset(wp->var, 0, sizeof(wp->var));
  wp->data = 0;
  wp->next = free_;
  free_ = wp;
}

void scan_all_wps(){
  WP *cur = head;
  while(cur != NULL){
    bool success = true;
    word_t tmp = expr(cur->var, &success);
    if(!success) assert(0);
    if(tmp != cur->data){
      printf("The value of watchpoint NO.%d has changed.\n Old value = %d\n  New value = %d.\n", cur->NO, cur->data, tmp);
      nemu_state.state = NEMU_STOP;
      cur->data = tmp;
    }
    cur = cur->next;
  }
}

