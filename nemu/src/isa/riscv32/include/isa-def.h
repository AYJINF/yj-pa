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

#ifndef __ISA_RISCV32_H__
#define __ISA_RISCV32_H__

#include <common.h>

#define MIE 0x008 // 第3位
#define MPIE 0x080 // 第7位

typedef struct {
  word_t gpr[32];
  word_t mepc, mstatus, mcause, mtvec, satp; // CSRs
  vaddr_t pc;
  bool INTR; // 阿巴阿巴
} riscv32_CPU_state;

// decode
typedef struct {
  union {
    uint32_t val;
  } inst;
} riscv32_ISADecodeInfo;

/*
cpu.stap(Supervisor Address Translation and Protection Register)
---------------------------
|  31  |30    22|21      0|
| MODE |  ASID  |   PPN   |
|  1   |   9    |   22    |
---------------------------
*/
// 检查当前系统状态下对内存区间为[vaddr, vaddr + len), 类型为type的访问是否需要经过地址转换.
// #define isa_mmu_check(vaddr, len, type) (MMU_DIRECT)
#define isa_mmu_check(vaddr, len, type) (cpu.satp & (1 << 31) ? MMU_TRANSLATE : MMU_DIRECT) // PA无MMU_FAIL

#endif
