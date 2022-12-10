#include <proc.h>
#include <elf.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

size_t ramdisk_read(void *buf, size_t offset, size_t len);

static uintptr_t loader(PCB *pcb, const char *filename) {
  Elf_Ehdr elf_ehdr;
  // FILE *elf = fopen(filename, "r");
  // fread(&elf_ehdr, sizeof(elf_ehdr), 1, elf);
  ramdisk_read(&elf_ehdr, 0, sizeof(elf_ehdr));

  assert(*(uint32_t *)elf_ehdr.e_ident == 0x464c457f);

  #if defined(__ISA_AM_NATIVE__)
  # define EXPECT_TYPE EM_X86_64
  #elif defined(__ISA_X86__)
  # define EXPECT_TYPE EM_X86_64
  #elif defined(__ISA_MIPS32__)
  # define EXPECT_TYPE EM_MIPS
  #elif defined(__ISA_RISCV32__) || defined(__ISA_RISCV64__)
  # define EXPECT_TYPE EM_RISCV
  #else
  # error Unsupported ISA
  #endif
  assert(EXPECT_TYPE == elf_ehdr.e_machine);

  for(int i = 0; i < elf_ehdr.e_phnum; i++){
    Elf_Phdr elf_phdr;
    // fseek(elf, elf_ehdr.e_phoff + elf_ehdr.e_phentsize * i, SEEK_SET);
    // fread(&elf_phdr, sizeof(elf_phdr), 1, elf);
    // printf("beforezyy, i=%d, e_phoff=%p\n", i, elf_ehdr.e_phoff);
    ramdisk_read(&elf_phdr, elf_ehdr.e_phoff + elf_ehdr.e_phentsize * i, sizeof(elf_phdr));  
    

    if(elf_phdr.p_type == PT_LOAD){
      // fseek(elf, elf_phdr.p_offset, SEEK_SET);
      // fread((void *)elf_phdr.p_vaddr, sizeof(elf_phdr.p_filesz), 1, elf);
      // printf("zyy, len=%p\n", elf_phdr.p_filesz);
      ramdisk_read((void *)elf_phdr.p_vaddr, elf_phdr.p_offset, elf_phdr.p_filesz);
      memset((void *)elf_phdr.p_vaddr + elf_phdr.p_filesz, 0, elf_phdr.p_memsz - elf_phdr.p_filesz);
    }
  }

  return elf_ehdr.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry); // ????????????????????
  ((void(*)())entry) ();
}

