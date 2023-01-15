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

#define IRQ_TIMER 0x80000007  // for riscv32

// #define MIE 0x008 // 第3位
// #define MPIE 0x080 // 第7位

word_t isa_raise_intr(word_t NO, vaddr_t epc) {
  cpu.mepc = epc;
  cpu.mcause = NO;
  #ifdef CONFIG_ETRACE
    printf("\033[1;33mThere is a NO.0x%08x exception at pc = 0x%08x !\033[m\n", NO, epc);
  #endif

  if(cpu.mstatus & MIE){
    cpu.mstatus |= MPIE;
    cpu.mstatus &= (~MIE);
  }
  else {
    cpu.mstatus &= (~MPIE);
    cpu.mstatus &= (~MIE);
  }

  return cpu.mtvec;
}

word_t isa_query_intr() {
  // if (cpu.INTR) {
  //   if(cpu.mstatus & MIE){ // 否则CPU处于关中断状态
  //   cpu.INTR = false;
  //   return IRQ_TIMER;
  //   }
  // }
  return INTR_EMPTY;
}
