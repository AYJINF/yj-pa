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

word_t vaddr_ifetch(vaddr_t addr, int len) {
  // printf("vaddr_ifetch, addr=%x\n", addr);
  if(isa_mmu_check(addr, len, 0) == MMU_TRANSLATE){
    vaddr_t tmp = addr;
    addr = isa_mmu_translate(addr, len, 0);
    printf("vaddr_ifetch, tmp=%x, addr=%x\n", tmp, addr);
    // Assert( tmp == addr, "不是恒等映射！");
  }
  return paddr_read(addr, len);
}

word_t vaddr_read(vaddr_t addr, int len) {
  // printf("vaddr_read, addr=%x\n", addr);
  if(isa_mmu_check(addr, len, 0) == MMU_TRANSLATE){
    vaddr_t tmp = addr;
    addr = isa_mmu_translate(addr, len, 0);
    printf("vaddr_read, tmp=%x, addr=%x\n", tmp, addr);
    // Assert( tmp == addr, "不是恒等映射！");
  }
  return paddr_read(addr, len);
}

void vaddr_write(vaddr_t addr, int len, word_t data) {
  // printf("vaddr_write, addr=%x\n", addr);
  if(isa_mmu_check(addr, len, 1) == MMU_TRANSLATE){
    vaddr_t tmp = addr;
    addr = isa_mmu_translate(addr, len, 0);
    printf("vaddr_write, tmp=%x, addr=%x\n", tmp, addr);
    // Assert( tmp == addr, "不是恒等映射！");
  }
  paddr_write(addr, len, data);
}
