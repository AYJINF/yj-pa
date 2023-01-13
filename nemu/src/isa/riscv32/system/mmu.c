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
#include <memory/paddr.h>
#include <memory/vaddr.h>


#define PTE_V 0x01
#define PTE_R 0x02
#define PTE_W 0x04
#define PTE_X 0x08
#define PTE_U 0x10
#define PTE_A 0x40
#define PTE_D 0x80

#define MY_PN 0xfffff000
#define MY_VPN_1 0xffc00000
#define MY_VPN_0 0x003ff000
#define MY_PAGE_NUMBER 0xfffff000
#define MY_PDE_ATT 0x3ff
#define MY_SATP_PPN 0x3fffff
/*
cpu.stap(Supervisor Address Translation and Protection Register)
---------------------------
|  31  |30    22|21      0|
| MODE |  ASID  |   PPN   |
|  1   |   9    |   22    |
---------------------------

              Riscv32 Sv32 Page-Table Entry(PTE) likely PDE
---------------------------------------------------------------
|31      20|19      10|9     8| 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
|  PPN[1]  |  PPN[0]  |  RSW  | D | A | G | U | X | W | R | V |
|    12    |    10    |   2   | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 |
---------------------------------------------------------------

              Sv32 virtual address(vaddr)
  ---------------------------------------------
  |31           22|21      12|11              0|
  |     VPN[1]    |  VPN[0]  |   page offset   |
  |      10       |    10    |        12       |
  ---------------------------------------------

              Sv32 physical address(paddr)
------------------------------------------------
|33            22 |21      12|11              0|
|      PPN[1]     |  PPN[0]  |   page offset   |
|       12        |    10    |       12        |
------------------------------------------------
*/
// 对内存区间为[vaddr, vaddr + len), 类型为type的内存访问进行地址转换
paddr_t isa_mmu_translate(vaddr_t vaddr, int len, int type) {
   // 处理pde
  paddr_t pde_addr = ((cpu.satp & MY_SATP_PPN) << 12) | (((vaddr & MY_VPN_1) >> 22) * 4);
  uintptr_t pde = paddr_read(pde_addr, 4);
  Assert((pde & PTE_V) != 0, "pde goes wrong in mmu.c!");
  
  // 处理pte
  paddr_t pte_addr = (((pde & (~MY_PDE_ATT)) >> 10) << 12) | (((vaddr & MY_VPN_0) >> 12) * 4);
  uintptr_t pte = paddr_read(pte_addr, 4);
  Assert((pte & PTE_V) != 0, "pte goes wrong in mmu.c!");

  switch (type)
  {
  case 0: // 读取
    paddr_write(pte_addr, 4, pte | PTE_A); // Access
    break;
  case 1: // 写入
    paddr_write(pte_addr, 4, pte | PTE_A); // Access
    paddr_write(pte_addr, 4, pte | PTE_D); // Dirty (阿巴阿巴不确定)
    break;
  default:
    Assert(0, "内存访问type=%d", type);
    break;
  }

  paddr_t pg_paddr = (((pte & (~MY_PDE_ATT)) >> 10) << 12) | (vaddr & (~MY_PAGE_NUMBER));
  return pg_paddr | MEM_RET_OK;
}
